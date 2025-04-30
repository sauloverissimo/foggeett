#include "obv.hpp"
#include "utils.hpp"
#include <numeric>



std::optional<KPIResult> obv(
    const std::vector<Tick>& ticks,
    const std::string& volume_field,
    std::optional<int> n,
    const std::string& direction) {

    std::vector<Tick> dados = prepare_ticks(ticks, n, direction);

    if (dados.size() < 2)
        return std::nullopt;

    // Verificação rigorosa: todos os ticks precisam ter os campos exigidos
    for (const auto& t : dados) {
        if (t.count("price") == 0 || t.count(volume_field) == 0)
            return std::nullopt;
    }

    double obv_value = 0.0;

    for (size_t i = 1; i < dados.size(); ++i) {
        double preco_atual = dados[i].at("price");
        double preco_anterior = dados[i - 1].at("price");
        double volume = dados[i].at(volume_field);

        if (preco_atual > preco_anterior) {
            obv_value += volume;
        } else if (preco_atual < preco_anterior) {
            obv_value -= volume;
        }
    }

    KPIResult r;
    r.acro = "OBV_" + volume_field.substr(0, 3) + (n ? "_" + std::to_string(*n) : "");
    r.value = obv_value;
    r.description = "On-Balance Volume using '" + volume_field + "'";
    r.debug = { {"ticks_used", static_cast<double>(dados.size())} };

    return r;
}

