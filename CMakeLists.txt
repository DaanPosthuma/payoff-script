cmake_minimum_required(VERSION 3.22)

project(ps
    VERSION 0.0.1
    DESCRIPTION "todo"
    LANGUAGES CXX
)

enable_testing()

find_package(GTest REQUIRED)
find_package(Python COMPONENTS Interpreter Development)
find_package(pybind11 CONFIG)

include(GoogleTest)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_subdirectory(ps)
