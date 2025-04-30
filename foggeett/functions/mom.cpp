#include "mom.hpp"
#include "utils.hpp"
#include "functions.hpp"


std::optional<KPIResult> mom(
    const std::vector<Tick>& ticks,
    const std::string& field,
    std::optional<int> n,
    const std::string& direction) {

    auto y = prepare_y(ticks, field, std::nullopt, direction);
    if (y.size() < 2) return std::nullopt;

    int n_real = n.value_or(y.size() - 1);
    if (n_real >= static_cast<int>(y.size())) return std::nullopt;

    double atual = y.front();
    double passado = y[n_real];
    double value = atual - passado;

    KPIResult r;
    r.acro = "MOM_" + field.substr(0, 3) + "_" + std::to_string(n_real);
    r.value = value;
    r.description = "Difference between current and past " + std::to_string(n_real) + " values of " + field;
    r.debug = { {"current", atual}, {"past", passado}, {"n", static_cast<double>(n_real)} };

    return r;
}

std::optional<KPIResult> mom(
    const std::vector<double>& values,
    std::optional<int> n,
    const std::string& direction) {

    auto y = prepare_y(values, std::nullopt, direction);
    if (y.size() < 2) return std::nullopt;

    int n_real = n.value_or(y.size() - 1);
    if (n_real >= static_cast<int>(y.size())) return std::nullopt;

    double atual = y.front();
    double passado = y[n_real];
    double value = atual - passado;

    KPIResult r;
    r.acro = "MOM_VEC_" + std::to_string(n_real);
    r.value = value;
    r.description = "Difference between current and past " + std::to_string(n_real) + " values in vector";
    r.debug = { {"current", atual}, {"past", passado}, {"n", static_cast<double>(n_real)} };

    return r;
}

