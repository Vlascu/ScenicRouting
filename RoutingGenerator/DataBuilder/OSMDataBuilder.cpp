#include "OSMDataBuilder.h"
#include "JsonHandler.h"
#include "DataConstants.h"

#include <iomanip>

using InfoToSave = std::vector<std::pair<Coordinate, int>>;

OSMDataBuilder::OSMDataBuilder()
{
	m_amenities = { "bar", "cafe", "restaurant", "fast_food", "pub", "monument", "artwork", "park", "plaza", "biergarten"
		, "bicycle_rental", "fountain", "planetarium", "place_of_worship", "theatre", "courthouse", "townhall", "clock", "monastery", "nightclub" };

	m_leisure = { "dog_park", "garden", "marina", "nature_reserve", "park", "resort", "stadium", "swimming_area", "water_park" };

	m_tourism = { "museum", "aquarium", "artwork", "attraction", "gallery", "theme_park", "viewpoint", "zoo" };

	m_historic = { "aircraft", "aqueduct", "building", "cannon", "castle", "castle_wall", "church", "citywalls", "fort"
		, "locomotive", "monastery", "tank", "tomb", "tower" };

}

void OSMDataBuilder::BuildData(const Coordinate& start, const Coordinate& end, double desiredTime, const ITimeMatrixCalculatorPtr timeCalculator, const std::string& path)
{
	std::ofstream output(path);

	if (!output.is_open())
	{
		std::cerr << "Can't open file at " << path << std::endl;
		exit(1);
	}

	int poiCount = 0;

	auto [startLat, startLon] = start;
	auto [endLat, endLon] = end;

	double centerLat = (startLat + endLat) / 2;
	double centerLon = (startLon + endLon) / 2;

	double searchRadius = DataBuildingConstants::AVERAGE_KMH_WALKING_SPEED * desiredTime / DataBuildingConstants::GENERAL_TIME_CONVERSION;

	std::string osmData = FetchPost(CreateQuery(searchRadius, centerLat, centerLon), "http://overpass-api.de/api/interpreter");

	auto charData = const_cast<char*>(osmData.c_str());
	auto elements = JsonHandler::GetElementsArray(charData, "elements");

	CoordinateList coordinates;

	output << "#pois coordinates and priorities" << std::endl;
	output << std::fixed << std::setprecision(7) << startLat << " " << startLon << " " << 0 << std::endl;

	coordinates.push_back(start);

	for (const auto& element : elements)
	{
		double lat = 0.0;
		double lon = 0.0;
		int priority = 0;

		Tags tags;

		if (JsonHandler::HasElement(element, "tags"))
		{
			tags = JsonHandler::GetTags(element);
		}

		if (JsonHandler::HasElement(element, "type") && JsonHandler::GetElement<std::string>(element, "type") == "way")
		{
			std::vector<double> lats, lons;

			for (const auto& nodeGeometry : JsonHandler::GetElementsArray(element, "geometry"))
			{
				lats.push_back(JsonHandler::GetElement<double>(nodeGeometry, "lat"));
				lons.push_back(JsonHandler::GetElement<double>(nodeGeometry, "lon"));
			}

			lat = std::accumulate(lats.begin(), lats.end(), 0.0) / lats.size();
			lon = std::accumulate(lons.begin(), lons.end(), 0.0) / lons.size();
		}
		else
		{
			JsonHandler::HasElement(element, "lat") ? lat = JsonHandler::GetElement<double>(element, "lat") : lat = 0;
			JsonHandler::HasElement(element, "lon") ? lon = JsonHandler::GetElement<double>(element, "lon") : lon = 0;
		}

		priority = CalculatePoiPriority(tags);

		double desiredTimeInSeconds = (searchRadius / DataBuildingConstants::AVERAGE_KMH_WALKING_SPEED) * DataBuildingConstants::HOURS_TO_SECONDS;

		if (IsPoiInTime(start, end, { lat, lon }, desiredTimeInSeconds, timeCalculator) && IsPoiHighPriority(priority))
		{
			output << std::fixed << std::setprecision(7) << lat << " " << lon << " " << priority << std::endl;

			poiCount++;

			coordinates.emplace_back(lat, lon);
		}

		FreeMemory(tags, element);
	}
	coordinates.push_back(end);

	output << std::fixed << std::setprecision(7) << endLat << " " << endLon << " " << 0 << std::endl;

	std::cout << "Total number of filtered POIS found in a circle area of " << searchRadius << "KMs is : " << poiCount << std::endl;

	auto [timeMatrix, totalTime] = timeCalculator->GetTimeInfo(coordinates);

	SaveTimeInfo(timeMatrix, totalTime, output);

	output.close();
}

size_t OSMDataBuilder::WriteCallback(void* contents, size_t size, size_t nmemb, std::string* data)
{
	size_t newLength = size * nmemb;
	try
	{
		data->append((char*)contents, newLength);
	}
	catch (std::bad_alloc& e)
	{
		std::cerr << e.what();
	}
	return newLength;
}

std::string OSMDataBuilder::FetchPost(const std::string& query, const std::string& url)
{
	CURL* curl;
	CURLcode res;
	std::string readBuffer;

	curl = curl_easy_init();
	if (curl)
	{
		curl_easy_setopt(curl, CURLOPT_URL, url.c_str());

		curl_easy_setopt(curl, CURLOPT_POST, 1L);

		curl_easy_setopt(curl, CURLOPT_POSTFIELDS, query.c_str());

		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, &WriteCallback);
		curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);

		res = curl_easy_perform(curl);
		if (res != CURLE_OK)
		{
			std::cerr << "curl_easy_perform() failed: " << curl_easy_strerror(res) << std::endl;
		}
		curl_easy_cleanup(curl);
	}

	return readBuffer;
}

std::string OSMDataBuilder::CreateQuery(double searchRadius, double latitude, double longitude)
{
	std::ostringstream query;
	query << "[out:json];(";

	auto addQueries = [&](const PoiTypes& tags, const std::string& type)
		{
			std::string aroundClause = "around:" + std::to_string(static_cast<int>(searchRadius * 1000)) + "," + std::to_string(latitude) + "," + std::to_string(longitude);
			for (const auto& tag : tags)
			{
				query << "node(" << aroundClause << ")[" << type << "=" << tag << "];";
				query << "way(" << aroundClause << ")[" << type << "=" << tag << "];";
			}
		};

	addQueries(m_amenities, "amenity");
	addQueries(m_leisure, "leisure");
	addQueries(m_tourism, "tourism");
	addQueries(m_historic, "historic");

	query << ");";
	query << "out body geom;";

	return query.str();
}


bool OSMDataBuilder::IsPoiHighPriority(int priority)
{
	if (priority >= DataBuildingConstants::LOWEST_PRIORITY)
		return true;
	return false;
}

bool OSMDataBuilder::IsPoiInTime(const Coordinate& start, const Coordinate& end, const Coordinate& poi, double desiredTime, const ITimeMatrixCalculatorPtr timeCalculator)
{
	auto toPoiTime = timeCalculator->GetTimeBetweenPoints(start, poi);
	auto fromPoiTime = timeCalculator->GetTimeBetweenPoints(poi, end);

	if (toPoiTime + fromPoiTime < desiredTime)
		return true;

	return false;
}

void OSMDataBuilder::SaveTimeInfo(const TimeMatrix& timeMatrix, double totalTime, std::ofstream& output)
{
	output << "#time matrix" << std::endl;

	for (const auto& row : timeMatrix)
	{
		for (const auto& value : row)
		{
			output << std::fixed << std::setprecision(2) << value << " ";
		}
		output << std::endl;
	}

	output << "#total time of matrix" << std::endl;
	output << std::fixed << std::setprecision(7) << totalTime << std::endl;
}

void OSMDataBuilder::FreeMemory(Tags& tags, char* element)
{
	for (auto& tag : tags)
	{
		delete[] tag.first;
		delete[] tag.second;
	}

	delete[] element;
}

int OSMDataBuilder::CalculatePoiPriority(const Tags& tags)
{
	int priority = 0;

	for (const auto& tag : tags)
	{

		if (strcmp(tag.first, "historic") == 0 || strcmp(tag.first, "tourism") == 0)
		{
			priority += 3;
		}

		else if (strcmp(tag.first, "wikipedia") == 0 || strcmp(tag.first, "wikidata") == 0)
		{
			priority += 3;
		}

		else if (strcmp(tag.first, "leisure") == 0)
		{
			if (strcmp(tag.second, "park") == 0)
				priority += 3;
			else
				priority += 2;
		}

		else if (strcmp(tag.first, "amenity") == 0)
		{
			if (strcmp(tag.second, "place_of_worship") == 0 ||
				strcmp(tag.second, "clock") == 0 ||
				strcmp(tag.second, "street_art") == 0)
			{
				priority += 2;
			}
		}

		else if (strcmp(tag.first, "website") == 0 || strcmp(tag.first, "url") == 0)
		{
			priority += 1;
		}

		else if (strncmp(tag.first, "name", 4) == 0)
		{
			priority += 1;
		}
	}

	return priority;
}
