#include "adx.hpp"
#include "utils.hpp"
#include <cmath>
#include <numeric>
#include <sstream>
#include <optional>
#include <algorithm>


std::optional<KPIResult> adx(const std::vector<Tick>& ticks,
                             const std::string& campo,
                             std::optional<int> n,
                             const std::string& direction) {
    std::vector<Tick> dados = ticks;
    if (direction == "asc") std::reverse(dados.begin(), dados.end());

    int n_periodos = n.value_or(14);
    if (dados.size() < static_cast<size_t>(n_periodos + 1)) return std::nullopt;

    std::vector<double> tr_list, plus_dm_list, minus_dm_list;

    for (size_t i = 1; i < dados.size(); ++i) {
        const auto& curr = dados[i];
        const auto& prev = dados[i - 1];

        double high = curr.at("high");
        double low = curr.at("low");
        double close_prev = prev.at("price");
        double high_prev = prev.at("high");
        double low_prev = prev.at("low");

        double tr = std::max({ high - low, std::abs(high - close_prev), std::abs(low - close_prev) });
        double plus_dm = (high - high_prev > low_prev - low && high - high_prev > 0) ? high - high_prev : 0.0;
        double minus_dm = (low_prev - low > high - high_prev && low_prev - low > 0) ? low_prev - low : 0.0;

        tr_list.push_back(tr);
        plus_dm_list.push_back(plus_dm);
        minus_dm_list.push_back(minus_dm);
    }

    size_t m = tr_list.size();
    if (m < static_cast<size_t>(n_periodos)) return std::nullopt;

    std::vector<double> dx_list;
    for (size_t i = n_periodos - 1; i < m; ++i) {
        double tr_sum = std::accumulate(tr_list.begin() + i + 1 - n_periodos, tr_list.begin() + i + 1, 0.0);
        double plus_dm_sum = std::accumulate(plus_dm_list.begin() + i + 1 - n_periodos, plus_dm_list.begin() + i + 1, 0.0);
        double minus_dm_sum = std::accumulate(minus_dm_list.begin() + i + 1 - n_periodos, minus_dm_list.begin() + i + 1, 0.0);

        double plus_di = tr_sum != 0.0 ? 100.0 * plus_dm_sum / tr_sum : 0.0;
        double minus_di = tr_sum != 0.0 ? 100.0 * minus_dm_sum / tr_sum : 0.0;

        double dx = (plus_di + minus_di != 0.0)
            ? 100.0 * std::abs(plus_di - minus_di) / (plus_di + minus_di)
            : 0.0;

        dx_list.push_back(dx);
    }

    if (dx_list.empty()) return std::nullopt;

    // Suavização tipo EMA (igual ao Pandas adjust=False)
    double alpha = 2.0 / (n_periodos + 1);
    double ema = dx_list[0];
    for (size_t i = 1; i < dx_list.size(); ++i) {
        ema = alpha * dx_list[i] + (1.0 - alpha) * ema;
    }

    // DEBUG: printar últimos 50 valores de dx
    std::ostringstream dx_stream;
    size_t limit = std::min(dx_list.size(), size_t(50));
    for (size_t i = dx_list.size() - limit; i < dx_list.size(); ++i) {
        dx_stream << dx_list[i];
        if (i < dx_list.size() - 1) dx_stream << ", ";
    }

    KPIResult r;
    r.acro = "ADX_" + campo.substr(0, 3);
    r.value = ema;
    r.description = "ADX suavizado com EMA(" + std::to_string(n_periodos) + ") | dx_list: [" + dx_stream.str() + "]";
    r.debug = {
        {"final_adx", ema},
        {"ultimo_dx", dx_list.back()},
        {"alpha", alpha},
        {"n", static_cast<double>(n_periodos)},
        {"qtd_dx", static_cast<double>(dx_list.size())}
    };

    return r;
}
