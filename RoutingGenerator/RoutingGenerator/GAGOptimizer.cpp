#include "GAGOptimizer.h"

GAGOptimizer::GAGOptimizer()
	: m_populationSize(ScenicRoutingConstants::INITIAL_POPULATION_SIZE)
	, m_maxGenerations(ScenicRoutingConstants::STOP_AFTER_ITERATIONS)
	, m_mutationRate(ScenicRoutingConstants::MUTATION_RATE)
	, m_tournamentSelection(ScenicRoutingConstants::ELITISM_SELECTION)
{

}

void GAGOptimizer::AdditionalOptimization(PoisIdx& route1, PoisIdx& route2, const InputDataPtr data, double desiredTime)
{
	Greedy(route1, data, desiredTime);
	Greedy(route2, data, desiredTime);
}

void GAGOptimizer::Greedy(PoisIdx& routeToOptimize, const InputDataPtr data, double desiredTime)
{
	PoisIdx route;
	VisitedNodes visited;

	route.reserve(routeToOptimize.size());

	double totalTime = 0;
	size_t currentIndex = 0;

	int removedNodeValue = ScenicRoutingConstants::REMOVED_NODE_VALUE;
	int defaultNodeValue = ScenicRoutingConstants::DEFAULT_NODE_VALUE;

	if (routeToOptimize[currentIndex] != removedNodeValue && routeToOptimize[currentIndex] != defaultNodeValue)
	{
		route.push_back(routeToOptimize[currentIndex]);
		visited[routeToOptimize[currentIndex]] = true;
	}

	while (true)
	{
		double minTime = std::numeric_limits<double>::max();
		size_t nextIndex = -1;

		for (size_t i = 0; i < routeToOptimize.size(); ++i)
		{
			if (routeToOptimize[i] == removedNodeValue || visited[routeToOptimize[i]] || routeToOptimize[i] == defaultNodeValue)
			{
				continue;
			}

			double timeToNext = m_input->GetTravelTime(routeToOptimize[currentIndex] + 1, routeToOptimize[i] + 1);

			if (timeToNext < minTime && totalTime + timeToNext <= desiredTime)
			{
				minTime = timeToNext;
				nextIndex = i;
			}
		}

		if (nextIndex != removedNodeValue)
		{
			route.push_back(routeToOptimize[nextIndex]);
			visited[routeToOptimize[nextIndex]] = true;

			totalTime += minTime;
			currentIndex = nextIndex;
		}
		else
		{
			break;
		}
	}

	for (size_t i = 0; i < route.size(); ++i)
	{
		if (routeToOptimize[i] == removedNodeValue || routeToOptimize[i] == defaultNodeValue)
		{
			route.insert(route.begin() + i, routeToOptimize[i]);
		}
	}

	while (route.size() < routeToOptimize.size())
	{
		route.push_back(removedNodeValue);
	}

	routeToOptimize = route;
}
