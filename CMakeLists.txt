cmake_minimum_required(VERSION 3.15)
project(HotSwapSystem VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    add_compile_options(-Wall -Wextra -Wpedantic -Werror)
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

add_library(hotswap_core SHARED
    ${CORE_DIR}/ModuleManager.cpp
    ${CORE_DIR}/DynamicLibrary.cpp
    ${CORE_DIR}/IModule.cpp
    # ${UTILS_DIR}/Logger.cpp
)

if(UNIX AND NOT APPLE)
    target_link_libraries(hotswap_core PUBLIC dl pthread)
endif()

target_include_directories(hotswap_core PUBLIC 
    ${SOURCE_DIR}
)

add_executable(demo ${EXAMPLES_DIR}/phase2_test.cpp)
target_link_libraries(demo hotswap_core)

message(STATUS "HotSwap System configured successfully!")