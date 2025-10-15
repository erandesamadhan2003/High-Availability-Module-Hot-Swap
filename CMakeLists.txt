cmake_minimum_required(VERSION 3.15)
project(HotSwapSystem VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    add_compile_options(-Wall -Wextra -Wpedantic)
endif()

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug)
endif()

message(STATUS "Build type: ${CMAKE_BUILD_TYPE}")

set(SOURCE_DIR ${CMAKE_SOURCE_DIR}/src)
set(CORE_DIR ${SOURCE_DIR}/core)
set(MODULES_DIR ${SOURCE_DIR}/modules)
set(UTILS_DIR ${SOURCE_DIR}/utils)
set(EXAMPLES_DIR ${CMAKE_SOURCE_DIR}/examples)

# === MAIN HOTSWAP CORE LIBRARY ===
add_library(hotswap_core SHARED
    ${CORE_DIR}/ModuleManager.cpp
    ${CORE_DIR}/DynamicLibrary.cpp
    ${CORE_DIR}/IModule.cpp
    # MODULE FILES YAHAN SE HATA DO - they should be separate libraries
)

# Platform-specific libraries link karo
if(UNIX AND NOT APPLE)
    target_link_libraries(hotswap_core PUBLIC dl pthread)
endif()

# Include directories set karo
target_include_directories(hotswap_core PUBLIC 
    ${SOURCE_DIR}
)

# === SIMPLE MODULE LIBRARY ===
add_library(simple_module SHARED
    ${MODULES_DIR}/SimpleModule.cpp
)
target_link_libraries(simple_module hotswap_core)

# === CALCULATOR MODULE LIBRARY ===  
add_library(calculator_module SHARED
    ${MODULES_DIR}/CalculatorModule.cpp
)
target_link_libraries(calculator_module hotswap_core)

# Linux pe .so file ka prefix remove karo
set_target_properties(simple_module PROPERTIES
    PREFIX ""
    OUTPUT_NAME "simple_module"
)

set_target_properties(calculator_module PROPERTIES
    PREFIX ""
    OUTPUT_NAME "calculator_module"
)

# === DEMO EXECUTABLE ===
add_executable(demo ${EXAMPLES_DIR}/main.cpp)
target_link_libraries(demo hotswap_core)

# === PHASE 3 TEST EXECUTABLE ===
add_executable(phase3_test ${EXAMPLES_DIR}/phase3_test.cpp)
target_link_libraries(phase3_test hotswap_core)

# === PHASE 4 TEST EXECUTABLE ===
add_executable(phase4_test ${EXAMPLES_DIR}/phase4_test.cpp)
target_link_libraries(phase4_test hotswap_core)

add_library(calculator_v1 SHARED
    ${MODULES_DIR}/CalculatorModuleV1.cpp
)
target_link_libraries(calculator_v1 hotswap_core)
set_target_properties(calculator_v1 PROPERTIES
    PREFIX ""
    OUTPUT_NAME "calculator_v1"
)

# Calculator Module Version 2
add_library(calculator_v2 SHARED
    ${MODULES_DIR}/CalculatorModuleV2.cpp
)
target_link_libraries(calculator_v2 hotswap_core)
set_target_properties(calculator_v2 PROPERTIES
    PREFIX ""
    OUTPUT_NAME "calculator_v2"
)

# Text Processor Module Version 1
add_library(textprocessor_v1 SHARED
    ${MODULES_DIR}/TextProcessorV1.cpp
)
target_link_libraries(textprocessor_v1 hotswap_core)
set_target_properties(textprocessor_v1 PROPERTIES
    PREFIX ""
    OUTPUT_NAME "textprocessor_v1"
)

# Phase 5 Test Executable
add_executable(phase5_test ${EXAMPLES_DIR}/phase5_test.cpp)
target_link_libraries(phase5_test hotswap_core)

# Advanced Demo Application
add_executable(advanced_demo ${EXAMPLES_DIR}/advanced_demo.cpp)
target_link_libraries(advanced_demo hotswap_core)

# Simple Demo (if not already present)
add_executable(simple_demo ${EXAMPLES_DIR}/simple_demo.cpp)
target_link_libraries(simple_demo hotswap_core)

# Test Executables
add_executable(test_basic_loading tests/test_basic_loading.cpp)
target_link_libraries(test_basic_loading hotswap_core)

add_executable(test_invalid_module tests/test_invalid_module.cpp)
target_link_libraries(test_invalid_module hotswap_core)

add_executable(test_duplicate_loading tests/test_duplicate_loading.cpp)
target_link_libraries(test_duplicate_loading hotswap_core)

add_executable(test_stress tests/test_stress.cpp)
target_link_libraries(test_stress hotswap_core dl pthread)