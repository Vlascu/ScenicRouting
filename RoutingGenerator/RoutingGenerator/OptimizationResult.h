#pragma once
#include "Chromosome.h"

#include <fstream>

class OptimizationResult
{
public:
	OptimizationResult();

	double GetFitness() const;
	void SetFitness(double fitness);

	void SetSolution(const PoisIdx& solution);
	
	double GetTime() const;

	int GetNumberOfPois() const;

	int GetPriority() const;

	CoordinateList GetCoordinates() const;

	void BuildData(const Coordinate& startPoint, const Coordinate& endPoint, const InputDataPtr data);

	void GenerateKML(const std::string& filename);
private:
	double m_fitness;
	double m_time;

	int m_numberOfPois;
	int m_priority;

	CoordinateList m_coordinates;

	PoisIdx m_bestSolution;
};

