cmake_minimum_required(VERSION 3.6)

add_library(color_core INTERFACE
        Color.h)
target_include_directories(color_core INTERFACE .)