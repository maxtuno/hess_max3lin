cmake_minimum_required(VERSION 2.8)
project(hess_max3lin)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/bin)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/lib)

set(CMAKE_C_FLAGS "-std=c11 -O3 -fPIC -D LOG")

add_executable(hess_max3lin hess_max3lin.c)
