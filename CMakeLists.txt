cmake_minimum_required(VERSION 3.23)
project(foggeett LANGUAGES CXX)

# Diz ao pybind11 para detectar o Python correto
set(PYBIND11_FINDPYTHON ON)

find_package(pybind11 CONFIG REQUIRED)   # fornecido via pip
pybind11_add_module(_core  # nome final
    foggeett/bindings.cpp
    foggeett/functions.cpp
)

target_include_directories(_core PRIVATE foggeett)
target_compile_features(_core PRIVATE cxx_std_17)

# Instala no mesmo diretório do pacote
install(TARGETS _core DESTINATION foggeett)
