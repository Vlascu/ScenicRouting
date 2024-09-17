#pragma once
#include "Optimizer.h"
#include "Random.h"
#include "FitnessCalculator.h"

#include <random>
#include <chrono>
#include <set>
#include <unordered_set>
#include <unordered_map>

struct POIData;

using ChromosomePair = std::pair<Chromosome, Chromosome>;
using Parents = std::pair<PoisIdx, PoisIdx>;

class GAOptimizer : public Optimizer
{
public:
	GAOptimizer();

protected:
	OptimizationResult GetOptimizedRoute(double desiredTime, const Coordinate& startPoint, const Coordinate& endPoint) override;

	ChromosomeList InitPopulation(double desiredTime);
	PoisIdx GenerateChromosome(int poisSize);

	void RemoveNodes(Chromosome& chromosome, double desiredTime, const TimeMatrix& timeMatrix);

	ChromosomePair Select(const ChromosomeList& population);
	
	Parents OrderedCrossover(const PoisIdx& parent1, const PoisIdx& parent2);

	void Mutation(PoisIdx& route);
	
	double CalculateTotalTime(const PoisIdx& route);

	int GetRandomIdGene(const PoisIdx& ids);

	virtual void AdditionalOptimization(PoisIdx& route1, PoisIdx& route2, const InputDataPtr data, double desiredTime);
protected:
	int m_populationSize;
	int m_maxGenerations;
	double m_mutationRate;
	int m_tournamentSelection;
};

