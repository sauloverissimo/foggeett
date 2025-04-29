#pragma once

#include <vector>
#include <map>
#include <string>
#include <optional>

using Tick = std::map<std::string, double>;

// Funções auxiliares
std::vector<double> prepare_y(const std::vector<Tick>& ticks,
                              const std::string& campo,
                              std::optional<int> n,
                              const std::string& direction);

std::vector<double> prepare_y(const std::vector<double>& values,
                              std::optional<int> n,
                              const std::string& direction);
