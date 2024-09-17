#pragma once
#include "Aliases.h"
#include "OptimizationResult.h"
#include "EOptimizationAlgorithms.h"
#include "IInputData.h"

#include <memory>
#include <vector>
#include <string>

using RouteBuilderPtr = std::shared_ptr<class IRouteBuilder>;

class IRouteBuilder
{
public:
	virtual void BuildRoute(int numberOfIterations, const InputDataPtr data, const Coordinate& start, const Coordinate& end, double desiredTime, EOptimizationAlgorithms algorithm) = 0;

	virtual ~IRouteBuilder() = default;
};