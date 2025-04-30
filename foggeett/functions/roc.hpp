#pragma once
#include <vector>
#include <string>
#include <optional>
#include "functions.hpp"

// Versão principal com Tick + campo
std::optional<KPIResult> roc(
    const std::vector<Tick>& ticks,
    const std::string& field,
    std::optional<int> n = std::nullopt,
    const std::string& direction = "desc"
);

// Versão com vetor de valores
std::optional<KPIResult> roc(
    const std::vector<double>& values,
    std::optional<int> n = std::nullopt,
    const std::string& direction = "desc"
);

