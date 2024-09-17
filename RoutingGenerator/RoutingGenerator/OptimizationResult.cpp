#include "OptimizationResult.h"
#include "Exceptions.h"

#include <iomanip>

OptimizationResult::OptimizationResult()
	: m_coordinates()
	, m_fitness(0.0)
	, m_numberOfPois(0)
	, m_priority(0)
	, m_time(0.0)
{
	//empty
}

double OptimizationResult::GetFitness() const
{
	return m_fitness;
}

void OptimizationResult::SetFitness(double fitness)
{
	this->m_fitness = fitness;
}

double OptimizationResult::GetTime() const
{
	return m_time;
}

int OptimizationResult::GetNumberOfPois() const
{
	return m_numberOfPois;
}

int OptimizationResult::GetPriority() const
{
	return m_priority;
}

CoordinateList OptimizationResult::GetCoordinates() const
{
	return m_coordinates;
}

void OptimizationResult::SetSolution(const PoisIdx& solution)
{
	this->m_bestSolution = solution;
}

void OptimizationResult::BuildData(const Coordinate& startPoint, const Coordinate& endPoint, const InputDataPtr data)
{

	if (m_bestSolution.empty())
		throw OptimizationResultException("[RESULT EXCEPTION]: Solution is empty");

	if(data->IsTimeMatrixEmpty() || data->IsPoisEmpty())
		throw OptimizationResultException("[RESULT EXCEPTION]: Input data is empty");

	PoisIdx poisIndexes;

	for (auto& poiId : m_bestSolution)
	{
		if (poiId == -1)
			continue;

		poisIndexes.push_back(poiId);
	}

	m_coordinates.push_back(startPoint);

	for (const auto& index : poisIndexes)
	{
		m_coordinates.push_back(data->GetPoiCoordinate(index));
	}

	m_coordinates.push_back(endPoint);

	m_numberOfPois = static_cast<int>(poisIndexes.size());

	if (poisIndexes.size() > 0)
	{
		m_time += data->GetTravelTime(0, poisIndexes[0] + 1);

		for (size_t i = 1; i < poisIndexes.size(); i++)
		{
			auto time = data->GetTravelTime(poisIndexes[i - 1] + 1, poisIndexes[i] + 1);

			m_time += time;
		}

		m_time += data->GetTravelTimeToEnd(poisIndexes.back() + 1);

		for (const auto& idx : poisIndexes)
		{
			m_priority += data->GetPoiPriority(idx);
		}
	}
}

void OptimizationResult::GenerateKML(const std::string& filename)
{
	std::ofstream kmlFile(filename);

	if (!kmlFile.is_open())
		throw FileException(("[FILE EXCEPTION]: Couldn't open file for saving KML at " + std::string(filename)).c_str());

	kmlFile << "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n";
	kmlFile << "<kml xmlns=\"http://www.opengis.net/kml/2.2\">\n";
	kmlFile << "  <Document>\n";
	kmlFile << "    <name>Route</name>\n";

	kmlFile << "    <!-- Define a style for the line and pins -->\n";
	kmlFile << "    <Style id=\"lineStyle\">\n";
	kmlFile << "      <LineStyle>\n";
	kmlFile << "        <color>ff0000ff</color> <!-- Blue line -->\n";
	kmlFile << "        <width>4</width> <!-- Thicker line -->\n";
	kmlFile << "      </LineStyle>\n";
	kmlFile << "      <IconStyle>\n";
	kmlFile << "        <Icon>\n";
	kmlFile << "          <href>http://maps.google.com/mapfiles/kml/pushpin/ylw-pushpin.png</href>\n";
	kmlFile << "        </Icon>\n";
	kmlFile << "        <scale>1.1</scale> <!-- Slightly larger pins -->\n";
	kmlFile << "      </IconStyle>\n";
	kmlFile << "      <LabelStyle>\n";
	kmlFile << "        <scale>1.1</scale> <!-- Slightly larger labels -->\n";
	kmlFile << "      </LabelStyle>\n";
	kmlFile << "    </Style>\n";

	kmlFile << "    <Placemark>\n";
	kmlFile << "      <name>Route Line</name>\n";
	kmlFile << "      <styleUrl>#lineStyle</styleUrl>\n";
	kmlFile << "      <LineString>\n";
	kmlFile << "        <coordinates>\n";

	for (const auto& coord : m_coordinates)
	{
		kmlFile << "          " << std::fixed << std::setprecision(6)
			<< coord.second << "," // latitude
			<< coord.first << ","  // longitude
			<< "1000" << "\n";        // altitude (0)
	}

	kmlFile << "        </coordinates>\n";
	kmlFile << "      </LineString>\n";
	kmlFile << "    </Placemark>\n";

	int index = 1;
	for (const auto& coord : m_coordinates)
	{
		kmlFile << "    <Placemark>\n";
		kmlFile << "      <name>Point " << index << "</name>\n"; 
		kmlFile << "      <styleUrl>#lineStyle</styleUrl>\n"; 
		kmlFile << "      <Point>\n";
		kmlFile << "        <coordinates>"
			<< coord.second << "," // latitude
			<< coord.first << ","  // longitude
			<< "1000" << "</coordinates>\n"; // altitude (0)
		kmlFile << "      </Point>\n";
		kmlFile << "    </Placemark>\n";
		++index;
	}

	kmlFile << "  </Document>\n";
	kmlFile << "</kml>\n";

	kmlFile.close();
}
