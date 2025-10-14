#include "rsi.hpp"
#include "utils.hpp"
#include "functions.hpp"
#include <numeric>
#include <algorithm>
#include <cctype>

// Helper function to convert string to uppercase
std::string to_upper_rsi(const std::string& str) {
    std::string result = str;
    std::transform(result.begin(), result.end(), result.begin(), ::toupper);
    return result;
}

// Internal compute function
std::optional<KPIResult> compute_rsi(
    const std::vector<double>& values,
    int n,
    const std::string& field,
    int window) {

    if (values.size() <= static_cast<size_t>(n))
        return std::nullopt;

    // Cálculo inicial de avg_gain e avg_loss com os primeiros n valores
    std::vector<double> gains, losses;
    for (size_t i = 1; i <= static_cast<size_t>(n); ++i) {
        double delta = values[i] - values[i - 1];
        if (delta > 0) {
            gains.push_back(delta);
        } else if (delta < 0) {
            losses.push_back(std::abs(delta));
        }
    }

    double avg_gain = gains.empty() ? 0.0 : std::accumulate(gains.begin(), gains.end(), 0.0) / n;
    double avg_loss = losses.empty() ? 0.0 : std::accumulate(losses.begin(), losses.end(), 0.0) / n;

    // Suavização exponencial a partir do n+1
    for (size_t i = n + 1; i < values.size(); ++i) {
        double delta = values[i] - values[i - 1];
        double gain = std::max(delta, 0.0);
        double loss = std::max(-delta, 0.0);
        avg_gain = (avg_gain * (n - 1) + gain) / n;
        avg_loss = (avg_loss * (n - 1) + loss) / n;
    }

    // Cálculo do RSI
    double rsi, rs;
    if (avg_loss == 0) {
        rsi = 100.0;
        rs = -1.0; // Valor especial para indicar infinito
    } else {
        rs = avg_gain / avg_loss;
        rsi = 100 - (100 / (1 + rs));
    }

    KPIResult r;
    
    // Build sigla
    if (!field.empty()) {
        r.acro = "RSI_" + to_upper_rsi(field) + "_" + std::to_string(n);
    } else {
        r.acro = "RSI_" + std::to_string(n);
    }
    
    r.value = rsi;
    r.description = "RSI suavizado (n=" + std::to_string(n) + ") com janela de " + 
                   std::to_string(window > 0 ? window : static_cast<int>(values.size())) + " candles";
    
    // Build debug info
    r.debug = {
        {"avg_gain", avg_gain},
        {"avg_loss", avg_loss},
        {"rs", rs}
    };
    
    // Add últimos 5 valores como campos separados
    size_t start_idx = values.size() >= 5 ? values.size() - 5 : 0;
    for (size_t i = start_idx; i < values.size(); ++i) {
        std::string key = "ultimo_valor_" + std::to_string(i - start_idx + 1);
        r.debug[key] = values[i];
    }

    return r;
}

// RSI com Ticks (overload 1)
std::optional<KPIResult> rsi(
    const std::vector<Tick>& ticks,
    const std::string& field,
    int n,
    int window) {

    // Seleciona a janela mais recente (reversed para ficar como no Python)
    std::vector<Tick> dados = ticks;
    std::reverse(dados.begin(), dados.end()); // reversed
    
    size_t take_count = std::min(static_cast<size_t>(window), dados.size());
    dados.resize(take_count);

    // Extract field values
    std::vector<double> values;
    for (const auto& t : dados) {
        auto it = t.find(field);
        if (it != t.end()) {
            values.push_back(it->second);
        }
    }

    return compute_rsi(values, n, field, window);
}

// RSI com vetor direto (overload 2)
std::optional<KPIResult> rsi(
    const std::vector<double>& values,
    int n,
    int window) {

    // Seleciona a janela mais recente
    std::vector<double> windowed_values = values;
    std::reverse(windowed_values.begin(), windowed_values.end()); // reversed
    
    size_t take_count = std::min(static_cast<size_t>(window), windowed_values.size());
    windowed_values.resize(take_count);

    return compute_rsi(windowed_values, n, "", window);
}

// RSI com parametros adicionais - Python-like interface (overload 3)
std::optional<KPIResult> rsi(
    const std::vector<Tick>& ticks,
    const std::string& field,
    int n,
    int window,
    const std::string& source) {

    // Seleciona a janela mais recente (reversed para ficar como no Python)
    std::vector<Tick> dados = ticks;
    std::reverse(dados.begin(), dados.end()); // reversed
    
    size_t take_count = std::min(static_cast<size_t>(window), dados.size());
    dados.resize(take_count);

    // Extract field values
    std::vector<double> values;
    for (const auto& t : dados) {
        auto it = t.find(field);
        if (it != t.end()) {
            values.push_back(it->second);
        }
    }

    return compute_rsi(values, n, field, window);
}