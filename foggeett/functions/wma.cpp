#include "wma.hpp"
#include "utils.hpp"
#include "functions.hpp"
#include <numeric>
#include <algorithm>

// WMA usando std::vector<Tick>
std::optional<KPIResult> wma(
    const std::vector<Tick>& ticks,
    const std::string& campo,
    std::optional<int> n,
    const std::string& direction) {
    
    auto y = prepare_y(ticks, campo, n, direction);
    if (y.empty()) return std::nullopt;

    // Create weights vector where the most recent value has the highest weight
    std::vector<int> weights(y.size());
    int nval = y.size();
    for (int i = 0; i < nval; ++i) {
        weights[i] = nval - i;
    }

    double weighted_sum = 0.0;
    double weight_sum = 0.0;
    for (size_t i = 0; i < y.size(); ++i) {
        weighted_sum += y[i] * weights[i];
        weight_sum += weights[i];
    }

    double wma_value = weight_sum != 0 ? weighted_sum / weight_sum : 0.0;

    KPIResult r;
    r.acro = "WMA_" + campo.substr(0, 3);
    r.value = wma_value;
    r.description = "Weighted Moving Average of " + campo + " over last " + std::to_string(y.size()) + " points";
    r.debug = {
        {"sum_weights", weight_sum},
        {"sum_weighted_values", weighted_sum}
    };
    return r;
}

// WMA usando std::vector<double>
std::optional<KPIResult> wma(
    const std::vector<double>& values,
    std::optional<int> n,
    const std::string& direction) {
    
    auto y = prepare_y(values, n, direction);
    if (y.empty()) return std::nullopt;

    // Create weights vector where the most recent value has the highest weight
    std::vector<int> weights(y.size());
    int nval = y.size();
    for (int i = 0; i < nval; ++i) {
        weights[i] = nval - i;
    }

    // Calculate the weighted moving average
    double weighted_sum = 0.0;
    double weight_sum = 0.0;
    for (size_t i = 0; i < y.size(); ++i) {
        weighted_sum += y[i] * weights[i];
        weight_sum += weights[i];
    }

    double wma_value = weight_sum != 0 ? weighted_sum / weight_sum : 0.0;

    KPIResult r;
    r.acro = "WMA_VEC";
    r.value = wma_value;
    r.description = "Weighted Moving Average of vector values over last " + std::to_string(y.size()) + " points";
    r.debug = {
        {"sum_weights", weight_sum},
        {"sum_weighted_values", weighted_sum}
    };
    return r;
}
