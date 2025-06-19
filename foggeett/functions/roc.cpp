#include "roc.hpp"
#include "utils.hpp"
#include "functions.hpp"
#include <algorithm>



std::optional<KPIResult> roc(
    const std::vector<Tick>& ticks,
    const std::string& field,
    std::optional<int> n,
    const std::string& direction) {

    auto y = prepare_y(ticks, field, std::nullopt, direction);
    if (y.size() < 2) return std::nullopt;

    int n_ticks = n.value_or(y.size() - 1);
    if (n_ticks >= static_cast<int>(y.size())) return std::nullopt;

    double atual = y.front();
    double passado = y[n_ticks];
    if (passado == 0.0) return std::nullopt;

    double rate = ((atual - passado) / passado) * 100.0;

    KPIResult r;
    r.acro = "ROC_" + field.substr(0, 3) + "_" + std::to_string(n_ticks);
    r.value = rate;
    r.description = "Percent change in " + field + " compared to " + std::to_string(n_ticks) + " periods ago";
    r.debug = { {"current", atual}, {"past", passado}, {"n", static_cast<double>(n_ticks)} };

    return r;
}

std::optional<KPIResult> roc(
    const std::vector<double>& values,
    std::optional<int> n,
    const std::string& direction) {

    auto y = prepare_y(values, std::nullopt, direction);
    if (y.size() < 2) return std::nullopt;

    int n_ticks = n.value_or(y.size() - 1);
    if (n_ticks >= static_cast<int>(y.size())) return std::nullopt;

    double atual = y.front();
    double passado = y[n_ticks];
    if (passado == 0.0) return std::nullopt;

    double rate = ((atual - passado) / passado) * 100.0;

    KPIResult r;
    r.acro = "ROC_VEC_" + std::to_string(n_ticks);
    r.value = rate;
    r.description = "Percent change in vector values compared to " + std::to_string(n_ticks) + " periods ago";
    r.debug = { {"current", atual}, {"past", passado}, {"n", static_cast<double>(n_ticks)} };

    return r;
}
