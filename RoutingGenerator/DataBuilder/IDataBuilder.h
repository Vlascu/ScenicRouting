#pragma once
#include "CommonAliases.h"
#include "ITimeMatrixCalculator.h"

#include <memory>

using DataGetterPtr = std::shared_ptr<class IDataBuilder>;

class IDataBuilder
{
public:
	virtual void BuildData(const Coordinate& start, const Coordinate& end, double searchRadius, const ITimeMatrixCalculatorPtr timeCalculator, const std::string& path) = 0;
	
	virtual ~IDataBuilder() = default;
};