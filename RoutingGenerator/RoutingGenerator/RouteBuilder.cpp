#include "RouteBuilder.h"

#include <ranges>
#include <algorithm>
#include <iomanip>
#include <iostream>
#include <format>
#include <chrono>

RouteBuilder::RouteBuilder()
{
}

void RouteBuilder::BuildRoute(int numberOfIterations, const InputDataPtr data, const Coordinate& start, const Coordinate& end, double desiredTime, EOptimizationAlgorithms algorithm)
{
	double desiredTimeInSeconds = desiredTime * ScenicRoutingConstants::GENERAL_TIME_CONVERSION;

	if (data->GetTravelTimeToEnd(0) > desiredTimeInSeconds / ScenicRoutingConstants::START_END_LIMIT_FACTOR)
	{
		auto msg = std::format("Start and end are too far apart for the walking time of {:.2f} minutes. Choose other points.", desiredTime);

		throw std::invalid_argument(msg);
	}

	auto optimizer = OptimizerFactory::Produce(algorithm);
	optimizer->SetInputData(data);
	
	OptimizationResult result;

	auto startTime = std::chrono::high_resolution_clock::now();
	for (int i = 0; i < numberOfIterations; i++)
	{
		result = optimizer->GetOptimizedRoute(desiredTimeInSeconds, start, end);

		static int bestTotalScore = 0;
		static int bestNumberOfPois = 0;
		static double bestTotalTime = 0;

		double totalTime = result.GetTime();
		int totalPriority = result.GetPriority();

		if (totalPriority > bestTotalScore && totalTime <= desiredTime * ScenicRoutingConstants::GENERAL_TIME_CONVERSION)
		{
			bestTotalScore = totalPriority;
			bestNumberOfPois = result.GetNumberOfPois();
			bestTotalTime = totalTime;

			std::cout << "Best time: " << bestTotalTime / ScenicRoutingConstants::GENERAL_TIME_CONVERSION << std::endl;
			std::cout << "Best priority: " << bestTotalScore << std::endl;
			std::cout << "Best number of POIs: " << bestNumberOfPois << std::endl;
			std::cout << "Best fitness: " << result.GetFitness() << std::endl;
			std::cout << "-------------------------" << std::endl;

			result.GenerateKML("../Output/route.kml");
		}

	}
	auto endTime = std::chrono::high_resolution_clock::now();

	std::chrono::duration<double> elapsed = endTime - startTime;
	std::cout << "Execution time: " << elapsed.count() << " seconds\n";
}

