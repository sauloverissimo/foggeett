#pragma once
#include <vector>
#include <string>
#include <optional>
#include "functions.hpp"

// MOM tradicional com Tick
std::optional<KPIResult> mom(
    const std::vector<Tick>& ticks,
    const std::string& field,
    std::optional<int> n = std::nullopt,
    const std::string& direction = "desc"
);

// MOM com vetor direto
std::optional<KPIResult> mom(
    const std::vector<double>& values,
    std::optional<int> n = std::nullopt,
    const std::string& direction = "desc"
);

