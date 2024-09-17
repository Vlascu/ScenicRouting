#pragma once
#include "IDataBuilder.h"
#include "DataConstants.h"

#include <iostream>
#include <vector>
#include <sstream>
#include <unordered_map>
#include <fstream>

#include <curl/curl.h>


class OSMDataBuilder : public IDataBuilder
{

public:
	OSMDataBuilder();

	//IDataBuilder
	void BuildData(const Coordinate& start, const Coordinate& end, double searchRadius, const ITimeMatrixCalculatorPtr timeCalculator, const std::string& path) override;

private:
	int CalculatePoiPriority(const Tags& tags);
	
	static size_t WriteCallback(void* contents, size_t size, size_t nmemb, std::string* data);

	std::string FetchPost(const std::string& query, const std::string& url);

	std::string CreateQuery(double searchRadius, double latitude, double longitude);

	bool IsPoiHighPriority(int priority);

	bool IsPoiInTime(const Coordinate& start, const Coordinate& end, const Coordinate& poi, double desiredTime, const ITimeMatrixCalculatorPtr timeCalculator);
	
	void SaveTimeInfo(const TimeMatrix& timeMatrix, double totalTime, std::ofstream& output);

	void FreeMemory(Tags& tags, char* element);

private:
	PoiTypes m_amenities;
	PoiTypes m_leisure;
	PoiTypes m_tourism;
	PoiTypes m_historic;
};
