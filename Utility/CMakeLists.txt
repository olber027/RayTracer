cmake_minimum_required(VERSION 3.6)

add_subdirectory(Third_Party)

add_library(utility INTERFACE Endian.h)
target_include_directories(utility INTERFACE .)