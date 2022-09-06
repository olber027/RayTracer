cmake_minimum_required(VERSION 3.6)

add_library(image_core INTERFACE Image.h)
target_include_directories(image_core INTERFACE .)
target_link_libraries(image_core INTERFACE color_core nlohmann_json)

# this subdirectory has to be added after image_core is defined because the image writers depend on image_core
add_subdirectory(Image_Writers)