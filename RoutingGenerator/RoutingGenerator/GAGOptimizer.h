#pragma once
#include "GAOptimizer.h"

using VisitedNodes = std::unordered_map<int, bool>;

class GAGOptimizer : public GAOptimizer
{
public:
	GAGOptimizer();

	void AdditionalOptimization(PoisIdx& route1, PoisIdx& route2, const InputDataPtr data, double desiredTime) override;
private:
	void Greedy(PoisIdx& routeToOptimize, const InputDataPtr data, double desiredTime);
private:
	int m_populationSize;
	int m_maxGenerations;
	int m_tournamentSelection;
	double m_mutationRate;
};

