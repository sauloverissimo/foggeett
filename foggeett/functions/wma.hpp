#pragma once
#include <vector>
#include <optional>
#include <string>
#include "functions.hpp"

std::optional<KPIResult> wma(
    const std::vector<Tick>& ticks,
    const std::string& campo,
    std::optional<int> n = std::nullopt,
    const std::string& direction = "desc"
);

std::optional<KPIResult> wma(
    const std::vector<double>& values,
    std::optional<int> n = std::nullopt,
    const std::string& direction = "desc"
);
