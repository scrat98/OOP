cmake_minimum_required(VERSION 3.8)
project(lab2_StatisticMultiset)

set(CMAKE_CXX_STANDARD 14)
include_directories(src)
set(SOURCE_FILES
        main.cpp
        src/StatisticMultiset.hpp
        src/StatisticMultiset.cpp)

add_executable (lab2_StatisticMultiset ${SOURCE_FILES})