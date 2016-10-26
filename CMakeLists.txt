cmake_minimum_required(VERSION 3.0)
project(mm_gps VERSION 0.0.1 LANGUAGES C)



set(SOURCE_DIR source)
file(GLOB LIB_SOURCES "${SOURCE_DIR}/mm_gps*.c")
include_directories(include)
add_compile_options(-std=c11)

add_library(mm_gps_static STATIC ${LIB_SOURCES})
add_executable(mm_gps_test ${SOURCE_DIR}/main.c)
target_link_libraries(mm_gps_test mm_gps_static) # link to mm_gps when building mm_gps_test
