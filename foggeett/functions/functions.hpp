// functions.hpp
#pragma once

#include <string>
#include <vector>
#include <map>
#include <optional>
#include <variant>

using Tick = std::map<std::string, double>;
using ParamType = std::variant<int, double, std::string>;

struct KPIConfig {
    std::string function;
    std::string campo;
    std::map<std::string, ParamType> params;
};

struct KPIResult {
    std::string acro;
    double value;
    std::string description;
    std::map<std::string, double> debug;
};

// Inclusões dos módulos de funções
#include "functions/utils.hpp"
#include "functions/slope.hpp"
#include "functions/sma.hpp"
#include "functions/wma.hpp"
#include "functions/ema.hpp"
#include "functions/mom.hpp"
#include "functions/roc.hpp"
#include "functions/macd.hpp"
#include "functions/obv.hpp"
#include "functions/adx.hpp"
#include "functions/enrich.hpp"
#include "functions/rsi.hpp"
#include "functions/extract.hpp"

