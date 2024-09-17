#include "GAOptimizer.h"

GAOptimizer::GAOptimizer()
	: m_populationSize(ScenicRoutingConstants::INITIAL_POPULATION_SIZE)
	, m_maxGenerations(ScenicRoutingConstants::STOP_AFTER_ITERATIONS)
	, m_mutationRate(ScenicRoutingConstants::MUTATION_RATE)
	, m_tournamentSelection(ScenicRoutingConstants::ELITISM_SELECTION)
{
	//empty
}

OptimizationResult GAOptimizer::GetOptimizedRoute(double desiredTime, const Coordinate& startPoint, const Coordinate& endPoint)
{
	ChromosomeList population = InitPopulation(desiredTime);

	PoisIdx bestSolution;
	double bestFitness = std::numeric_limits<double>::max();

	int noImprovementCounter = 0;

	Chromosome bestChromosome;

	OptimizationResult result;

	while (true)
	{
		ChromosomeList newPopulation;

		sort(population.begin(), population.end());

		int selectionFactor = (m_tournamentSelection * m_populationSize) / 100;

		for (int i = 0; i < selectionFactor; i++)
		{
			RemoveNodes(population[i], desiredTime, m_input->GetTimeMatrix());

			newPopulation.push_back(population[i]);

			if (population[i].GetFitness() < bestFitness)
				bestChromosome = population[i];
		}

		for (int i = 0; i < m_populationSize / 2; ++i)
		{
			auto [parent1, parent2] = Select(population);
			auto [child1, child2] = OrderedCrossover(parent1.GetChromosomeMembers(), parent2.GetChromosomeMembers());

			AdditionalOptimization(child1, child2, m_input, desiredTime);

			Mutation(child1);
			Mutation(child2);

			Chromosome firstChromosome(child1, m_input, desiredTime);
			Chromosome secondChromosome(child2, m_input, desiredTime);

			RemoveNodes(firstChromosome, desiredTime, m_input->GetTimeMatrix());
			RemoveNodes(secondChromosome, desiredTime, m_input->GetTimeMatrix());

			newPopulation.push_back(firstChromosome);
			newPopulation.push_back(secondChromosome);

			if (firstChromosome.GetFitness() < bestFitness)
				bestChromosome = firstChromosome;
			if (secondChromosome.GetFitness() < bestFitness)
				bestChromosome = secondChromosome;
		}

		if (bestChromosome.GetFitness() < bestFitness)
		{
			bestSolution = bestChromosome.GetChromosomeMembers();
			bestFitness = bestChromosome.GetFitness();
			noImprovementCounter = 0;
		}
		else
		{
			noImprovementCounter++;
		}

		if (noImprovementCounter >= m_maxGenerations)
		{
			break;
		}

		population = newPopulation;
	}

	result.SetFitness(bestFitness);
	result.SetSolution(bestSolution);
	result.BuildData(startPoint, endPoint, m_input);

	return result;
}

ChromosomePair GAOptimizer::Select(const ChromosomeList& population)
{
	int idx1 = GenerateRandomNumber<int>(0, static_cast<int>(population.size()) - 1);
	int idx2 = GenerateRandomNumber<int>(0, static_cast<int>(population.size()) - 1);

	return { population[idx1], population[idx2] };
}

void GAOptimizer::RemoveNodes(Chromosome& chromosome, double desiredTime, const TimeMatrix& m_timeMatrix)
{
	auto ids = chromosome.GetChromosomeMembers();
	double maxTime = std::numeric_limits<double>::min();
	int expensiveNodeIdx = -1;

	for (size_t index = 0; index < ids.size(); index++)
	{
		auto id = ids[index];
		double segmentTime = 0.0;

		if (id == -1)
			continue;

		if (index == 0)
		{
			segmentTime = m_timeMatrix[0][id + 1];
		}
		else if (index > 0 && index < m_timeMatrix.size())
		{
			segmentTime = m_timeMatrix[ids[index - 1] + 1][id + 1];
		}
		else
		{
			segmentTime = m_timeMatrix[id + 1][m_timeMatrix.size() - 1];
		}

		if (segmentTime > maxTime)
		{
			maxTime = segmentTime;
			expensiveNodeIdx = id;
		}
	}

	if (expensiveNodeIdx != ScenicRoutingConstants::REMOVED_NODE_VALUE)
	{
		if (chromosome.GetTotalTime() > desiredTime)
		{
			auto it = std::find(ids.begin(), ids.end(), expensiveNodeIdx);

			if (it != ids.end())
			{
				*it = ScenicRoutingConstants::REMOVED_NODE_VALUE;
			}

			chromosome.SetChromosomeMembers(ids);
		}
	}

}

ChromosomeList GAOptimizer::InitPopulation(double desiredTime)
{
	ChromosomeList population;

	while (population.size() < m_populationSize)
	{
		auto popIds = GenerateChromosome(m_input->GetPoisSize() - 1);
		population.push_back(Chromosome(popIds, m_input, desiredTime));
	}

	return population;
}

PoisIdx GAOptimizer::GenerateChromosome(int poisSize)
{
	PoisIdx chromosome;
	for (int i = 0; i < poisSize; i++)
	{
		int idx = GenerateRandomNumber(0, poisSize);

		while(std::find(chromosome.begin(), chromosome.end(), idx) != chromosome.end())
			idx = GenerateRandomNumber(0, poisSize);

		chromosome.push_back(idx);
	}

	return chromosome;
}

Parents GAOptimizer::OrderedCrossover(const PoisIdx& parent1, const PoisIdx& parent2)
{
	int cut = GenerateRandomNumber<int>(0, static_cast<int>(parent1.size()) - 1);

	int defaultNodeValue = ScenicRoutingConstants::DEFAULT_NODE_VALUE;
	int removedNodeValue = ScenicRoutingConstants::REMOVED_NODE_VALUE;

	PoisIdx child1(parent1.size(), defaultNodeValue);
	PoisIdx child2(parent2.size(), defaultNodeValue);

	std::copy(parent1.begin(), parent1.begin() + cut + 1, child1.begin());
	std::copy(parent2.begin(), parent2.begin() + cut + 1, child2.begin());

	auto fillChildOX = [&](const PoisIdx& parent, PoisIdx& child, int cut)
	{
		size_t idx = (cut + 1) % parent.size();
		for (size_t i = 0; i < parent.size(); ++i)
		{
			int id = parent[(cut + 1 + i) % parent.size()];
			if (id == removedNodeValue || (id != defaultNodeValue && std::find(child.begin(), child.end(), id) == child.end()))
			{
				child[idx] = id;
				idx = (idx + 1) % parent.size();
			}
		}
	};
	fillChildOX(parent2, child1, cut);
	fillChildOX(parent1, child2, cut);

	return { child1, child2 };
}

void GAOptimizer::Mutation(PoisIdx& route)
{
	if (GenerateRandomNumber<double>(0.0, 1.0) < m_mutationRate)
	{
		int i = GenerateRandomNumber<int>(0, static_cast<int>(route.size()) - 1);
		int j = GenerateRandomNumber<int>(0, static_cast<int>(route.size()) - 1);

		if (i == j)
		{
			j = (j + 1) % route.size();
		}

		double currentTotalTime = CalculateTotalTime(route);

		std::swap(route[i], route[j]);

		double newTotalTime = CalculateTotalTime(route);

		if (newTotalTime >= currentTotalTime) 
		{
			std::swap(route[i], route[j]);
		}
	}
}

double GAOptimizer::CalculateTotalTime(const PoisIdx& route)
{
	double totalTime = 0;

	int defaultNodeValue = ScenicRoutingConstants::DEFAULT_NODE_VALUE;
	int removedNodeValue = ScenicRoutingConstants::REMOVED_NODE_VALUE;
	
	for (size_t k = 0; k < route.size() - 1; ++k) {
		if (route[k] >= 0 && route[k + 1] >= 0) 
		{
			if (route[k] == removedNodeValue || route[k] == defaultNodeValue)
				continue;

			totalTime += m_input->GetTravelTime(route[k] + 1, route[k + 1] + 1);
		}
	}
	
	int i = 0;
	while (i < route.size() - 1 && route[i] == removedNodeValue)
		i++;

	totalTime += m_input->GetTravelTime(0, route[i] + 1);

	size_t last = route.size() - 1;

	while (last > 0 && route[last] == -1)
		last--;

	totalTime += m_input->GetTravelTimeToEnd(route[last] + 1);

	return totalTime;
}

int GAOptimizer::GetRandomIdGene(const PoisIdx& ids)
{
	int randomIdGene = GenerateRandomNumber<int>(0, static_cast<int>(ids.size()) - 1);
	return ids[randomIdGene];
}

void GAOptimizer::AdditionalOptimization(PoisIdx& route1, PoisIdx& route2, const InputDataPtr data, double desiredTime)
{
	//empty default;
}

