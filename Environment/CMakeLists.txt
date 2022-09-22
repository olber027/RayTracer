cmake_minimum_required(VERSION 3.6)

add_library(environment
        Environment.h
        Environment.cpp)
target_include_directories(environment PUBLIC .)
target_link_libraries(environment PUBLIC linear_algebra_core color_core nlohmann_json geometry geometry_builder)