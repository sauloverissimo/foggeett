#pragma once
#include <vector>
#include <string>
#include <optional>
#include "functions.hpp"

// EMA com Ticks
std::optional<KPIResult> ema(
    const std::vector<Tick>& ticks,
    const std::string& field,
    std::optional<int> n = std::nullopt,
    const std::string& direction = "desc"
);

// EMA com vetor direto
std::optional<KPIResult> ema(
    const std::vector<double>& values,
    std::optional<int> n = std::nullopt,
    const std::string& direction = "desc"
);

