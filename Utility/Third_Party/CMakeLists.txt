cmake_minimum_required(VERSION 3.6)

# add_subdirectory(polyscope)

add_library(third_party INTERFACE
        ArgParse.h
        json.h)
target_include_directories(third_party INTERFACE .)
# target_link_libraries(third_party INTERFACE polyscope)

add_library(nlohmann_json INTERFACE json.h)
target_include_directories(nlohmann_json INTERFACE .)

add_library(arg_parse INTERFACE ArgParse.h)
target_include_directories(arg_parse INTERFACE .)