#include "functions.hpp"
#include "macd.hpp"
#include <map>
#include <string>
#include <optional>

std::map<std::string, double> enrich(const std::vector<Tick>& ticks,
                                     const std::map<std::string, KPIConfig>& kpis) {
    std::map<std::string, double> results;

    for (const auto& [key, cfg] : kpis) {
        std::optional<KPIResult> r;
        std::optional<int> window =
            cfg.params.count("n") ? std::optional<int>(static_cast<int>(cfg.params.at("n"))) : std::nullopt;
        std::string direction =
            cfg.params.count("direction") ? (cfg.params.at("direction") == 1 ? "asc" : "desc") : "desc";

        if (cfg.function == "slope") {
            r = slope(ticks, cfg.campo, window, direction);
        }

        else if (cfg.function == "sma") {
            r = sma(ticks, cfg.campo, window, direction);
        }

        else if (cfg.function == "wma") {
            r = wma(ticks, cfg.campo, window, direction);
        }

        else if (cfg.function == "ema") {
            r = ema(ticks, cfg.campo, window, direction);
        }

        else if (cfg.function == "mom") {
            r = mom(ticks, cfg.campo, window, direction);
        }

        else if (cfg.function == "roc") {
            r = roc(ticks, cfg.campo, window, direction);
        }

        else if (cfg.function == "obv") {
            r = obv(ticks, cfg.campo, window, direction);
        }

        else if (cfg.function == "macd") {
            int short_p = cfg.params.count("short") ? static_cast<int>(cfg.params.at("short")) : 12;
            int long_p  = cfg.params.count("long")  ? static_cast<int>(cfg.params.at("long"))  : 26;
            std::optional<int> signal_p = cfg.params.count("signal") ? std::optional<int>(static_cast<int>(cfg.params.at("signal"))) : std::nullopt;
            r = macd(ticks, cfg.campo, short_p, long_p, signal_p, direction);
        }
        
        if (r.has_value()) {
            results[key] = r->value;
        }
    }

    return results;
}
