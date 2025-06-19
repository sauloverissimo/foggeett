#include "sma.hpp"
#include "utils.hpp"
#include "functions.hpp"
#include <numeric>
#include <algorithm>



std::optional<KPIResult> sma(const std::vector<Tick>& ticks,
                             const std::string& campo,
                             std::optional<int> n,
                             const std::string& direction) {
    auto y = prepare_y(ticks, campo, n, direction);
    if (y.empty()) return std::nullopt;

    double sum = std::accumulate(y.begin(), y.end(), 0.0);
    double mean = sum / y.size();

    KPIResult r;
    r.acro = "SMA_" + campo.substr(0, 3);
    r.value = mean;
    r.description = "Média móvel simples de " + campo;
    r.debug = { {"count", static_cast<double>(y.size())} };
    return r;
}

std::optional<KPIResult> sma(const std::vector<double>& values,
                             std::optional<int> n,
                             const std::string& direction) {
    auto y = prepare_y(values, n, direction);
    if (y.empty()) return std::nullopt;

    double sum = std::accumulate(y.begin(), y.end(), 0.0);
    double mean = sum / y.size();

    KPIResult r;
    r.acro = "SMA_vec";
    r.value = mean;
    r.description = "Média móvel simples do vetor de valores";
    r.debug = { {"count", static_cast<double>(y.size())} };
    return r;
}

