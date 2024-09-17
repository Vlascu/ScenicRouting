#pragma once
#include "Aliases.h"
#include "Random.h"

#include <vector>
class Ant
{
public:
	Ant(int poiCount)
		: m_poiCount(poiCount)
		, m_visitedPois(poiCount, false)
		, m_route()
		, m_totalTime(0.0)
	{
		//empty
	}

	void Reset();

	bool IsVisited(int poiIndex) const;
	void VisitPOI(int poiIndex, double travelTime);
	int GetLastVisitedPOI() const;

	bool CanContinue(double desiredTimeLimit) const;

	double GetTotalTime() const;
	void AddTravelTime(double travelTime);

	PoisIdx GetRoute() const;
private:
	int m_poiCount;
	std::vector<bool> m_visitedPois;
	PoisIdx m_route;
	double m_totalTime;
};

