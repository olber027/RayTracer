cmake_minimum_required(VERSION 3.6)

add_library(environment INTERFACE Environment.h)
target_include_directories(environment INTERFACE .)
target_link_libraries(environment INTERFACE linear_algebra_core color_core nlohmann_json geometry geometry_builder nlohmann_json)