#include "GATSOptimizer.h"

GATSOptimizer::GATSOptimizer()
	: m_tabuList()
	, m_maxTabuSize(ScenicRoutingConstants::TABU_LIST_MAX_SIZE)
	, m_populationSize(ScenicRoutingConstants::INITIAL_POPULATION_SIZE)
	, m_maxGenerations(ScenicRoutingConstants::STOP_AFTER_ITERATIONS)
	, m_mutationRate(ScenicRoutingConstants::MUTATION_RATE)
	, m_tournamentSelection(ScenicRoutingConstants::ELITISM_SELECTION)
{
}

void GATSOptimizer::AdditionalOptimization(PoisIdx& route1, PoisIdx& route2, const InputDataPtr data, double desiredTime)
{
	TabuSearch(route1, data, desiredTime, m_tabuList, m_maxTabuSize);
	TabuSearch(route2, data, desiredTime, m_tabuList, m_maxTabuSize);
}

void GATSOptimizer::TabuSearch(PoisIdx& route, const InputDataPtr data, double desiredTime, TabuList& tabuList, int maxTabuSize)
{
	auto bestSolution = route;
	auto bestSolutionFitness = std::numeric_limits<double>::max();

	int noImprovementCounter = 0;

	PoisIdx neighbor = route;

	while (true)
	{
		std::vector<PoisIdx> neighborhood;
		
		double bestNeighbourFitness = std::numeric_limits<double>::max();
		PoisIdx bestNeighbour;

		for (size_t i = 0; i < route.size() - 1; ++i)
		{
			std::swap(neighbor[i], neighbor[i + 1]);

			if (find(tabuList.begin(), tabuList.end(), neighbor) == tabuList.end())
			{
				neighborhood.push_back(neighbor);

				auto [neighborFitness, totalTime, totalPriority] = FitnessCalculator::EvaluateRoute(neighbor, data, desiredTime);

				if (neighborFitness < bestNeighbourFitness)
				{
					bestNeighbourFitness = neighborFitness;
					bestNeighbour = neighbor;
				}
			}

			std::swap(neighbor[i], neighbor[i + 1]);
		}

		if (neighborhood.empty()) break;

		if (bestNeighbourFitness < bestSolutionFitness)
		{
			bestSolution = bestNeighbour;
			bestSolutionFitness = bestNeighbourFitness;

			tabuList.push_back(bestNeighbour);
			if (tabuList.size() > maxTabuSize)
			{
				tabuList.erase(tabuList.begin());
			}
			noImprovementCounter = 0;
		}
		else
		{
			noImprovementCounter++;
		}

		if (noImprovementCounter >= ScenicRoutingConstants::TABU_STOP_AFTER)
		{
			break;
		}
	}

	route = bestSolution;
}
