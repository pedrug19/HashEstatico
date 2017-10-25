cmake_minimum_required(VERSION 3.8)

project(HashEstatico)

set(CMAKE_C_STANDARD 99)

set(SOURCE_FILES HashEstatico.c)

include_directories(
        ${PROJECT_SOURCE_DIR}
)

add_executable(HashEstatico ${SOURCE_FILES})
