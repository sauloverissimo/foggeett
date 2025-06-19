#include "macd.hpp"
#include "utils.hpp"
#include "functions.hpp"
#include <numeric>
#include <algorithm>



// Função interna renomeada para evitar ambiguidade
std::optional<KPIResult> compute_macd(
    const std::vector<double>& y,
    int short_period,
    int long_period,
    std::optional<int> signal_period) {

    if (y.size() < static_cast<size_t>(long_period + signal_period.value_or(0)))
        return std::nullopt;

    auto ema_series = [](const std::vector<double>& data, int span) {
        std::vector<double> result(data.size());
        if (data.empty()) return result;

        double alpha = 2.0 / (span + 1.0);
        result[0] = data[0];

        for (size_t i = 1; i < data.size(); ++i)
            result[i] = alpha * data[i] + (1.0 - alpha) * result[i - 1];

        return result;
    };

    std::vector<double> ema_short = ema_series(y, short_period);
    std::vector<double> ema_long  = ema_series(y, long_period);

    std::vector<double> macd_line(y.size());
    for (size_t i = 0; i < y.size(); ++i)
        macd_line[i] = ema_short[i] - ema_long[i];

    double macd_val = macd_line.back();

    KPIResult r;
    if (!signal_period.has_value()) {
        r.acro = "MACD_" + std::to_string(short_period) + "_" + std::to_string(long_period);
        r.value = macd_val;
        r.description = "MACD (EMA " + std::to_string(short_period) + " - EMA " + std::to_string(long_period) + ")";
        r.debug = { {"MACD", macd_val} };
    } else {
        std::vector<double> signal_line = ema_series(macd_line, signal_period.value());
        double signal_val = signal_line.back();
        double hist = macd_val - signal_val;

        r.acro = "MACDSIGNAL_" + std::to_string(short_period) + "_" +
                 std::to_string(long_period) + "_" + std::to_string(signal_period.value());
        r.value = macd_val;
        r.description = "MACD with signal (" + std::to_string(signal_period.value()) + ") and histogram";
        r.debug = {
            {"MACD", macd_val},
            {"SIGNAL", signal_val},
            {"HISTOGRAM", hist}
        };
    }

    return r;
}

// Tick-based wrapper
std::optional<KPIResult> macd(
    const std::vector<Tick>& ticks,
    const std::string& field,
    int short_period,
    int long_period,
    std::optional<int> signal_period,
    const std::string& direction) {

    auto y = prepare_y(ticks, field, std::nullopt, direction);
    return compute_macd(y, short_period, long_period, signal_period);
}

// Double vector wrapper
std::optional<KPIResult> macd(
    const std::vector<double>& values,
    int short_period,
    int long_period,
    std::optional<int> signal_period,
    const std::string& direction) {

    auto y = prepare_y(values, std::nullopt, direction);
    return compute_macd(y, short_period, long_period, signal_period);
}



