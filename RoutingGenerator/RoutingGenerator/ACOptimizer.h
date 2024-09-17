#pragma once
#include "Optimizer.h"
#include "Ant.h"

struct POIData;

using PheromoneMatrix = std::vector<std::vector<double>>;
using ScoreMatrix = std::vector<std::vector<double>>;

class ACOptimizer : public Optimizer
{
public:
	ACOptimizer();
protected:
	OptimizationResult GetOptimizedRoute(double desiredTime, const Coordinate& startPoint, const Coordinate& endPoint) override;


private:
	ScoreMatrix InitializeScoresMatrix();
	PheromoneMatrix InitializePheromoneMatrix();

	void UpdatePheromones(PheromoneMatrix& pheromone, const std::vector<int>& route, double fitness);

	int SelectNextPOI(const Ant& ant, const PheromoneMatrix& pheromone, const ScoreMatrix& heuristic);

	void PrintBestSolution(PoisIdx bestSolution, Ant& ant, int totalPriority);
};

