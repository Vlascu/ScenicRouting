#include "GeometryCalculator.h"
#include "DataConstants.h"

double GeometryCalculator::Deg2Rad(double deg)
{
	return deg * (DataBuildingConstants::PI / 180);
}

double GeometryCalculator::HaversineDistance(const Coordinate& point1, const Coordinate& point2)
{
	auto [lat1, lon1] = point1;
	auto [lat2, lon2] = point2;

	double dLat = Deg2Rad(lat2 - lat1);
	double dLon = Deg2Rad(lon2 - lon1);

	lat1 = Deg2Rad(lat1);
	lat2 = Deg2Rad(lat2);

	double a = sin(dLat / 2) * sin(dLat / 2) +
		cos(lat1) * cos(lat2) *
		sin(dLon / 2) * sin(dLon / 2);
	double c = 2 * atan2(sqrt(a), sqrt(1 - a));

	return DataBuildingConstants::R * c;
}

