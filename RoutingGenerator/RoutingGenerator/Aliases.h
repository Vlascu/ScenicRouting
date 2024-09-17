#pragma once
#include "IPOI.h"

#include <utility>
#include <vector>
#include <deque>

using PoisIdx = std::vector<int>;
using POIDeque = std::deque<PoiPtr>;
using POIVector = std::vector<PoiPtr>;

using Coordinate = std::pair<double, double>;
using CoordinateList = std::vector<Coordinate>;

using TimeMatrix = std::vector<std::vector<double>>;
using TimeInfo = std::pair<TimeMatrix, double>;