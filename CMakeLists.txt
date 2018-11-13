cmake_minimum_required(VERSION 3.12)
project(dltools)

set(CMAKE_CXX_STANDARD 11)

find_package(PythonLibs REQUIRED)
find_package(pybind11 REQUIRED)
include_directories(
        ${PYTHON_INCLUDE_DIRS}
        include
        dltools/src
)

add_subdirectory(dltools/src)

add_executable(demo demo/main.cpp)
