cmake_minimum_required(VERSION 3.6)

project(RayTracer)
set(CMAKE_CXX_STANDARD 17)

add_subdirectory(Linear_Algebra)
add_subdirectory(Color)
# Image depends on color, so color must be included first
add_subdirectory(Image)
# Geometry depends on both linear algebra and color
add_subdirectory(Geometry)

add_executable(ray_tracer prettyPicture.cpp)
target_include_directories(ray_tracer PUBLIC .)
target_link_libraries(ray_tracer PUBLIC
        linear_algebra_core
        image_writers
        geometry)