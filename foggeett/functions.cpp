#include "functions.hpp"
#include <numeric>
#include <algorithm>

namespace {

// Função auxiliar para preparar vetor y de trabalho
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

} // namespace interno

// Implementação do slope

std::optional<KPIResult> slope(const std::vector<Tick>& ticks,
                               const std::string& campo,
                               std::optional<int> n,
                               const std::string& direction) {
    auto y = prepare_y(ticks, campo, n, direction);
    if (y.size() < 2) return std::nullopt;

    int N = y.size();
    std::vector<double> x(N);
    std::iota(x.begin(), x.end(), 0.0);

    double sum_x  = std::accumulate(x.begin(), x.end(), 0.0);
    double sum_y  = std::accumulate(y.begin(), y.end(), 0.0);
    double sum_xx = 0.0, sum_xy = 0.0;
    for (int i = 0; i < N; ++i) {
        sum_xx += x[i] * x[i];
        sum_xy += x[i] * y[i];
    }

    double denom = N * sum_xx - sum_x * sum_x;
    if (denom == 0) return std::nullopt;

    double slope_v = (N * sum_xy - sum_x * sum_y) / denom;

    KPIResult r;
    r.acro = "SLOPEP_" + campo.substr(0, 3);
    r.value = slope_v;
    r.description = "Inclinação da regressão linear de " + campo;
    r.debug = {
        {"x0", x.front()}, {"y0", y.front()},
        {"xN", x.back()}, {"yN", y.back()}
    };
    return r;
}

std::optional<KPIResult> slope(const std::vector<double>& values,
                               std::optional<int> n,
                               const std::string& direction) {
    std::vector<double> y = prepare_y(values, n, direction);
    if (y.size() < 2) return std::nullopt;

    int N = y.size();
    std::vector<double> x(N);
    std::iota(x.begin(), x.end(), 0.0);

    double sum_x  = std::accumulate(x.begin(), x.end(), 0.0);
    double sum_y  = std::accumulate(y.begin(), y.end(), 0.0);
    double sum_xx = 0.0, sum_xy = 0.0;
    for (int i = 0; i < N; ++i) {
        sum_xx += x[i] * x[i];
        sum_xy += x[i] * y[i];
    }

    double denom = N * sum_xx - sum_x * sum_x;
    if (denom == 0) return std::nullopt;

    double slope_v = (N * sum_xy - sum_x * sum_y) / denom;

    KPIResult r;
    r.acro = "SLOPEP_vec";
    r.value = slope_v;
    r.description = "Inclinação da regressão linear do vetor de valores";
    r.debug = {
        {"x0", x.front()}, {"y0", y.front()},
        {"xN", x.back()}, {"yN", y.back()}
    };
    return r;
}

// Implementação do sma

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

std::map<std::string, double> enrich(const std::vector<Tick>& ticks,
                                     const std::map<std::string, KPIConfig>& kpis) {
    std::map<std::string, double> results;
    for (auto const& [key, cfg] : kpis) {
        std::optional<KPIResult> r;
        std::optional<int> window = cfg.params.count("n") ? std::optional<int>(cfg.params.at("n")) : std::nullopt;
        std::string direction = cfg.params.count("direction") ? (cfg.params.at("direction") == 1 ? "asc" : "desc") : "desc";

        if (cfg.function == "slope") {
            r = slope(ticks, cfg.campo, window, direction);
        }
        else if (cfg.function == "sma") {
            r = sma(ticks, cfg.campo, window, direction);
        }
        if (r.has_value()) {
            results[key] = r->value;
        }
    }
    return results;
}
