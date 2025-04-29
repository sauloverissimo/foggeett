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
        .def(py::init<>())
        .def_readwrite("acro", &KPIResult::acro)
        .def_readwrite("value", &KPIResult::value)
        .def_readwrite("description", &KPIResult::description)
        .def_readwrite("debug", &KPIResult::debug)
        .def("to_dict", [](const KPIResult& self) {
            py::dict d;
            d["acro"] = self.acro;
            d["value"] = self.value;
            d["description"] = self.description;
            d["debug"] = self.debug;
            return d;
        })
        .def("__repr__", [](const KPIResult& self) {
            std::string repr = "{";
            repr += "'acro': '" + self.acro + "', ";
            repr += "'value': " + std::to_string(self.value) + ", ";
            repr += "'description': '" + self.description + "'";
            repr += "}";
            return repr;
        });
        

    m.def("slope", &slope, py::arg("ticks"), py::arg("campo"), py::arg("n"));
    m.def("sma", &sma, py::arg("ticks"), py::arg("campo"), py::arg("n"));
    m.def("enrich", &enrich, py::arg("ticks"), py::arg("kpis"));
}
