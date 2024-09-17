#pragma once
#include "GAOptimizer.h"

using TabuList = std::vector<PoisIdx>;

class GATSOptimizer : public GAOptimizer
{
public:
	GATSOptimizer();

	void AdditionalOptimization(PoisIdx& route1, PoisIdx& route2, const InputDataPtr data, double desiredTime) override;
private:
	void TabuSearch(PoisIdx& route, const InputDataPtr data, double desiredTime, TabuList& tabuList, int maxTabuSize);
private:
	int m_populationSize;
	int m_maxGenerations;
	double m_mutationRate;
	int m_tournamentSelection;

	TabuList m_tabuList;
	int m_maxTabuSize;
};

