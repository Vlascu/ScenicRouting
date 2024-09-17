#include "FileDataLoader.h"
#include "OSMPoi.h"
#include "OSMInputData.h"
#include "Exceptions.h"

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <memory>

InputDataPtr FileDataLoader::LoadData(const char* data)
{
	std::ifstream input(data);

	if (!input.is_open())
	{
		throw FileException(("[FILE EXCEPTION]: Couldn't load data from " + std::string(data)).c_str());
	}
	
	POIDeque pois;

	std::string line;

	auto ReadRow = [&](std::vector<std::vector<double>>& values, std::string& line)
	{
		if (line.empty())
		{
			return false;
		}

		std::istringstream iss(line);
		double value;
		std::vector<double> row;

		while (iss >> value)
		{
			row.push_back(value);
		}

		if (row.empty())
		{
			return false;
		}

		values.push_back(row);

		return true;
	};

	auto ReadValues = [&](std::vector<std::vector<double>>& values)
	{
		static bool data_section_started = false;
		values.clear();

		while (std::getline(input, line)) 
		{
			char nextChar = input.peek();

			if (!input || nextChar == EOF)
			{
				break;
			}

			if (nextChar == '#')
			{
				if (data_section_started)
				{
					ReadRow(values, line);

					break;
				}

				std::getline(input, line);
				data_section_started = true;
				continue;
			}
			else if (line[0] == '#')
			{
				data_section_started = true;
				std::getline(input, line);
			}

			if (data_section_started)
			{
				if (!ReadRow(values, line))
					break;
			}
		}
	};

	std::vector<std::vector<double>> values;

	//pois
	ReadValues(values);

	for (const auto& row : values)
	{
		Coordinate coordinate(row[0], row[1]);
		int priority = static_cast<int>(row[2]);

		PoiPtr poi = std::make_shared<OSMPoi>(coordinate, priority);

		pois.push_back(poi);
	}
	
	//time matrix
	ReadValues(values);
	
	TimeMatrix timeMatrix;

	std::vector<double> timeRow;

	for (const auto& row : values)
	{
		timeRow.clear();
		
		for (const auto& value : row)
		{
			timeRow.push_back(value);
		}

		timeMatrix.push_back(timeRow);

	}

	//total time
	ReadValues(values);

	double totalTime = values[0][0];

	TimeInfo timeInfo(timeMatrix, totalTime);

	return std::make_shared<OSMInputData>(timeInfo, pois);
}
