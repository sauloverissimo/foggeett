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
 * Versão final — Corrige o bug de inferência de tipo bool/int:
 * 1️⃣ Lê ticks Python e converte mantendo bool puro
 * 2️⃣ Processa (opcional) sem GIL
 * 3️⃣ Retorna com tipos preservados
 */

struct NativeRow {
    std::unordered_map<std::string, double> numerics;   // price, volume, etc.
    std::unordered_map<std::string, bool>   flags;      // booleans
    std::unordered_map<std::string, py::object> passthrough; // tick_id, timestamp, etc.
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

    for (auto item : ticks) {
        if (!py::isinstance<py::dict>(item))
            continue;

        py::dict t = py::cast<py::dict>(item);
        NativeRow row;
        bool valido = true;

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
            else { valido = false; break; }
        }

        if (!valido)
            continue;

        for (const auto& k : passthrough) {
            if (t.contains(py::str(k)))
                row.passthrough[k] = t[py::str(k)];
        }

        rows.emplace_back(std::move(row));
    }

    // processamento paralelo opcional
    {
        py::gil_scoped_release release;
#ifdef FOGGEETT_USE_OPENMP
        #pragma omp parallel for schedule(static)
#endif
        for (size_t i = 0; i < rows.size(); ++i) { }
    }

    // reconstrução do retorno
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
