#pragma once

#include <string>
#include <vector>
#include <optional>
#include <map>

// Um Tick é um dicionário campo→valor
using Tick = std::map<std::string, double>;

// Configuração de cada KPI
struct KPIConfig {
    std::string function;                  // nome da função: "slope", "sma", ...
    std::string campo;                     // ex: "price"
    std::map<std::string, float> params;   // ex: {"n": 20}
};

// Resultado de um KPI (sigla, valor, descrição e dados de debug simples)
struct KPIResult {
    std::string acro;
    double value;
    std::string description;
    std::map<std::string, double> debug;
};

// Declarações das funções
std::optional<KPIResult> slope(const std::vector<Tick>& ticks,
                               const std::string& campo,
                               int n);

std::optional<KPIResult> sma(const std::vector<Tick>& ticks,
                             const std::string& campo,
                             int n);

std::map<std::string, double> enrich(const std::vector<Tick>& ticks,
                                     const std::map<std::string, KPIConfig>& kpis);
