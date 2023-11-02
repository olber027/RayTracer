cmake_minimum_required(VERSION 3.6)

add_library(geometry INTERFACE
        Geometry.h
        Sphere.h
        Plane.h
        BoundedPlane.h
        Triangle.h
)
target_include_directories(geometry INTERFACE .)
target_link_libraries(geometry INTERFACE linear_algebra_core color_core nlohmann_json utility)

add_library(geometry_builder INTERFACE
        GeometryBuilder.h)
target_include_directories(geometry_builder INTERFACE .)
target_link_libraries(geometry_builder INTERFACE geometry nlohmann_json)