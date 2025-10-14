#pragma once
#include <vector>
#include <string>
#include <optional>
#include "functions.hpp"

// Helper function to convert string to uppercase
std::string to_upper(const std::string& str);

// Internal compute function
std::optional<KPIResult> compute_macd(
    const std::vector<double>& y,
    int short_period,
    int long_period,
    int signal_period,
    const std::string& field = "",
    int n = 0,
    const std::string& direction = "desc"
);

// MACD com Ticks (overload 1)
std::optional<KPIResult> macd(
    const std::vector<Tick>& ticks,
    const std::string& field,
    int short_period = 12,
    int long_period = 26,
    std::optional<int> signal_period = std::nullopt,
    const std::string& direction = "desc"
);

// MACD com vetor direto (overload 2)
std::optional<KPIResult> macd(
    const std::vector<double>& values,
    int short_period = 12,
    int long_period = 26,
    std::optional<int> signal_period = std::nullopt,
    const std::string& direction = "desc"
);

// MACD com parametros adicionais - Python-like interface (overload 3)
std::optional<KPIResult> macd(
    const std::vector<Tick>& ticks,
    const std::string& field,
    int short_period,
    int long_period,
    int signal_period,
    int n,
    const std::string& source,
    const std::string& direction = "desc"
);

