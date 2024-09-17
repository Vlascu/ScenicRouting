#pragma once
#include "IInputData.h"

#include <memory>

using DataLoaderPtr = std::shared_ptr<class IDataLoader>;

class IDataLoader
{
public:
	virtual InputDataPtr LoadData(const char* data) = 0;

	virtual ~IDataLoader() = default;
};