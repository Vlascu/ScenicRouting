#include "FitnessCalculator.h"

RouteInfo FitnessCalculator::EvaluateRoute(const PoisIdx& route, const InputDataPtr data, double desiredTime)
{
	double fitness = 0.0;
	double totalTime = 0.0;
	int totalPriority = 0;

	double maxUnderTime = ScenicRoutingConstants::MAX_UNDER_TIME;

	auto routeSize = route.size();

	if (route.empty())
	{
		return { 0.0, 0.0, 0 };
	}

	for (size_t i = 0; i < routeSize; ++i)
	{
		auto poiId = route[i];

		if (poiId == -1)
			continue;

		auto[time, priority] = GetTimeAndPriorityInfo(data, route, poiId, i);

		totalTime += time;
		totalPriority += priority;
	}

	double normalizedTime = ((data->GetMaxTime() - (desiredTime - maxUnderTime)) != 0) ?
		(totalTime - (desiredTime - maxUnderTime)) / (data->GetMaxTime() - (desiredTime - maxUnderTime)) : 0;

	double timePenality = 0.0;
	if (totalTime > desiredTime)
	{
		double timeDifference;
		if (totalTime > desiredTime)
			timeDifference = totalTime - desiredTime;
		else
			timeDifference = desiredTime - totalTime;

		double normalizedTimeDifference = ((timeDifference - (desiredTime - maxUnderTime)) != 0) ?
			(timeDifference - (desiredTime - maxUnderTime)) / (totalTime - (desiredTime - maxUnderTime)) : 0;

		double expArg = normalizedTime + normalizedTimeDifference;

		if (expArg < ScenicRoutingConstants::MAX_EXPONENT)
			timePenality = std::exp(expArg) - 1;
		else
			timePenality = std::numeric_limits<double>::max();
	}
	else
	{
		double expArg = normalizedTime;
		if (expArg < ScenicRoutingConstants::MAX_EXPONENT)
			timePenality = std::exp(expArg) - 1;
		else
			timePenality = std::numeric_limits<double>::max();
	}

	double normalizedPriority = ((static_cast<double>(totalPriority) - ScenicRoutingConstants::MIN_PRIORITY) 
		/ (ScenicRoutingConstants::MAX_PRIORITY * routeSize - ScenicRoutingConstants::MIN_PRIORITY));

	int negativesCount = static_cast<int>(std::count(route.begin(), route.end(), -1));
	double normalizedNegativesCount = 0.0;

	double halfLen = static_cast<double>(routeSize / 2);

	if (negativesCount > 0)
		normalizedNegativesCount = (static_cast<double>(negativesCount) - halfLen) / (static_cast<double>(routeSize) - halfLen);

	fitness += ScenicRoutingConstants::NEGATIVES_WEIGHT * normalizedNegativesCount;

	if (totalTime < desiredTime)
		fitness -= ScenicRoutingConstants::UNDER_TIME_BONUS;

	fitness -= ScenicRoutingConstants::PRIORITY_WEIGHT * normalizedPriority;

	fitness += ScenicRoutingConstants::TIME_WEIGHT * timePenality;

	return {fitness, totalTime, totalPriority};
}

TimePriorityInfo FitnessCalculator::GetTimeAndPriorityInfo(const InputDataPtr data, const PoisIdx& ids, int poiIndex, size_t index)
{
	double time = 0.0;

	if (poiIndex >= data->GetPoisSize())
		throw std::invalid_argument("PoiId bigger then pois list");

	int poiIncrementedIndex = poiIndex + 1;

	if (index == 0)
	{
		CalculateTime(data->GetTimeMatrix(), poiIncrementedIndex, 0, time);
	}

	if (index > 0)
	{
		int prevPoiIndex = ids[index - 1];

		size_t copiedIndex = index;

		while (prevPoiIndex == -1 && copiedIndex >= 0)
		{
			copiedIndex--;

			if (copiedIndex == 0)
			{
				prevPoiIndex = poiIncrementedIndex - 1;

				if (index != ids.size() - 1)
					poiIncrementedIndex = ids[index + 1] + 1;
			}
		}

		prevPoiIndex++;

		CalculateTime(data->GetTimeMatrix(), poiIncrementedIndex, prevPoiIndex, time);
	}

	if (index == ids.size() - 1)
	{
		CalculateTime(data->GetTimeMatrix(), data->GetTimeMatrixSize() - 1, poiIncrementedIndex, time);
	}

	return { time, data->GetPoiPriority(poiIndex) };
}

void FitnessCalculator::CalculateTime(const TimeMatrix& timeMatrix, size_t current, size_t prev, double& totalTime)
{
	auto time = timeMatrix[prev][current];

	totalTime += time;
}

RouteInfo FitnessCalculator::EvaluateAntColonyRoute(const PoisIdx& route, const InputDataPtr data, double desiredTime, double totalTime)
{
	double fitness = 0.0;
	int totalPriority = 0;

	int routeSize = static_cast<int>(route.size());
	int poisSize = data->GetPoisSize();

	if (route.empty())
	{
		return { 0.0, 0.0, 0 };
	}

	for (const auto& idx : route)
	{
		if (idx == poisSize || idx == poisSize + 1)
			continue;

		totalPriority += data->GetPoiPriority(idx);
	}

	double timePenaltyWeight = 2.0;
	double timePenalty = 0.0;
	if (totalTime < desiredTime * 0.9)
	{
		timePenalty = (desiredTime - totalTime) * timePenaltyWeight;

	}
	double priorityScore = 1.5 * totalPriority;

	fitness = 1.0 / (priorityScore * ((desiredTime / totalTime) + timePenaltyWeight));

	return { fitness, 0, totalPriority };
}
