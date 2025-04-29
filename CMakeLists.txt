cmake_minimum_required(VERSION 3.15)
project(foggeett LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/foggeett)

add_library(_core MODULE
    foggeett/bindings.cpp
    foggeett/functions/utils.cpp
    foggeett/functions/slope.cpp
    foggeett/functions/sma.cpp
    foggeett/functions/enrich.cpp
)

set_target_properties(_core PROPERTIES PREFIX "" SUFFIX ".pyd")  # <- IMPORTANTE NO WINDOWS

find_package(pybind11 REQUIRED)
target_link_libraries(_core PRIVATE pybind11::module)

# Ajuste para build correto em editable
install(TARGETS _core
    LIBRARY DESTINATION foggeett
    RUNTIME DESTINATION foggeett  # <- necessário para Windows (runtime *.pyd)
    ARCHIVE DESTINATION foggeett
)
