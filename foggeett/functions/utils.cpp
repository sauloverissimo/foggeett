#include "utils.hpp"
#include <algorithm>

std::vector<double> prepare_y(const std::vector<Tick>& ticks,
                              const std::string& campo,
                              std::optional<int> n,
                              const std::string& direction) {
    std::vector<double> y;
    for (const auto& tick : ticks) {
        auto it = tick.find(campo);
        if (it != tick.end())
            y.push_back(it->second);
    }
    if (direction == "desc") {
        std::reverse(y.begin(), y.end());
    }
    if (n.has_value() && n.value() > 0 && y.size() > n.value()) {
        y = std::vector<double>(y.end() - n.value(), y.end());
    }
    return y;
}

std::vector<double> prepare_y(const std::vector<double>& values,
                              std::optional<int> n,
                              const std::string& direction) {
    std::vector<double> y = values;
    if (direction == "desc") {
        std::reverse(y.begin(), y.end());
    }
    if (n.has_value() && n.value() > 0 && y.size() > n.value()) {
        y = std::vector<double>(y.end() - n.value(), y.end());
    }
    return y;
}
