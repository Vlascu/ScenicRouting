#pragma once

namespace ScenicRoutingConstants
{
	//data getter
	inline const double AVERAGE_KMH_WALKING_SPEED = 4.0;
	inline const double AVERAGE_MS_WALKING_SPEED = 1.1;
	inline const double SEARCH_RADIUS_REDUCTION_FACTOR = 3;
	inline const int LOWEST_PRIORITY = 4;
	inline const int POIS_LIMIT = 100;
	inline const double START_END_LIMIT_FACTOR = 1.4;

	//genetic algorithm fitness calculation
	inline const double MIN_PRIORITY = 0.0;
	inline const double MIN_TIME = 0.0;
	inline const double MAX_PRIORITY = 70.0;
	inline const int MAX_EXPONENT = 700;
	inline const double MAX_UNDER_TIME = 600;

	inline const int NEGATIVES_WEIGHT = 1000;
	inline const double PRIORITY_WEIGHT = 500;
	inline const int TIME_WEIGHT = 10000;
	inline const int UNDER_TIME_BONUS = 5000;

	//general genetic algorithm
	inline const int INITIAL_POPULATION_SIZE = 200;
	inline const int STOP_AFTER_ITERATIONS = 50;
	inline const int ELITISM_SELECTION = 10;
	inline const double MUTATION_RATE = 0.1;
	inline const int REMOVED_NODE_VALUE = -1;
	inline const int DEFAULT_NODE_VALUE = -2;

	//ga + tabu search
	inline const int TABU_STOP_AFTER = 10;
	inline const int TABU_LIST_MAX_SIZE = 10;

	//geometry calculations
	inline const double R = 6371e3;
	inline const double PI = 3.14159265358979323846;

	//aco optimization
	inline const int ANT_COUNT = 150;
	inline const double PHEROMONE_ALPHA = 4;
	inline const double HEURISTIC_BETA = 10;
	inline const double DIRECTION_BONUS = 1.5;
	inline const double PRIORITY_OFFSET = 2;
	inline const double EVAPORATION_RATE = 0.2;

	//time manipulation
	inline const int HOURS_TO_SECONDS = 3600;
	inline const int GENERAL_TIME_CONVERSION = 60;

}
