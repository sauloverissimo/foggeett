#pragma once

#include <vector>
#include <optional>
#include <string>
#include <map>
#include "utils.hpp"

struct KPIResult;

std::optional<KPIResult> sma(const std::vector<Tick>& ticks,
                             const std::string& campo,
                             std::optional<int> n = std::nullopt,
                             const std::string& direction = "desc");

std::optional<KPIResult> sma(const std::vector<double>& values,
                             std::optional<int> n = std::nullopt,
                             const std::string& direction = "desc");
