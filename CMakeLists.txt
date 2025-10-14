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

# --------- UNIVERSAL WINDOWS / LINUX ---------
if (WIN32)
    set_target_properties(_core PROPERTIES PREFIX "" SUFFIX ".pyd")
else()
    set_target_properties(_core PROPERTIES PREFIX "" SUFFIX ".so")
endif()
# ----------------------------

find_package(pybind11 REQUIRED)
target_link_libraries(_core PRIVATE pybind11::module)

install(TARGETS _core
    LIBRARY DESTINATION foggeett
    RUNTIME DESTINATION foggeett
    ARCHIVE DESTINATION foggeett
)

# Install the header files
install(DIRECTORY foggeett/
    DESTINATION foggeett
    FILES_MATCHING PATTERN "*.h"
)   