#pragma once
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <pybind11/stl_bind.h>
#include <vector>
#include <string>
#include <unordered_map>

namespace py = pybind11;

/**
 * Extrai campos numéricos, booleanos e passthroughs dos ticks.
 * 
 * - 'fields': lista de campos obrigatórios (numéricos ou booleanos)
 * - 'passthrough': campos opcionais (qualquer tipo)
 */
std::vector<std::unordered_map<std::string, py::object>>
extract_fields_native(
    const py::list& ticks,
    const std::vector<std::string>& fields,
    const std::vector<std::string>& passthrough,
    bool include_all_numeric = false
);
