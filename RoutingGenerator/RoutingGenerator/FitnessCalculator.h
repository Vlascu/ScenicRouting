#pragma once
#include "Aliases.h"
#include "Constants.h"
#include "IInputData.h"

#include <vector>
#include <cmath>
#include <iostream>

struct POIData;

using RouteInfo = std::tuple<double, double, int>;
using TimePriorityInfo = std::pair<double, int>;

class FitnessCalculator
{
public:
	RouteInfo static EvaluateRoute(const PoisIdx& route, const InputDataPtr data, double desiredTime);
	RouteInfo static EvaluateAntColonyRoute(const PoisIdx& route, const InputDataPtr data, double desiredTime, double totalTime);
private:
	TimePriorityInfo static GetTimeAndPriorityInfo(const InputDataPtr data, const PoisIdx& ids, int poiIndex, size_t index);
	void static CalculateTime(const TimeMatrix& timeMatrix, size_t current, size_t prev, double& totalTime);

};

