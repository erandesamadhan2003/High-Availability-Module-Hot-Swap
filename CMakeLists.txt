cmake_minimum_required(VERSION 3.10)

project(High-Availability-module-hot-swap CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Threads REQUIRED)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)

add_executable(High-Availability-module-hot-swap 
    src/main.cpp
)

target_link_libraries(High-Availability-module-hot-swap PRIVATE Threads::Threads)
