#include "ema.hpp"
#include "utils.hpp"
#include "functions.hpp"


std::optional<KPIResult> ema(
    const std::vector<Tick>& ticks,
    const std::string& field,
    std::optional<int> n,
    const std::string& direction) {

    auto y = prepare_y(ticks, field, n, direction);
    if (y.empty()) return std::nullopt;

    double alpha = 2.0 / (y.size() + 1);
    double value = y[0];
    for (size_t i = 1; i < y.size(); ++i)
        value = alpha * y[i] + (1.0 - alpha) * value;

    KPIResult r;
    r.acro = "EMA_" + field.substr(0, 3);
    r.value = value;
    r.description = "Exponential Moving Average of " + field + " over last " + std::to_string(y.size()) + " points";
    r.debug = { {"alpha", alpha}, {"first_value", y[0]}, {"last_value", y.back()} };

    return r;
}

std::optional<KPIResult> ema(
    const std::vector<double>& values,
    std::optional<int> n,
    const std::string& direction) {

    auto y = prepare_y(values, n, direction);
    if (y.empty()) return std::nullopt;

    double alpha = 2.0 / (y.size() + 1);
    double value = y[0];
    for (size_t i = 1; i < y.size(); ++i)
        value = alpha * y[i] + (1.0 - alpha) * value;

    KPIResult r;
    r.acro = "EMA_VEC";
    r.value = value;
    r.description = "Exponential Moving Average of vector values over last " + std::to_string(y.size()) + " points";
    r.debug = { {"alpha", alpha}, {"first_value", y[0]}, {"last_value", y.back()} };

    return r;
}
