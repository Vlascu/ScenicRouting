#pragma once
#include "IOptimizer.h"

struct POIData;

class Optimizer : public IOptimizer
{
public:
	Optimizer() = default;

	void SetInputData(const InputDataPtr inputData) override;
protected:
	virtual OptimizationResult GetOptimizedRoute(double desiredTime, const Coordinate& startPoint, const Coordinate& endPoint);
protected:
	InputDataPtr m_input;
};

