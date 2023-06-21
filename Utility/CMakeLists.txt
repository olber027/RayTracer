cmake_minimum_required(VERSION 3.6)

add_subdirectory(Third_Party)

add_library(utility
        # Endian.h
        RandomNumberGenerator.h
        RandomNumberGenerator.cpp)
target_include_directories(utility PUBLIC .)