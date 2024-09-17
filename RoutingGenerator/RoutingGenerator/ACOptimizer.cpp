#include "ACOptimizer.h"
#include "Random.h"

using Ants = std::vector<Ant>;

ACOptimizer::ACOptimizer()
{

}

OptimizationResult ACOptimizer::GetOptimizedRoute(double desiredTime, const Coordinate& startPoint, const Coordinate& endPoint)
{
	PheromoneMatrix pheromones = InitializePheromoneMatrix();
	ScoreMatrix scores = InitializeScoresMatrix();

	PoisIdx bestSolution;
	double bestFitness = std::numeric_limits<double>::max();
	int noImprovementCounter = 0;

	int poiSize = m_input->GetPoisSize();

	Ant ant(poiSize);
	while (true)
	{
		for (int i = 0; i < ScenicRoutingConstants::ANT_COUNT; ++i)
		{
			ant.Reset();

			int firstPOI = SelectNextPOI(ant, pheromones, scores);

			double startToFirstPOITime = m_input->GetTravelTime(0, firstPOI + 1);
			ant.AddTravelTime(startToFirstPOITime);

			ant.VisitPOI(firstPOI, startToFirstPOITime);

			while (ant.CanContinue(desiredTime))
			{
				int nextPOI = SelectNextPOI(ant, pheromones, scores);
				int lastVisitedPOI = ant.GetLastVisitedPOI();

				double travelTime;
				if (lastVisitedPOI == -1)
				{
					travelTime = m_input->GetTravelTime(0, nextPOI + 1);
				}
				else
				{
					travelTime = m_input->GetTravelTime(lastVisitedPOI + 1, nextPOI + 1);
				}

				double projectedTotalTime = ant.GetTotalTime() + travelTime;

				double travelTimeToEndPoint = m_input->GetTravelTimeToEnd(nextPOI + 1);
				if (projectedTotalTime + travelTimeToEndPoint > desiredTime)
				{
					break;
				}

				ant.VisitPOI(nextPOI, travelTime);
			}

			double lastPOIToEndPointTime = m_input->GetTravelTimeToEnd(ant.GetLastVisitedPOI() + 1);
			ant.AddTravelTime(lastPOIToEndPointTime);

			auto [fitness, totalTime, totalPriority] = FitnessCalculator::EvaluateAntColonyRoute(ant.GetRoute(), m_input, desiredTime, ant.GetTotalTime());

			UpdatePheromones(pheromones, ant.GetRoute(), fitness);

			if (fitness < bestFitness)
			{
				bestFitness = fitness;
				bestSolution = ant.GetRoute();
				noImprovementCounter = 0;

				//PrintBestSolution(bestSolution, ant, totalPriority);
			}
			else
			{
				noImprovementCounter++;
			}

		}

		if (noImprovementCounter >= ScenicRoutingConstants::STOP_AFTER_ITERATIONS)
		{
			break;
		}
	}

	OptimizationResult result;

	result.SetFitness(bestFitness);
	result.SetSolution(bestSolution);
	result.BuildData(startPoint, endPoint, m_input);

	return result;
}

ScoreMatrix ACOptimizer::InitializeScoresMatrix()
{
	int poiCount = m_input->GetPoisSize();
	ScoreMatrix heuristicMatrix(poiCount + 1, std::vector<double>(poiCount + 1, 0.0));

	double minPriority = ScenicRoutingConstants::MIN_PRIORITY;
	double maxPriority = ScenicRoutingConstants::MAX_PRIORITY;

	double timeHeuristic = 0.0;

	for (int i = 0; i <= poiCount; ++i) {
		for (int j = 0; j <= poiCount; ++j) {
			if (i != j) {
				double timeHeuristic = 0.0;
				double priorityI = 0.0;
				double priorityJ = 0.0;

				double travelTime = m_input->GetTravelTime(i, j);

				if (i == 0)
				{
					timeHeuristic = 1.0 / (travelTime + 1e-6);

					if (j > 0 && j < poiCount + 1)
					{
						priorityJ = (m_input->GetPoiPriority(j - 1) - minPriority) / (maxPriority - minPriority);
					}
				}
				else if (i == poiCount)
				{
					timeHeuristic = 1.0 / (travelTime + 1e-6);

					if (j > 0 && j < poiCount + 1)
					{

						priorityJ = (m_input->GetPoiPriority(j - 1) - minPriority) / (maxPriority - minPriority);
					}
				}
				else if (j == 0)
				{

					timeHeuristic = 1.0 / (travelTime + 1e-6);

					if (i > 0 && i < poiCount) {

						priorityI = (m_input->GetPoiPriority(i - 1) - minPriority) / (maxPriority - minPriority);
					}
				}
				else if (j == poiCount)
				{

					timeHeuristic = 1.0 / (travelTime + 1e-6);

					if (i > 0 && i < poiCount) {
						priorityI = (m_input->GetPoiPriority(i - 1) - minPriority) / (maxPriority - minPriority);
					}
				}
				else {
					timeHeuristic = 1.0 / (travelTime + 1e-6);
					if (i > 0 && i < poiCount + 1)
					{
						priorityI = (m_input->GetPoiPriority(i - 1) - minPriority) / (maxPriority - minPriority);
					}

					if (j > 0 && j < poiCount + 1)
					{

						priorityJ = (m_input->GetPoiPriority(j - 1) - minPriority) / (maxPriority - minPriority);
					}
				}

				double averagePriority = (priorityI + priorityJ) / 2.0;
				heuristicMatrix[i][j] = timeHeuristic * (1 + averagePriority);
			}
			else {
				heuristicMatrix[i][j] = 0.0;
			}
		}
	}

	return heuristicMatrix;
}

PheromoneMatrix ACOptimizer::InitializePheromoneMatrix()
{
	PheromoneMatrix pheromone(m_input->GetPoisSize(), std::vector<double>(m_input->GetPoisSize(), 1.0));
	return pheromone;
}

void ACOptimizer::UpdatePheromones(PheromoneMatrix& pheromone, const std::vector<int>& route, double fitness)
{
	double pheromoneUpdate = 0.0;

	fitness > 0 ? pheromoneUpdate = fitness : pheromoneUpdate = -fitness;

	for (size_t i = 0; i < route.size() - 1; ++i)
	{
		int from = route[i];
		int to = route[i + 1];

		pheromone[from][to] *= (1 - ScenicRoutingConstants::EVAPORATION_RATE);
		pheromone[from][to] += pheromoneUpdate;
	}
}


int ACOptimizer::SelectNextPOI(const Ant& ant, const PheromoneMatrix& pheromone, const ScoreMatrix& heuristic)
{
	int currentPOI = ant.GetLastVisitedPOI();

	int poiCount = static_cast<int>(pheromone.size()) - 2;

	std::vector<double> probabilities(poiCount, 0.0);
	double totalProbability = 0.0;

	double currentDistanceToEnd = m_input->GetTravelTimeToEnd(currentPOI + 1);

	for (int i = 0; i < poiCount; ++i)
	{
		if (!ant.IsVisited(i))
		{
			double pheromoneLevel;
			double scoreValue;
			double priorityValue;

			double poiPriority = static_cast<double>(m_input->GetPoiPriority(i));

			if (currentPOI == -1)
			{
				pheromoneLevel = pheromone[0][i + 1];
				scoreValue = heuristic[0][i + 1];
			}
			else
			{
				pheromoneLevel = pheromone[currentPOI + 1][i + 1];
				scoreValue = heuristic[currentPOI + 1][i + 1];
			}

			priorityValue = poiPriority / ScenicRoutingConstants::MAX_PRIORITY;

			scoreValue = scoreValue * (ScenicRoutingConstants::PRIORITY_OFFSET + priorityValue);

			double nextDistanceToEnd = m_input->GetTravelTimeToEnd(i + 1);

			if (nextDistanceToEnd <= currentDistanceToEnd)
			{
				scoreValue *= ScenicRoutingConstants::DIRECTION_BONUS;
			}

			double probability = std::pow(pheromoneLevel, ScenicRoutingConstants::PHEROMONE_ALPHA) *
				std::pow(scoreValue, ScenicRoutingConstants::HEURISTIC_BETA);

			if (std::isnan(probability))
				probability = 0.0;

			probabilities[i] = probability;

			totalProbability += probabilities[i];
		}
		else {
			probabilities[i] = 0.0;
		}
	}

	if (totalProbability == 0.0)
	{
		return -1;
	}

	for (int i = 0; i < poiCount; ++i)
	{
		probabilities[i] /= totalProbability;
	}

	double rand = GenerateRandomNumber<double>(0.0, 1.0);
	double cumulativeProbability = 0.0;

	for (int i = 0; i < poiCount; ++i)
	{
		cumulativeProbability += probabilities[i];
		if (rand < cumulativeProbability)
		{
			return i;
		}
	}

	return currentPOI;
}

void ACOptimizer::PrintBestSolution(PoisIdx bestSolution, Ant& ant, int totalPriority)
{
	std::cout << "---------Best----------" << std::endl;

	for (const auto& id : bestSolution)
	{
		std::cout << id << " ";
	}
	std::cout << "t: " << ant.GetTotalTime() << "p: " << totalPriority;
	std::cout << "\n-----------------------" << std::endl;
}
