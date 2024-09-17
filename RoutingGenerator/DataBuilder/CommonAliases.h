#pragma once

#include <vector>
#include <string>
#include <unordered_map>

using Coordinate = std::pair<double, double>;
using CoordinateList = std::vector<Coordinate>;

using TimeMatrix = std::vector<std::vector<double>>;
using TimeInfo = std::pair<TimeMatrix, double>;

using Tags = std::unordered_map<char*, char*>;

using PoiTypes = std::vector<std::string>;