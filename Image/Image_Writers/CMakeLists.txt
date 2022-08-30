cmake_minimum_required(VERSION 3.6)

add_library(image_writers
        ImageWriter_I.h
        PPMImageWriter.h
        PPMImageWriter.cpp)
target_include_directories(image_writers PUBLIC .)
target_link_libraries(image_writers PUBLIC image_core)