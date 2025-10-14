#pragma once
#include <vector>
#include <string>
#include <optional>
#include "functions.hpp"

// Helper function to convert string to uppercase
std::string to_upper_rsi(const std::string& str);

// Internal compute function
std::optional<KPIResult> compute_rsi(
    const std::vector<double>& values,
    int n = 14,
    const std::string& field = "",
    int window = 0
);

// RSI com Ticks (overload 1)
std::optional<KPIResult> rsi(
    const std::vector<Tick>& ticks,
    const std::string& field,
    int n = 14,
    int window = 60
);

// RSI com vetor direto (overload 2)
std::optional<KPIResult> rsi(
    const std::vector<double>& values,
    int n = 14,
    int window = 60
);

