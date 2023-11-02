cmake_minimum_required(VERSION 3.6)
set(CMAKE_CXX_STANDARD 20)

project(RayTracer)

add_subdirectory(Linear_Algebra)
# Utility defines linear algebra json parsers, so linear algebra must be included first
add_subdirectory(Utility)
add_subdirectory(Color)
# Image depends on color, so color must be included first
add_subdirectory(Image)
# Geometry depends on both linear algebra, color, and utility
add_subdirectory(Geometry)
# Scene depends on linear algebra, geometry, and utility
add_subdirectory(Scene)
# Environment depends on Geometry, linear algebra, and color
add_subdirectory(Environment)

set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)

add_executable(ray_tracer
        main.cpp
        RayTracer.h
)

target_include_directories(ray_tracer PUBLIC .)

target_link_libraries(ray_tracer
        PUBLIC
                third_party
                utility
                linear_algebra_core
                geometry
                image_core
                image_writer_builder
                environment
                scene
        PRIVATE
                Threads::Threads
        )