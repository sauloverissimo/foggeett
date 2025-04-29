#include "enrich.hpp"
#include "slope.hpp"
#include "sma.hpp"
#include "functions.hpp"

std::map<std::string, double> enrich(const std::vector<Tick>& ticks,
                                     const std::map<std::string, KPIConfig>& kpis) {
    std::map<std::string, double> results;
    for (auto const& [key, cfg] : kpis) {
        std::optional<KPIResult> r;
        std::optional<int> window = cfg.params.count("n") ? std::optional<int>(cfg.params.at("n")) : std::nullopt;
        std::string direction = cfg.params.count("direction") ? (cfg.params.at("direction") == 1 ? "asc" : "desc") : "desc";

        if (cfg.function == "slope") {
            r = slope(ticks, cfg.campo, window, direction);
        }
        else if (cfg.function == "sma") {
            r = sma(ticks, cfg.campo, window, direction);
        }
        if (r.has_value()) {
            results[key] = r->value;
        }
    }
    return results;
}
