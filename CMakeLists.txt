cmake_minimum_required(VERSION 3.8)
project(HashEstatico)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH})

set(CMAKE_C_STANDARD 99)

set(SOURCE_FILES main.c tad.h)

add_executable(HashEstatico ${SOURCE_FILES})
