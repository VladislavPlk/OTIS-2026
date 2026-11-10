cmake_minimum_required(VERSION 3.16)
project(OTIS_Task_03_PID LANGUAGES CXX)

include(CTest)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

option(OTIS_BUILD_TESTS "Build Google Test unit tests" ON)

add_subdirectory(src)

if(OTIS_BUILD_TESTS AND BUILD_TESTING)
    include(FetchContent)
    FetchContent_Declare(
        googletest
        GIT_REPOSITORY https://github.com/google/googletest.git
        GIT_TAG v1.15.2
        GIT_SHALLOW TRUE
    )
    set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
    FetchContent_MakeAvailable(googletest)

    add_executable(otis_task_03_tests test/test_pid.cpp)
    target_link_libraries(otis_task_03_tests PRIVATE otis_task_03_core GTest::gtest_main)
    include(GoogleTest)
    gtest_discover_tests(otis_task_03_tests)
endif()
