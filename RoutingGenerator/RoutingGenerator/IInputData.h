#pragma once
#include "Aliases.h"

#include <memory>

using InputDataPtr = std::shared_ptr<class IInputData>;
using StartEndCoordinates = std::pair<Coordinate, Coordinate>;

class IInputData
{
public:
	virtual int GetPoisSize() const = 0;
	virtual int GetPoiPriority(int idx) const = 0;
	virtual Coordinate GetPoiCoordinate(int idx) const = 0;

	virtual double GetMaxTime() const = 0;
	
	virtual int GetTimeMatrixSize() const = 0;
	virtual TimeMatrix GetTimeMatrix() const = 0;
	
	virtual double GetTravelTime(int poi1, int poi2) const = 0;
	virtual double GetTravelTimeToEnd(int poi) const = 0;

	virtual StartEndCoordinates GetStartEndCoordinates() = 0;

	virtual bool IsTimeMatrixEmpty() const = 0;
	virtual bool IsPoisEmpty() const = 0;

	virtual ~IInputData() = default;
};