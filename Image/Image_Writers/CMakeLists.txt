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

#find_package(ImageMagick REQUIRED COMPONENTS Magick++)
#target_compile_definitions(png_writer PRIVATE
#        MAGICKCORE_QUANTUM_DEPTH=16
#        MAGICKCORE_HDRI_ENABLE=0
#        )
#target_include_directories(png_writer PUBLIC . ${ImageMagick_INCLUDE_DIRS})
#execute_process(COMMAND Magick++-config --ldflags
#        OUTPUT_VARIABLE ImageMagick_LINK_FLAGS
#        OUTPUT_STRIP_TRAILING_WHITESPACE
#        )
#target_link_libraries(png_writer PUBLIC image_core utility ${ImageMagick_LINK_FLAGS})