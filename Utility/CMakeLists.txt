cmake_minimum_required(VERSION 3.6)

add_subdirectory(Third_Party)

add_library(utility INTERFACE
        RandomNumberGenerator.h
        LinearAlgebraJsonParser.h)
target_include_directories(utility INTERFACE .)
target_link_libraries(utility INTERFACE linear_algebra_core nlohmann_json)