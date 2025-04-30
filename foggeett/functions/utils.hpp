#pragma once
#include <vector>
#include <string>
#include <optional>
#include "functions.hpp"

// Extrai vetor numérico de um campo de uma lista de Tick
std::vector<double> prepare_y(
    const std::vector<Tick>& ticks,
    const std::string& campo,
    std::optional<int> n = std::nullopt,
    const std::string& direction = "desc"
);

// Recorta e ordena um vetor direto de valores numéricos
std::vector<double> prepare_y(
    const std::vector<double>& values,
    std::optional<int> n = std::nullopt,
    const std::string& direction = "desc"
);

// Recorta e ordena ticks completos (para OBV, AD etc.)
std::vector<Tick> prepare_ticks(
    const std::vector<Tick>& ticks,
    std::optional<int> n = std::nullopt,
    const std::string& direction = "desc"
);
