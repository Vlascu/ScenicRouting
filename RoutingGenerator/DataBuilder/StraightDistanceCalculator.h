#pragma once
#include "ITimeMatrixCalculator.h"

class StraightDistanceCalculator : public ITimeMatrixCalculator
{
public:
	TimeInfo GetTimeInfo(const CoordinateList& coordinates) const override;
	double GetTimeBetweenPoints(const Coordinate& start, const Coordinate& end) const override;
};

