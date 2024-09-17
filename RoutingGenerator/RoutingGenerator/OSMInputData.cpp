#include "OSMInputData.h"
#include "IPOI.h"

OSMInputData::OSMInputData(const TimeInfo& timeInfo, const POIDeque& pois)
	: m_timeInfo(timeInfo)
	, m_pois(pois)
{
	//empty;
}

int OSMInputData::GetPoisSize() const
{
	return static_cast<int>(m_pois.size());
}

double OSMInputData::GetMaxTime() const
{
	return m_timeInfo.second;
}

TimeMatrix OSMInputData::GetTimeMatrix() const
{
	return m_timeInfo.first;
}

double OSMInputData::GetTravelTime(int poi1, int poi2) const
{
	return m_timeInfo.first[poi1][poi2];
}

double OSMInputData::GetTravelTimeToEnd(int poi) const
{
	auto endPoint = m_timeInfo.first.size() - 1;

	return m_timeInfo.first[poi][endPoint];
}

StartEndCoordinates OSMInputData::GetStartEndCoordinates()
{
	PoiPtr start = m_pois.front();
	PoiPtr end = m_pois.back();

	//if start-end present in list
	if (start->GetPriority() == 0 && end->GetPriority() == 0)
	{
		Coordinate startCoordinates(start->GetLatitude(), start->GetLongitude());
		Coordinate endCoordinates(end->GetLatitude(), end->GetLongitude());

		m_pois.pop_front();
		m_pois.pop_back();

		return { startCoordinates, endCoordinates };
	}
	else
		return {};
}

bool OSMInputData::IsTimeMatrixEmpty() const
{
	return m_timeInfo.first.empty();
}

bool OSMInputData::IsPoisEmpty() const
{
	return m_pois.empty();
}

int OSMInputData::GetTimeMatrixSize() const
{
	return static_cast<int>(m_timeInfo.first.size());
}

int OSMInputData::GetPoiPriority(int idx) const
{
	return m_pois[static_cast<size_t>(idx)]->GetPriority();
}

Coordinate OSMInputData::GetPoiCoordinate(int idx) const
{
	PoiPtr poi = m_pois[static_cast<size_t>(idx)];

	return { poi->GetLatitude(), poi->GetLongitude() };
}
