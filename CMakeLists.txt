cmake_minimum_required(VERSION 3.6)

project(RayTracer)
set(CMAKE_CXX_STANDARD 11)
add_executable(RayTracer prettyPicture.cpp Point.h Vector.h Matrix.h)