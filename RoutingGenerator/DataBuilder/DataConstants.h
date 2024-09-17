#pragma once

namespace DataBuildingConstants
{
	//data getting
	inline const double AVERAGE_KMH_WALKING_SPEED = 4.0;
	inline const double AVERAGE_MS_WALKING_SPEED = 1.1;
	inline const double SEARCH_RADIUS_REDUCTION_FACTOR = 3;
	inline const int LOWEST_PRIORITY = 4;
	inline const int POIS_LIMIT = 100;
	inline const double START_END_LIMIT_FACTOR = 1.4;

	//time manipulation
	inline const int HOURS_TO_SECONDS = 3600;
	inline const int GENERAL_TIME_CONVERSION = 60;

	//geometry calculations
	inline const double R = 6371e3;
	inline const double PI = 3.14159265358979323846;
}