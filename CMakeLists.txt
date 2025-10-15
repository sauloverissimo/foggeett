cmake_minimum_required(VERSION 3.15)
project(foggeett LANGUAGES CXX)

# ======================================================================
# 🔧 CONFIGURAÇÕES BÁSICAS DO COMPILADOR
# ======================================================================
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

# Inclui diretório principal
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/foggeett)

# ======================================================================
# 🧩 ARQUIVOS DE FONTE — MÓDULO CORE
# ======================================================================
add_library(_core MODULE
    foggeett/bindings.cpp
    foggeett/functions/utils.cpp
    foggeett/functions/slope.cpp
    foggeett/functions/sma.cpp
    foggeett/functions/wma.cpp
    foggeett/functions/ema.cpp
    foggeett/functions/mom.cpp
    foggeett/functions/macd.cpp
    foggeett/functions/roc.cpp
    foggeett/functions/obv.cpp
    foggeett/functions/adx.cpp
    foggeett/functions/rsi.cpp
    foggeett/functions/enrich.cpp
    foggeett/functions/extract.cpp
)

# ======================================================================
# ⚙️ EXTENSÃO DE SAÍDA (Windows = .pyd | Linux/macOS = .so)
# ======================================================================
if (WIN32)
    set_target_properties(_core PROPERTIES PREFIX "" SUFFIX ".pyd")
else()
    set_target_properties(_core PROPERTIES PREFIX "" SUFFIX ".so")
endif()

# ======================================================================
# 🧱 PYBIND11 — INTEGRAÇÃO PYTHON/C++
# ======================================================================
find_package(pybind11 REQUIRED)
target_link_libraries(_core PRIVATE pybind11::module)

# ======================================================================
# ⚡ OPENMP (Paralelismo Opcional)
# ======================================================================
find_package(OpenMP)
if(OpenMP_CXX_FOUND)
    message(STATUS "✅ OpenMP detectado — paralelismo habilitado.")
    target_link_libraries(_core PRIVATE OpenMP::OpenMP_CXX)
    target_compile_definitions(_core PRIVATE FOGGEETT_USE_OPENMP=1)
else()
    message(WARNING "⚠️ OpenMP não detectado — compilando sem paralelismo.")
endif()

# ======================================================================
# 📦 INSTALAÇÃO DO MÓDULO (para pip editable / wheel)
# ======================================================================
install(TARGETS _core
    LIBRARY DESTINATION foggeett
    RUNTIME DESTINATION foggeett
    ARCHIVE DESTINATION foggeett
)

# ======================================================================
# 📄 INSTALAÇÃO DOS HEADERS
# ======================================================================
install(DIRECTORY foggeett/
    DESTINATION foggeett
    FILES_MATCHING PATTERN "*.h" PATTERN "*.hpp"
)



# cmake_minimum_required(VERSION 3.15)
# project(foggeett LANGUAGES CXX)

# set(CMAKE_CXX_STANDARD 17)
# set(CMAKE_CXX_STANDARD_REQUIRED ON)

# include_directories(${CMAKE_CURRENT_SOURCE_DIR}/foggeett)

# add_library(_core MODULE
#     foggeett/bindings.cpp
#     foggeett/functions/utils.cpp
#     foggeett/functions/slope.cpp
#     foggeett/functions/sma.cpp
#     foggeett/functions/wma.cpp
#     foggeett/functions/ema.cpp
#     foggeett/functions/mom.cpp
#     foggeett/functions/macd.cpp
#     foggeett/functions/roc.cpp
#     foggeett/functions/obv.cpp
#     foggeett/functions/adx.cpp
#     foggeett/functions/rsi.cpp
#     foggeett/functions/enrich.cpp
#     foggeett/functions/extract.cpp

# )

# # --------- UNIVERSAL WINDOWS / LINUX ---------
# if (WIN32)
#     set_target_properties(_core PROPERTIES PREFIX "" SUFFIX ".pyd")
# else()
#     set_target_properties(_core PROPERTIES PREFIX "" SUFFIX ".so")
# endif()
# # ----------------------------

# find_package(pybind11 REQUIRED)
# target_link_libraries(_core PRIVATE pybind11::module)

# install(TARGETS _core
#     LIBRARY DESTINATION foggeett
#     RUNTIME DESTINATION foggeett
#     ARCHIVE DESTINATION foggeett
# )

# # Install the header files
# install(DIRECTORY foggeett/
#     DESTINATION foggeett
#     FILES_MATCHING PATTERN "*.h"
# )   