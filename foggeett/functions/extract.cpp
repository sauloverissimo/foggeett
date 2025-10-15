#include "extract.hpp"
#include <pybind11/pytypes.h>
#include <pybind11/gil.h>
#include <unordered_map>
#include <vector>
#include <string>

#ifdef FOGGEETT_USE_OPENMP
#include <omp.h>
#endif

namespace py = pybind11;

/*
 * 🔹 Versão revisada (Foggeett 2025)
 * 
 * Objetivos:
 * 1️⃣ Corrige o bug de meta-KPIs — campos como "rsi_24" e "macd_26"
 *     agora são lidos como valores numéricos se estiverem no passthrough.
 * 2️⃣ Mantém compatibilidade total com uso anterior (ticks, flags, etc.).
 * 3️⃣ Preserva tipos bool/int/float e paralelização opcional com OpenMP.
 */

struct NativeRow {
    std::unordered_map<std::string, double> numerics;   // price, volume, rsi_24, etc.
    std::unordered_map<std::string, bool>   flags;      // booleans
    std::unordered_map<std::string, py::object> passthrough; // metadados (tick_id, timestamp, etc.)
};

std::vector<std::unordered_map<std::string, py::object>>
extract_fields_native(
    const py::list& ticks,
    const std::vector<std::string>& fields,
    const std::vector<std::string>& passthrough,
    bool /*include_all_numeric*/
) {
    std::vector<NativeRow> rows;
    rows.reserve(py::len(ticks));

    // ============================================================
    // 🔹 Leitura dos ticks
    // ============================================================
    for (auto item : ticks) {
        if (!py::isinstance<py::dict>(item))
            continue;

        py::dict t = py::cast<py::dict>(item);
        NativeRow row;
        bool valido = true;

        // ------------------------------------------------------------
        // 1️⃣ Extrai campos principais (fields)
        // ------------------------------------------------------------
        for (const auto& k : fields) {
            if (!t.contains(py::str(k))) { valido = false; break; }
            py::object val = t[py::str(k)];

            // ⚠️ ORDEM IMPORTANTE: testa bool antes de int/float
            if (py::isinstance<py::bool_>(val)) {
                try { row.flags[k] = py::cast<bool>(val); }
                catch (...) { valido = false; break; }
            }
            else if (py::isinstance<py::float_>(val) || py::isinstance<py::int_>(val)) {
                try { row.numerics[k] = py::cast<double>(val); }
                catch (...) { valido = false; break; }
            }
            else {
                // ignora campos não numéricos (ex: strings)
                valido = false; break;
            }
        }

        if (!valido)
            continue;

        // ------------------------------------------------------------
        // 2️⃣ Extrai campos passthrough — agora com suporte numérico!
        // ------------------------------------------------------------
        for (const auto& k : passthrough) {
            if (!t.contains(py::str(k))) continue;

            py::object val = t[py::str(k)];

            // 🔹 Se o passthrough for numérico, adiciona também em numerics
            if (py::isinstance<py::float_>(val) || py::isinstance<py::int_>(val)) {
                try {
                    row.numerics[k] = py::cast<double>(val);
                } catch (...) {
                    row.passthrough[k] = val;
                }
            }
            // 🔹 Se for bool, adiciona em flags
            else if (py::isinstance<py::bool_>(val)) {
                try {
                    row.flags[k] = py::cast<bool>(val);
                } catch (...) {
                    row.passthrough[k] = val;
                }
            }
            // 🔹 Caso contrário, mantém como passthrough padrão (metadados)
            else {
                row.passthrough[k] = val;
            }
        }

        rows.emplace_back(std::move(row));
    }

    // ============================================================
    // 🔹 (Opcional) processamento paralelo sem GIL
    // ============================================================
    {
        py::gil_scoped_release release;
#ifdef FOGGEETT_USE_OPENMP
        #pragma omp parallel for schedule(static)
#endif
        for (size_t i = 0; i < rows.size(); ++i) {
            // Nada a processar por enquanto — reservado para validações futuras
        }
    }

    // ============================================================
    // 🔹 Reconstrução do retorno (para Python)
    // ============================================================
    std::vector<std::unordered_map<std::string, py::object>> resultado;
    resultado.reserve(rows.size());

    for (auto& r : rows) {
        std::unordered_map<std::string, py::object> out;

        for (const auto& kv : r.numerics)
            out[kv.first] = py::float_(kv.second);

        for (const auto& kv : r.flags)
            out[kv.first] = py::bool_(kv.second);

        for (auto& kv : r.passthrough)
            out[kv.first] = kv.second;

        resultado.emplace_back(std::move(out));
    }

    return resultado;
}
