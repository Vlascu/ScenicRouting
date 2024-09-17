#pragma once
#include "CommonAliases.h"

#include <vector>
#include <memory>

using ITimeMatrixCalculatorPtr = std::shared_ptr<class ITimeMatrixCalculator>;

class ITimeMatrixCalculator
{
public:
	virtual TimeInfo GetTimeInfo(const CoordinateList& coordinates) const = 0;
	virtual double GetTimeBetweenPoints(const Coordinate& start, const Coordinate& end) const = 0;

	virtual ~ITimeMatrixCalculator() = default;
};