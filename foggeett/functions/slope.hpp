#pragma once

#include <vector>
#include <optional>
#include <string>
#include <map>
#include "utils.hpp"
#include "functions.hpp"

struct KPIResult;

// Versão com Tick + campo
std::optional<KPIResult> slope(const std::vector<Tick>& ticks,
                               const std::string& campo,
                               std::optional<int> n = std::nullopt,
                               const std::string& direction = "desc");

// Versão com vetor direto
std::optional<KPIResult> slope(const std::vector<double>& values,
                               std::optional<int> n = std::nullopt,
                               const std::string& direction = "desc");
