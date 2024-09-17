#pragma once
#include "CommonAliases.h"

#include <cmath>

class GeometryCalculator
{
public:
	static double Deg2Rad(double deg);

	static double HaversineDistance(const Coordinate& point1, const Coordinate& point2);
};

