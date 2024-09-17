#pragma once
#include "FitnessCalculator.h"
#include "IInputData.h"

using ChromosomeList = std::vector<class Chromosome>;

class Chromosome
{
public:
	Chromosome();
	Chromosome(const PoisIdx& chromosomeMembers, const InputDataPtr data, double desiredTime);

	void SetFitness(double fitness);
	double GetFitness() const;

	double GetTotalTime() const;
	int GetTotalPriority() const;
	
	int GetNegativesCount() const;

	void SetChromosomeMembers(const PoisIdx& ids);
	PoisIdx GetChromosomeMembers() const;

	bool operator<(const Chromosome& other);
private:
	int GetRandomIdGene(const PoisIdx& populationIds);
private:
	PoisIdx m_chromosomeMembers;
	double m_fitness;
	double m_totalTime;
	double m_maxRoutingTime;
	int m_totalPriority;
};

