#pragma once
#include <vector>
#include <string>
#include <optional>
#include "functions.hpp"  // Inclui Tick, KPIResult, etc.

/**
 * ADX: Average Directional Index
 * Mede a força da tendência com base em high, low e price.
 *
 * @param ticks      Vetor de candles (Tick) com campos: price, high, low
 * @param campo      Nome do campo principal analisado (ex: "price")
 * @param n          Janela de suavização (default = 14)
 * @param direction  "asc" para mais antigo → recente, "desc" (padrão) para mais recente → antigo
 * @return           KPIResult com valor suavizado e debug de cálculos intermediários
 */

std::optional<KPIResult> adx(const std::vector<Tick>& ticks,
                             const std::string& campo,
                             std::optional<int> n = std::nullopt,
                             const std::string& direction = "desc");
