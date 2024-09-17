#include "Ant.h"

void Ant::Reset()
{
	m_visitedPois.assign(m_poiCount, false);
	m_route.clear();
	m_totalTime = 0.0;
}

void Ant::VisitPOI(int poiIndex, double travelTime)
{
	m_route.push_back(poiIndex);
	m_visitedPois[poiIndex] = true;
	AddTravelTime(travelTime);
}

bool Ant::CanContinue(double desiredTimeLimit) const
{
	return m_route.size() < m_poiCount && m_totalTime < desiredTimeLimit;
}

bool Ant::IsVisited(int poiIndex) const
{
	return m_visitedPois[poiIndex];
}

double Ant::GetTotalTime() const
{
	return m_totalTime;
}

void Ant::AddTravelTime(double travelTime)
{
	m_totalTime += travelTime;
}

PoisIdx Ant::GetRoute() const
{
	return m_route;
}

int Ant::GetLastVisitedPOI() const
{
	if (m_route.empty())
		return -1;
	return m_route.back();
}
