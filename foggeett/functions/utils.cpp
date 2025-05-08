#include "utils.hpp"
#include <algorithm>

std::vector<double> prepare_y(
    const std::vector<Tick>& ticks,
    const std::string& campo,
    std::optional<int> n,
    const std::string& direction) {

    std::vector<double> y;

    for (const auto& t : ticks) {
        auto it = t.find(campo);
        if (it != t.end())
            y.push_back(it->second);
    }

    if (direction == "asc") {
        std::reverse(y.begin(), y.end());
    }

    if (n && y.size() > static_cast<size_t>(*n)) {
        y.resize(*n);
    }

    return y;
}

std::vector<double> prepare_y(
    const std::vector<double>& values,
    std::optional<int> n,
    const std::string& direction) {

    std::vector<double> y = values;

    if (direction == "asc") {
        std::reverse(y.begin(), y.end());
    }

    if (n && y.size() > static_cast<size_t>(*n)) {
        y.resize(*n);
    }

    return y;
}

std::vector<Tick> prepare_ticks(
    const std::vector<Tick>& ticks,
    std::optional<int> n,
    const std::string& direction) {

    std::vector<Tick> out = ticks;

    if (direction == "asc") {
        std::reverse(out.begin(), out.end());
    }

    if (n && out.size() > static_cast<size_t>(*n)) {
        out.resize(*n);
    }

    return out;
}

std::vector<Tick> prepare_ticks_fix(
    const std::vector<Tick>& ticks,
    std::optional<int> n,
    const std::string& direction) {
    
    std::vector<Tick> out = ticks;

    if (direction == "asc") {
        std::reverse(out.begin(), out.end());  // passa para cronológico
    }

    if (n && out.size() > static_cast<size_t>(*n)) {
        // PEGA os últimos N da ordem cronológica
        out = std::vector<Tick>(out.end() - *n, out.end());
    }

    return out;
}
