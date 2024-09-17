#pragma once
#include "IInputData.h"

class OSMInputData : public IInputData
{
public:
	OSMInputData() = default;
	OSMInputData(const TimeInfo& timeInfo, const POIDeque& pois);

	int GetPoisSize() const override;
	int GetPoiPriority(int idx) const override;
	Coordinate GetPoiCoordinate(int idx) const override;

	double GetMaxTime() const override;
	TimeMatrix GetTimeMatrix() const override;
	double GetTravelTime(int poi1, int poi2) const override;
	int GetTimeMatrixSize() const override;

	double GetTravelTimeToEnd(int poi) const override;

	StartEndCoordinates GetStartEndCoordinates() override;

	bool IsTimeMatrixEmpty() const override;
	bool IsPoisEmpty() const override;
private:
	TimeInfo m_timeInfo;

	POIDeque m_pois;
};

