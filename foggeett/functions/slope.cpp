#include "slope.hpp"
#include "utils.hpp"
#include "functions.hpp"
#include <numeric>
#include <algorithm>




std::optional<KPIResult> slope(const std::vector<Tick>& ticks,
                               const std::string& campo,
                               std::optional<int> n,
                               const std::string& direction) {
    auto y = prepare_y(ticks, campo, n, direction);
    if (y.size() < 2) return std::nullopt;

    int N = y.size();
    std::vector<double> x(N);
    std::iota(x.begin(), x.end(), 0.0);

    double sum_x = std::accumulate(x.begin(), x.end(), 0.0);
    double sum_y = std::accumulate(y.begin(), y.end(), 0.0);
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
    auto y = prepare_y(values, n, direction);
    if (y.size() < 2) return std::nullopt;

    int N = y.size();
    std::vector<double> x(N);
    std::iota(x.begin(), x.end(), 0.0);

    double sum_x = std::accumulate(x.begin(), x.end(), 0.0);
    double sum_y = std::accumulate(y.begin(), y.end(), 0.0);
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

