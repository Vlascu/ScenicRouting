#pragma once

#include <memory>

using PoiPtr = std::shared_ptr<class IPOI>;

class IPOI 
{
public:
	virtual double GetLatitude() const = 0;
	virtual double GetLongitude() const = 0;
	virtual int GetPriority() const = 0;

	virtual ~IPOI() = default;
};