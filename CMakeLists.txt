cmake_minimum_required(VERSION 3.0)
project(mm_gps VERSION 0.0.1 LANGUAGES C)



set(SOURCE_DIR source)
file(GLOB LIB_SOURCES "${SOURCE_DIR}/mm_gps*.c")
include_directories(include)
add_compile_options(-std=c11)

add_library(mm_gps_static STATIC ${LIB_SOURCES})
