#include "extract.hpp"
#include <pybind11/pytypes.h>
#include <iostream>

namespace py = pybind11;

/**
 * Versão nativa em C++ da função extract_crypto_fields.
 * Extrai campos numéricos (float/int/bool) e adiciona campos extras (passthrough).
 */
std::vector<std::unordered_map<std::string, py::object>>
extract_fields_native(
    const py::list& ticks,
    const std::vector<std::string>& fields,
    const std::vector<std::string>& passthrough,
    bool /*include_all_numeric*/
) {
    std::vector<std::unordered_map<std::string, py::object>> resultado;

    for (auto item : ticks) {
        if (!py::isinstance<py::dict>(item))
            continue;

        py::dict t = py::cast<py::dict>(item);
        std::unordered_map<std::string, py::object> tick_filtrado;
        bool valido = true;

        // 🔹 Validação dos campos obrigatórios
        for (const auto& k : fields) {
            if (!t.contains(py::str(k))) {
                valido = false;
                break;
            }

            py::object val = t[py::str(k)];

            if (!(py::isinstance<py::float_>(val) ||
                  py::isinstance<py::int_>(val) ||
                  py::isinstance<py::bool_>(val))) {
                valido = false;
                break;
            }

            tick_filtrado[k] = val;
        }

        if (!valido)
            continue;

        // 🔹 Adiciona campos extras (passthrough)
        for (const auto& k : passthrough) {
            if (t.contains(py::str(k))) {
                tick_filtrado[k] = t[py::str(k)];
            }
        }

        resultado.push_back(std::move(tick_filtrado));
    }

    return resultado;
}
