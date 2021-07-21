cmake_minimum_required(VERSION 3.9 FATAL_ERROR)
project(TimersLibrary CXX)
enable_testing()

SET(CMAKE_CXX_STANDARD 20)

option(BUILD_TESTS "Build tests" OFF)

add_subdirectory(Source)
add_subdirectory(Tests)