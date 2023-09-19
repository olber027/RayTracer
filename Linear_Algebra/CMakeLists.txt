cmake_minimum_required(VERSION 3.6)

add_library(linear_algebra_core INTERFACE
        Vector_X.h
        Point_X.h
        Matrix_MxN.h
        Ray.h
        LinearAlgebraTypeTraits.h)
target_include_directories(linear_algebra_core INTERFACE .)