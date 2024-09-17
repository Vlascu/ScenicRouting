#pragma once
#include <exception>

class ScenicRoutingException : public std::exception
{
public:
	ScenicRoutingException(const char* msg) : exception(msg) {};
};

class FileException : public ScenicRoutingException
{
public:
	FileException(const char* msg) : ScenicRoutingException(msg) {};
};

class OptimizationResultException : public ScenicRoutingException
{
public:
	OptimizationResultException(const char* msg) : ScenicRoutingException(msg) {};
};