cmake_minimum_required(VERSION 3.6)

add_subdirectory(Third_Party)

add_library(utility
        RandomNumberGenerator.h
        RandomNumberGenerator.cpp
        LinearAlgebraJsonParser.h)
target_include_directories(utility PUBLIC .)
target_link_libraries(utility PUBLIC linear_algebra_core nlohmann_json)