// macd.cpp

#include "macd.hpp"
#include "utils.hpp"
#include "functions.hpp"
#include <numeric>
#include <algorithm>
#include <cctype>

// Helper function to convert string to uppercase
std::string to_upper(const std::string& str) {
    std::string result = str;
    std::transform(result.begin(), result.end(), result.begin(), ::toupper);
    return result;
}

// Função interna renomeada para evitar ambiguidade
std::optional<KPIResult> compute_macd(
    const std::vector<double>& y,
    int short_period,
    int long_period,
    int signal_period,
    const std::string& field,
    int n,
    const std::string& direction) {

    if (y.size() < static_cast<size_t>(long_period + signal_period))
        return std::nullopt;

    // EMA Binance-style: EMA₀ = first value, returns vector with same size as input
    auto ema_binance = [](const std::vector<double>& data, int period) {
        std::vector<double> result;
        if (data.empty()) return result;
        
        double alpha = 2.0 / (period + 1.0);
        double ema = data[0];  // EMA₀ = first value (Binance style)
        result.push_back(ema);
        
        for (size_t i = 1; i < data.size(); ++i) {
            ema = alpha * data[i] + (1.0 - alpha) * ema;
            result.push_back(ema);
        }
        
        return result;
    };

    // Calculate EMAs
    std::vector<double> ema_short = ema_binance(y, short_period);
    std::vector<double> ema_long = ema_binance(y, long_period);

    // Align by the end (take minimum length)
    size_t min_len = std::min(ema_short.size(), ema_long.size());
    std::vector<double> macd_line;
    for (size_t i = ema_short.size() - min_len; i < ema_short.size(); ++i) {
        size_t long_idx = ema_long.size() - min_len + (i - (ema_short.size() - min_len));
        macd_line.push_back(ema_short[i] - ema_long[long_idx]);
    }

    // Calculate signal line (EMA of MACD)
    std::vector<double> signal_line = ema_binance(macd_line, signal_period);
    if (signal_line.empty()) {
        return std::nullopt;
    }

    // Final values (most recent)
    double macd_val = macd_line.back();
    double signal_val = signal_line.back();
    double hist = macd_val - signal_val;

    KPIResult r;
    
    // Build sigla based on whether field is provided
    if (!field.empty()) {
        r.acro = "MACDSIGNAL_" + to_upper(field) + "_" + 
                 std::to_string(short_period) + "_" +
                 std::to_string(long_period) + "_" + 
                 std::to_string(signal_period);
    } else {
        r.acro = "MACDSIGNAL_" + std::to_string(short_period) + "_" +
                 std::to_string(long_period) + "_" + 
                 std::to_string(signal_period);
    }
    
    r.value = macd_val;
    r.description = "MACD estilo Binance (EMA₀ = y[0], ordem cronológica)";
    
    // Build debug info
    r.debug = {
        {"MACD", macd_val},
        {"SIGNAL", signal_val},
        {"HISTOGRAM", hist}
    };
    
    // Add últimos 5 valores como campos separados
    size_t start_idx = y.size() >= 5 ? y.size() - 5 : 0;
    for (size_t i = start_idx; i < y.size(); ++i) {
        std::string key = "ultimo_valor_" + std::to_string(i - start_idx + 1);
        r.debug[key] = y[i];
    }
    
    // Add n if provided
    if (n > 0) {
        r.debug["n"] = static_cast<double>(n);
    }
    
    // Convert direction to numeric value for debug
    if (!direction.empty()) {
        r.debug["direction_desc"] = (direction == "desc") ? 1.0 : 0.0;
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

    // Use signal_period=9 as default if not provided
    int actual_signal_period = signal_period.value_or(9);

    // 1. Select all ticks (no n parameter in this overload)
    std::vector<Tick> dados = ticks;

    // 2. Ensure chronological order (oldest to newest) for correct calculation
    if (direction == "desc") {
        std::reverse(dados.begin(), dados.end());  // reverse to chronological
    }

    // 3. Extract field values
    std::vector<double> y;
    for (const auto& t : dados) {
        auto it = t.find(field);
        if (it != t.end()) {
            y.push_back(it->second);
        }
    }

    return compute_macd(y, short_period, long_period, actual_signal_period, field, 0, direction);
}

// Double vector wrapper
std::optional<KPIResult> macd(
    const std::vector<double>& values,
    int short_period,
    int long_period,
    std::optional<int> signal_period,
    const std::string& direction) {

    // Use signal_period=9 as default if not provided
    int actual_signal_period = signal_period.value_or(9);

    // 1. Take values as provided
    std::vector<double> y = values;

    // 2. Ensure chronological order for calculation
    if (direction == "desc") {
        std::reverse(y.begin(), y.end());  // reverse to chronological
    }

    return compute_macd(y, short_period, long_period, actual_signal_period, "", 0, direction);
}

// Third overload with n and source parameters
std::optional<KPIResult> macd(
    const std::vector<Tick>& ticks,
    const std::string& field,
    int short_period,
    int long_period,
    int signal_period,
    int n,
    const std::string& source,
    const std::string& direction) {

    // 1. Select the n most recent candles (most recent first)
    std::vector<Tick> dados;
    size_t take_count = std::min(static_cast<size_t>(n), ticks.size());
    dados.assign(ticks.begin(), ticks.begin() + take_count);

    // 2. Ensure chronological order (oldest to newest) for correct calculation
    if (direction == "desc") {
        std::reverse(dados.begin(), dados.end());  // reverse to chronological
    }

    // 3. Extract field values
    std::vector<double> y;
    for (const auto& t : dados) {
        auto it = t.find(field);
        if (it != t.end()) {
            y.push_back(it->second);
        }
    }

    return compute_macd(y, short_period, long_period, signal_period, field, n, direction);
}

