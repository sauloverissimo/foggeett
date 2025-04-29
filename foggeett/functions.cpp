#include "functions.hpp"
#include <numeric>

std::optional<KPIResult> slope(const std::vector<Tick>& ticks,
                               const std::string& campo,
                               int n) {
    int m = std::min<int>(ticks.size(), n);
    if (m < 2) return std::nullopt;

    // pega os primeiros m valores de campo
    std::vector<double> y;
    y.reserve(m);
    for (int i = 0; i < m; ++i) {
        auto it = ticks[i].find(campo);
        if (it == ticks[i].end()) return std::nullopt;
        y.push_back(it->second);
    }

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
    r.acro     = "SLOPEP_" + campo.substr(0,3);
    r.value     = slope_v;
    r.description = "Inclinação da regressão linear de " + campo;
    // debug minimal: primeiro e último ponto
    r.debug = {
        {"x0", x.front()}, {"y0", y.front()},
        {"xN", x.back()},  {"yN", y.back()}
    };
    return r;
}

std::optional<KPIResult> sma(const std::vector<Tick>& ticks,
                             const std::string& campo,
                             int n) {
    int m = std::min<int>(ticks.size(), n);
    if (m == 0) return std::nullopt;

    double sum = 0.0;
    for (int i = 0; i < m; ++i) {
        auto it = ticks[i].find(campo);
        if (it == ticks[i].end()) return std::nullopt;
        sum += it->second;
    }
    double mean = sum / m;

    KPIResult r;
    r.acro     = "SMA_" + campo.substr(0,3);
    r.value     = mean;
    r.description = "Média móvel simples de " + campo;
    r.debug     = { {"count", static_cast<double>(m)} };
    return r;
}

std::map<std::string, double> enrich(const std::vector<Tick>& ticks,
                                     const std::map<std::string, KPIConfig>& kpis) {
    std::map<std::string, double> results;
    for (auto const& [key, cfg] : kpis) {
        std::optional<KPIResult> r;
        int window = cfg.params.count("n") ? int(cfg.params.at("n")) : int(ticks.size());

        if (cfg.function == "slope") {
            r = slope(ticks, cfg.campo, window);
        }
        else if (cfg.function == "sma") {
            r = sma(ticks, cfg.campo, window);
        }
        // se quiser mais funções, basta estender aqui...

        if (r.has_value()) {
            results[key] = r->value;
        }
    }
    return results;
}
