cmake_minimum_required(VERSION 3.6)

project(RayTracer)
set(CMAKE_CXX_STANDARD 20)

add_subdirectory(Linear_Algebra)
add_subdirectory(Utility)
add_subdirectory(Color)
# Image depends on color, so color must be included first
add_subdirectory(Image)
# Geometry depends on both linear algebra and color
add_subdirectory(Geometry)
# Scene depends on linear algebra, geometry, and utility
add_subdirectory(Scene)
# Environment depends on Geometry, linear algebra, and color
add_subdirectory(Environment)

add_executable(ray_tracer
        main.cpp
        RayTracer.h
        RayTracer.cpp)
target_include_directories(ray_tracer PUBLIC .)
target_link_libraries(ray_tracer PUBLIC
        third_party
        utility
        linear_algebra_core
        geometry
        image_core
        image_writer_builder
        environment
        scene
        # polyscope
        )
