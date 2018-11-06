cmake_minimum_required(VERSION 3.12)
project(dltools)

set(CMAKE_CXX_STANDARD 14)

add_library(dltools STATIC sacla_model.cpp hittype.cpp)
