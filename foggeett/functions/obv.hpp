// #pragma once
// #include "functions.hpp"
// #include <optional>
// #include <string>

// // OBV: On-Balance Volume
// std::optional<KPIResult> obv(
//     const std::vector<Tick>& ticks,
//     const std::string& volume_field,
//     std::optional<int> n = std::nullopt,
//     const std::string& direction = "desc"
// );

#pragma once
#include "functions.hpp"
#include <optional>
#include <string>

// OBV: On-Balance Volume — acumula volume conforme a direção do preço
std::optional<KPIResult> obv(
    const std::vector<Tick>& ticks,
    const std::string& volume_field,
    std::optional<int> n = std::nullopt,
    const std::string& direction = "desc"
);
