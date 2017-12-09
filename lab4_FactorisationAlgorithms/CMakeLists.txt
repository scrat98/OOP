cmake_minimum_required(VERSION 3.8)
project(lab4_FactorisationAlgorithms)

set(CMAKE_CXX_STANDARD 14)

set(BOOST_ROOT C:/Boost/1.65.1)
set(BOOST_LIBRARYDIR ${BOOST_ROOT}/stage/lib)
set(BOOST_INCLUDEDIR ${BOOST_ROOT}/boost)
set(Boost_USE_STATIC_LIBS ON)

find_package(Boost REQUIRED COMPONENTS thread chrono system)
include_directories(${Boost_INCLUDE_DIRS} src)
link_libraries(${Boost_LIBRARIES})

file(GLOB Algorithms
        src/algorithms/*.*)

set(Core
        src/core/CheckSystem.hpp
        src/core/CheckSystem.cpp
        src/core/Factorizer.hpp
        src/core/FactorizationAlgorithm.hpp
        src/core/Randomizer.hpp
        src/core/ThreadPool.hpp)

set(Utils
        src/utils/CommonFunctions.hpp
        src/utils/CommonFunctions.cpp)

set(SOURCE_FILES
        main.cpp
        ${Core}
        ${Utils}
        ${Algorithms}
        )

add_executable(lab4_FactorisationAlgorithms ${SOURCE_FILES})