#include "OSMPoi.h"

OSMPoi::OSMPoi(const Coordinate& coordinate, int priority)
	: m_lat(coordinate.first)
	, m_lon(coordinate.second)
	, m_priority(priority)
{
	//empty
}

double OSMPoi::GetLatitude() const
{
	return m_lat;
}

double OSMPoi::GetLongitude() const
{
	return m_lon;
}

int OSMPoi::GetPriority() const
{
	return m_priority;
}
