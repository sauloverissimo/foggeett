#pragma once

#include <string>
#include <vector>
#include <optional>
#include <map>

using Tick = std::map<std::string, double>;

struct KPIConfig {
    std::string function;
    std::string campo;
    std::map<std::string, float> params;
};

struct KPIResult {
    std::string acro;
    double value;
    std::string description;
    std::map<std::string, double> debug;
};

// Funções públicas
std::optional<KPIResult> slope(const std::vector<Tick>& ticks,
                               const std::string& campo,
                               std::optional<int> n = std::nullopt,
                               const std::string& direction = "desc");

std::optional<KPIResult> slope(const std::vector<double>& values,
                               std::optional<int> n = std::nullopt,
                               const std::string& direction = "desc");

std::optional<KPIResult> sma(const std::vector<Tick>& ticks,
                             const std::string& campo,
                             std::optional<int> n = std::nullopt,
                             const std::string& direction = "desc");

std::optional<KPIResult> sma(const std::vector<double>& values,
                             std::optional<int> n = std::nullopt,
                             const std::string& direction = "desc");

std::map<std::string, double> enrich(const std::vector<Tick>& ticks,
                                     const std::map<std::string, KPIConfig>& kpis);
