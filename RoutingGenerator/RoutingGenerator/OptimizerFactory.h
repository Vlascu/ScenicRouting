#pragma once
#include "EOptimizationAlgorithms.h"
#include "IOptimizer.h"

struct POIData;

class OptimizerFactory
{
public:
	static OptimizerPtr Produce(EOptimizationAlgorithms algorithm);
};