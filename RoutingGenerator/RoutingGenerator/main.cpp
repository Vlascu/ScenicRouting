#include "RouteBuilder.h"
#include "GATSOptimizer.h"
#include "EOptimizationAlgorithms.h"
#include "FileDataLoader.h"
#include "Exceptions.h"

#include <memory>
#include <chrono>

const double DESIRED_TIME_MINS = 50;
const int ITERATIONS = 20;

int main()
{
	try
	{
		auto loader = std::make_shared<FileDataLoader>();

		auto data = loader->LoadData("../Output/brasov_saguna_consiliu_50.txt");

		auto [start, end] = data->GetStartEndCoordinates();

		auto routeBuilder = std::make_shared<RouteBuilder>();

		routeBuilder->BuildRoute(ITERATIONS, data, start, end, DESIRED_TIME_MINS, EOptimizationAlgorithms::AC);
	}
	catch (ScenicRoutingException& e)
	{
		std::cout << e.what() << std::endl;
	}
	
	return 0;
}