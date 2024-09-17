#pragma once
#include "IDataLoader.h"

class FileDataLoader : public IDataLoader
{
public:
	InputDataPtr LoadData(const char* data) override;
};

