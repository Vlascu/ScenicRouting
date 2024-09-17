#pragma once

#include <random>

template <typename T>
struct UniformDistribution;

template <>
struct UniformDistribution<int>
{
	using type = std::uniform_int_distribution<int>;
};

template <>
struct UniformDistribution<double>
{
	using type = std::uniform_real_distribution<double>;
};

template <class NumberType>
NumberType GenerateRandomNumber(NumberType start, NumberType end)
{
	static std::random_device rd;
	static std::mt19937 mt(rd());

	typename UniformDistribution<NumberType>::type dist(start, end);

	return dist(mt);
}
