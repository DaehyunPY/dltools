cmake_minimum_required(VERSION 3.12)
project(dltools-demo)

set(CMAKE_CXX_STANDARD 11)

add_subdirectory(dltools/src)

include_directories(dltools/src)
add_executable(demo demo/main.cpp)
target_link_libraries(demo dltools)
