#include "Chromosome.h"
#include "Constants.h"
#include "Random.h"

#include <ranges>
#include <algorithm>
#include <unordered_set>
#include <iostream>


Chromosome::Chromosome()
	: m_chromosomeMembers()
	, m_fitness(0.0)
	, m_maxRoutingTime(0.0)
	, m_totalTime(0.0)
	, m_totalPriority(0)
{
}

Chromosome::Chromosome(const PoisIdx& chromosomeMembers, const InputDataPtr data, double desiredTime)
	: m_chromosomeMembers(chromosomeMembers)
	, m_totalTime(0)
	, m_maxRoutingTime(data->GetMaxTime())
	, m_totalPriority(0)
{
	auto [fitness, totalTime, totalPriority] = FitnessCalculator::EvaluateRoute(m_chromosomeMembers, data, desiredTime);

	m_fitness = fitness;
	m_totalTime = totalTime;
	m_totalPriority = totalPriority;
}

double Chromosome::GetFitness() const
{
	return m_fitness;
}

double Chromosome::GetTotalTime() const
{
	return m_totalTime;
}

int Chromosome::GetNegativesCount() const
{
	return static_cast<int>(std::count(m_chromosomeMembers.begin(), m_chromosomeMembers.end(), -1));
}

int Chromosome::GetTotalPriority() const
{
	return m_totalPriority;
}

void Chromosome::SetChromosomeMembers(const PoisIdx& ids)
{
	this->m_chromosomeMembers = ids;
}

PoisIdx Chromosome::GetChromosomeMembers() const
{
	return m_chromosomeMembers;
}

void Chromosome::SetFitness(double fitness)
{
	this->m_fitness = fitness;
}

bool Chromosome::operator<(const Chromosome& other)
{
	return this->m_fitness < other.GetFitness();
}

int Chromosome::GetRandomIdGene(const PoisIdx& ids)
{
	int randomIdGene = GenerateRandomNumber<int>(0, static_cast<int>(ids.size() - 1));
	return ids[randomIdGene];
}

