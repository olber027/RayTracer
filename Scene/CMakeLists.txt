cmake_minimum_required(VERSION 3.6)

add_library(screen INTERFACE Screen.h)
target_include_directories(screen INTERFACE .)
target_link_libraries(screen INTERFACE linear_algebra_core)

add_library(camera INTERFACE Camera.h)
target_include_directories(camera INTERFACE .)
target_link_libraries(camera INTERFACE linear_algebra_core)

add_library(scene INTERFACE Scene.h)
target_include_directories(scene INTERFACE .)
target_link_libraries(scene INTERFACE linear_algebra_core screen camera)