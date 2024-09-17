#pragma once
#include "Constants.h"
#include "OptimizationResult.h"
#include "EOptimizationAlgorithms.h"
#include "IInputData.h"

#include <memory>

using OptimizerPtr = std::shared_ptr<class IOptimizer>;

class IOptimizer
{
public:
	virtual OptimizationResult GetOptimizedRoute(double desiredTime, const Coordinate& startPoint, const Coordinate& endPoint) = 0;
	virtual void SetInputData(const InputDataPtr inputData) = 0;

	virtual ~IOptimizer() = default;
};