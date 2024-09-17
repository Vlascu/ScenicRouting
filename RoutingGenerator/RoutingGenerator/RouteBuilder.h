#pragma once
#include "IRouteBuilder.h"
#include "EOptimizationAlgorithms.h"
#include "OptimizerFactory.h"

#include <vector>
#include <unordered_map>
#include <random>
#include <fstream>

class RouteBuilder : public IRouteBuilder
{
public:
	RouteBuilder();

	void BuildRoute(int numberOfIterations, const InputDataPtr data, const Coordinate& start, const Coordinate& end, double desiredTime, EOptimizationAlgorithms algorithm) override;
};

