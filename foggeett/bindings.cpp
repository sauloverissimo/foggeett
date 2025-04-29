#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include "functions.hpp"

namespace py = pybind11;

PYBIND11_MODULE(_core, m) {
    py::class_<KPIConfig>(m, "KPIConfig")
        .def(py::init<const std::string&, const std::string&, const std::map<std::string, float>&>(),
             py::arg("function"), py::arg("campo"), py::arg("params"))
        .def_readwrite("function", &KPIConfig::function)
        .def_readwrite("campo", &KPIConfig::campo)
        .def_readwrite("params", &KPIConfig::params);

    py::class_<KPIResult>(m, "KPIResult")
        .def_readwrite("sigla", &KPIResult::sigla)
        .def_readwrite("valor", &KPIResult::valor)
        .def_readwrite("descricao", &KPIResult::descricao)
        .def_readwrite("debug", &KPIResult::debug);

    m.def("slope", &slope, py::arg("ticks"), py::arg("campo"), py::arg("n"));
    m.def("sma", &sma, py::arg("ticks"), py::arg("campo"), py::arg("n"));
    m.def("enrich", &enrich, py::arg("ticks"), py::arg("kpis"));
}
