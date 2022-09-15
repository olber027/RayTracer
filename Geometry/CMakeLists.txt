cmake_minimum_required(VERSION 3.6)

add_library(geometry
        Geometry.h
        Sphere.h
        Sphere.cpp)
target_include_directories(geometry PUBLIC .)
target_link_libraries(geometry PUBLIC linear_algebra_core color_core nlohmann_json)

add_library(geometry_builder
        GeometryBuilder.cpp
        GeometryBuilder.h)
target_include_directories(geometry_builder PUBLIC .)
target_link_libraries(geometry_builder PUBLIC geometry nlohmann_json)