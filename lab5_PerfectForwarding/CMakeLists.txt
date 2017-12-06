cmake_minimum_required(VERSION 3.8)
project(lab5_PerfectForwarding)

set(CMAKE_CXX_STANDARD 14)

set(SOURCE_FILES main.cpp BoolVector.hpp)
add_executable(lab5_PerfectForwarding ${SOURCE_FILES})