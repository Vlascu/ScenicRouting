#include "Optimizer.h"

void Optimizer::SetInputData(const InputDataPtr inputData)
{
	m_input = inputData;
}

OptimizationResult Optimizer::GetOptimizedRoute(double desiredTime, const Coordinate& startPoint, const Coordinate& endPoint)
{
	return OptimizationResult();
}

