cmake_minimum_required(VERSION 3.6)

add_library(ppm_writer
        ImageWriter_I.h
        PPMImageWriter.cpp
        PPMImageWriter.h)
target_include_directories(ppm_writer PUBLIC .)
target_link_libraries(ppm_writer PUBLIC image_core)

add_library(png_writer
        ImageWriter_I.h
        PNGImageWriter.cpp
        PNGImageWriter.h)
target_include_directories(png_writer PUBLIC .)
target_link_libraries(png_writer PUBLIC image_core utility)

add_library(image_writer_builder
        ImageWriter_I.h
        ImageWriterBuilder.cpp
        ImageWriterBuilder.h)
target_include_directories(image_writer_builder PUBLIC .)
target_link_libraries(image_writer_builder PUBLIC image_core ppm_writer png_writer)