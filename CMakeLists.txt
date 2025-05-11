cmake_minimum_required(VERSION 3.20)

project(
    puckman
    VERSION 0.0.0
    LANGUAGES CXX
)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

find_package(SDL2 REQUIRED)

install(
    DIRECTORY
    res
    DESTINATION ${CMAKE_BINARY_DIR}/dist/bin
)

add_subdirectory(main)
add_subdirectory(src)
