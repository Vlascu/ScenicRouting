#include "OSMDataBuilder.h"
#include "StraightDistanceCalculator.h"

#include <memory>

Coordinate FROM(45.638217, 25.584412);
Coordinate TO(45.64589, 25.59932);
const double DESIRED_TIME_MINS = 40;

int main()
{
	auto dataBuilder = std::make_shared<OSMDataBuilder>();
	auto timeCalculator = std::make_shared<StraightDistanceCalculator>();

	dataBuilder->BuildData(FROM, TO, DESIRED_TIME_MINS, timeCalculator, "../Output/brasov_saguna_consiliu_50.txt");
	return 0;
}