#include "functions.hpp"
#include "macd.hpp"
#include <map>
#include <string>
#include <optional>
#include <variant>
#include <algorithm>
#include <algorithm>


std::map<std::string, double> enrich(const std::vector<Tick>& ticks,
                                     const std::map<std::string, KPIConfig>& kpis) {
    std::map<std::string, double> results;

    for (const auto& [key, cfg] : kpis) {
        std::optional<KPIResult> r;

        // Janela (n)
        std::optional<int> window = std::nullopt;
        if (cfg.params.count("n")) {
            const auto& val = cfg.params.at("n");
            if (std::holds_alternative<int>(val)) {
                window = std::get<int>(val);
            } else if (std::holds_alternative<double>(val)) {
                window = static_cast<int>(std::get<double>(val));
            }
        }

        // Direção (asc/desc)
        std::string direction = "desc";
        if (cfg.params.count("direction")) {
            const auto& val = cfg.params.at("direction");

            if (std::holds_alternative<std::string>(val)) {
                std::string d = std::get<std::string>(val);
                std::transform(d.begin(), d.end(), d.begin(), ::tolower);
                if (d == "asc" || d == "1") direction = "asc";
            } else if (std::holds_alternative<int>(val)) {
                direction = (std::get<int>(val) == 1) ? "asc" : "desc";
            } else if (std::holds_alternative<double>(val)) {
                direction = (static_cast<int>(std::get<double>(val)) == 1) ? "asc" : "desc";
            }
        }

        // Chamada de função
        if (cfg.function == "slope") {
            r = slope(ticks, cfg.campo, window, direction);
        } else if (cfg.function == "sma") {
            r = sma(ticks, cfg.campo, window, direction);
        } else if (cfg.function == "wma") {
            r = wma(ticks, cfg.campo, window, direction);
        } else if (cfg.function == "ema") {
            r = ema(ticks, cfg.campo, window, direction);
        } else if (cfg.function == "mom") {
            r = mom(ticks, cfg.campo, window, direction);
        } else if (cfg.function == "roc") {
            r = roc(ticks, cfg.campo, window, direction);
        } else if (cfg.function == "obv") {
            r = obv(ticks, cfg.campo, window, direction);
        } else if (cfg.function == "adx") {
            r = adx(ticks, cfg.campo, window, direction);
        } else if (cfg.function == "macd") {
            int short_p = cfg.params.count("short") ? static_cast<int>(std::get<double>(cfg.params.at("short"))) : 12;
            int long_p  = cfg.params.count("long")  ? static_cast<int>(std::get<double>(cfg.params.at("long")))  : 26;
            std::optional<int> signal_p = cfg.params.count("signal")
                                            ? std::optional<int>(static_cast<int>(std::get<double>(cfg.params.at("signal"))))
                                            : std::nullopt;
            r = macd(ticks, cfg.campo, short_p, long_p, signal_p, direction);
        }

        if (r.has_value()) {
            results[key] = r->value;
        }
    }

    return results;
}


// #include "functions.hpp"
// #include "macd.hpp"
// #include <map>
// #include <string>
// #include <optional>

// std::map<std::string, double> enrich(const std::vector<Tick>& ticks,
//                                      const std::map<std::string, KPIConfig>& kpis) {
//     std::map<std::string, double> results;

//     for (const auto& [key, cfg] : kpis) {
//         std::optional<KPIResult> r;
//         std::optional<int> window =
//             cfg.params.count("n") ? std::optional<int>(static_cast<int>(cfg.params.at("n"))) : std::nullopt;
//         std::string direction =
//             cfg.params.count("direction") ? (cfg.params.at("direction") == 1 ? "asc" : "desc") : "desc";

//         if (cfg.function == "slope") {
//             r = slope(ticks, cfg.campo, window, direction);
//         }

//         else if (cfg.function == "sma") {
//             r = sma(ticks, cfg.campo, window, direction);
//         }

//         else if (cfg.function == "wma") {
//             r = wma(ticks, cfg.campo, window, direction);
//         }

//         else if (cfg.function == "ema") {
//             r = ema(ticks, cfg.campo, window, direction);
//         }

//         else if (cfg.function == "mom") {
//             r = mom(ticks, cfg.campo, window, direction);
//         }

//         else if (cfg.function == "roc") {
//             r = roc(ticks, cfg.campo, window, direction);
//         }

//         else if (cfg.function == "obv") {
//             r = obv(ticks, cfg.campo, window, direction);
//         }

//         else if (cfg.function == "adx") {
//             r = adx(ticks, cfg.campo, window, direction);
//         }
        

//         else if (cfg.function == "macd") {
//             int short_p = cfg.params.count("short") ? static_cast<int>(cfg.params.at("short")) : 12;
//             int long_p  = cfg.params.count("long")  ? static_cast<int>(cfg.params.at("long"))  : 26;
//             std::optional<int> signal_p = cfg.params.count("signal") ? std::optional<int>(static_cast<int>(cfg.params.at("signal"))) : std::nullopt;
//             r = macd(ticks, cfg.campo, short_p, long_p, signal_p, direction);
//         }
        
//         if (r.has_value()) {
//             results[key] = r->value;
//         }
//     }

//     return results;
// }
