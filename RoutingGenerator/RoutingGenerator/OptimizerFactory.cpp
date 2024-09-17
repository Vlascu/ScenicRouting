#include "OptimizerFactory.h"
#include "GAOptimizer.h"
#include "GATSOptimizer.h"
#include "GAGOptimizer.h"
#include "ACOptimizer.h"


OptimizerPtr OptimizerFactory::Produce(EOptimizationAlgorithms algorithm)
{
	switch (algorithm)
	{
	case EOptimizationAlgorithms::GA:
		return std::make_shared<GAOptimizer>();
	case EOptimizationAlgorithms::GATS:
		return std::make_shared<GATSOptimizer>();
	case EOptimizationAlgorithms::GAG:
		return std::make_shared<GAGOptimizer>();
	case EOptimizationAlgorithms::AC:
		return std::make_shared<ACOptimizer>();
	default:
		return nullptr;
	}
}
