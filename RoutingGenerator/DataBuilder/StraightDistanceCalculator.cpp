#include "StraightDistanceCalculator.h"
#include "GeometryCalculator.h"
#include "DataConstants.h"

TimeInfo StraightDistanceCalculator::GetTimeInfo(const CoordinateList& coordinates) const
{
	double totalTime = 0.0;

	TimeMatrix timeMatrix;

	for (size_t i = 0; i < coordinates.size(); i++)
	{
		std::vector<double> row;

		for (size_t j = 0; j < coordinates.size(); j++)
		{
			auto time = GetTimeBetweenPoints(coordinates[i], coordinates[j]);

			totalTime += time;

			row.push_back(time);
		}

		timeMatrix.push_back(row);
	}

	return { timeMatrix, totalTime };
}

double StraightDistanceCalculator::GetTimeBetweenPoints(const Coordinate& start, const Coordinate& end) const
{
	return GeometryCalculator::HaversineDistance(start, end) / DataBuildingConstants::AVERAGE_MS_WALKING_SPEED;
}
