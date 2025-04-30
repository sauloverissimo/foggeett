#pragma once
#include "functions.hpp"
#include <optional>

// === Sinal tradicional ===
// Retorna SMA de um campo específico em uma janela n
std::optional<KPIResult> sma(
    const std::vector<Tick>& ticks,
    const std::string& campo,
    std::optional<int> n,
    const std::string& direction);

// === Sinal alternativo ===
// Permite calcular a SMA diretamente sobre um vetor de valores
std::optional<KPIResult> sma(
    const std::vector<double>& values,
    std::optional<int> n,
    const std::string& direction);

