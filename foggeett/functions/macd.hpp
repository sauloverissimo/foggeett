#pragma once
#include <vector>
#include <string>
#include <optional>
#include "functions.hpp"

// MACD com Ticks
std::optional<KPIResult> macd(
    const std::vector<Tick>& ticks,
    const std::string& field,
    int short_period = 12,
    int long_period = 26,
    std::optional<int> signal_period = std::nullopt,
    const std::string& direction = "desc"
);

// MACD com vetor direto
std::optional<KPIResult> macd(
    const std::vector<double>& values,
    int short_period = 12,
    int long_period = 26,
    std::optional<int> signal_period = std::nullopt,
    const std::string& direction = "desc"
);

