cmake_minimum_required(VERSION 3.6)

add_library(geometry
        Geometry.h
        Sphere.h
        Sphere.cpp)
target_include_directories(geometry PUBLIC .)
target_link_libraries(geometry PUBLIC linear_algebra_core nlohmann_json)