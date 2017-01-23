cmake_minimum_required(VERSION 3.6)
project(RayTracer)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES prettyPicture.cpp Camera.h Color.h Output.h Ray.h RayTracer.h Utils.h)
add_executable(RayTracer ${SOURCE_FILES})