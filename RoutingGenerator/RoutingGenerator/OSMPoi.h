#pragma once
#include "IPOI.h"
#include "Aliases.h"

class OSMPoi : public IPOI
{
public:
	OSMPoi() = default;
	OSMPoi(const Coordinate& coordinate, int priority);

	double GetLatitude() const override;
	double GetLongitude() const override;
	int GetPriority() const override;
private:
	double m_lat;
	double m_lon;
	int m_priority;
};

