cmake_minimum_required(VERSION 3.8)
project(lab1_IniParser)

set(CMAKE_CXX_STANDARD 14)

set(Boost_USE_STATIC_LIBS ON)

find_package(Boost REQUIRED COMPONENTS regex)
include_directories(${Boost_INCLUDE_DIRS} src)

set(SOURCE_FILES
        main.cpp
        src/Parser.hpp
        src/Parser.cpp
        src/Exceptions.hpp)

link_libraries(${Boost_LIBRARIES})
add_executable (lab1_IniParser ${SOURCE_FILES})