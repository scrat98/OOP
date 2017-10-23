cmake_minimum_required(VERSION 3.8)
project(Lab03_WavCore)

set(CMAKE_CXX_STANDARD 14)
include_directories(src)
set(SOURCE_FILES
        main.cpp
        src/wav_core.cpp
        src/wav_core.h
	src/wav_header.h)

add_executable (Lab03_WavCore ${SOURCE_FILES})