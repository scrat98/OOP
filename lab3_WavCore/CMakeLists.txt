cmake_minimum_required(VERSION 3.8)
project(lab3_WavCore)

set(CMAKE_CXX_STANDARD 14)
include_directories(src)
set(SOURCE_FILES
        main.cpp
        src/WavException.h
        src/WavHeader.h
        src/WavReader.cpp
        src/WavReader.hpp)

add_executable (lab3_WavCore ${SOURCE_FILES})