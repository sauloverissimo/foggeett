#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include "functions/functions.hpp"



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
        })
        .def("to_str", [](const KPIResult& self) {
            return self.acro + ": " + std::to_string(self.value) + " (" + self.description + ")";
        })
        .def("to_json", [](const KPIResult& self) {
            py::dict d;
            d["acro"] = self.acro;
            d["value"] = self.value;
            d["description"] = self.description;
            d["debug"] = self.debug;
            py::object json = py::module_::import("json");
            return json.attr("dumps")(d);
        })
        .def("__eq__", [](const KPIResult& self, const KPIResult& other) {
            return self.acro == other.acro &&
                   self.value == other.value &&
                   self.description == other.description &&
                   self.debug == other.debug;
        });

    // === FUNÇÕES SLOPE ===
    m.def("slope", 
        py::overload_cast<const std::vector<Tick>&, const std::string&, std::optional<int>, const std::string&>(&slope),
        py::arg("ticks"),
        py::arg("campo"),
        py::arg("n") = std::nullopt,
        py::arg("direction") = "desc"
    );

    m.def("slope", 
        py::overload_cast<const std::vector<double>&, std::optional<int>, const std::string&>(&slope),
        py::arg("values"),
        py::arg("n") = std::nullopt,
        py::arg("direction") = "desc"
    );

    // === FUNÇÕES SMA ===
    m.def("sma", 
        py::overload_cast<const std::vector<Tick>&, const std::string&, std::optional<int>, const std::string&>(&sma),
        py::arg("ticks"),
        py::arg("campo"),
        py::arg("n") = std::nullopt,
        py::arg("direction") = "desc"
    );

    m.def("sma", 
        py::overload_cast<const std::vector<double>&, std::optional<int>, const std::string&>(&sma),
        py::arg("values"),
        py::arg("n") = std::nullopt,
        py::arg("direction") = "desc"
    );

    // === FUNÇÃO ENRICH ===
    m.def("enrich", &enrich, py::arg("ticks"), py::arg("kpis"));
}
