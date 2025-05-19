#pragma once

#include <map>
#include <string>
#include <vector>
#include <variant>
#include "functions.hpp"

std::map<std::string, double> enrich(const std::vector<Tick>& ticks,
                                     const std::map<std::string, KPIConfig>& kpis);


