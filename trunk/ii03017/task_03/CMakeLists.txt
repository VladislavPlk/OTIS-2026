cmake_minimum_required(VERSION 3.16)
project(OTIS_Task_03_PID LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

include(CTest)
option(OTIS_BUILD_TESTS "Build Google Test unit tests" ON)

add_subdirectory(src)
