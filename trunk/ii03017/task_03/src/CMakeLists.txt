# Модели и их реализации берутся напрямую из соседней task_01/src.
# Исходники моделей не копируются в task_03.
set(OTIS_TASK01_SRC "${CMAKE_CURRENT_SOURCE_DIR}/../../task_01/src"
    CACHE PATH "Путь к исходникам моделей лабораторной работы №1")
get_filename_component(OTIS_TASK01_SRC "${OTIS_TASK01_SRC}" ABSOLUTE)

foreach(required_file Model.h Model1_3.h Model1_3.cpp Model2_5.h Model2_5.cpp Model3_7.h Model3_7.cpp)
    if(NOT EXISTS "${OTIS_TASK01_SRC}/${required_file}")
        message(FATAL_ERROR
            "Не найден ${required_file} в ${OTIS_TASK01_SRC}. "
            "Расположите task_03 рядом с task_01 или задайте -DOTIS_TASK01_SRC=...")
    endif()
endforeach()

add_library(otis_task_03_core
    PIDController.cpp
    PIDSimulation.cpp
    "${OTIS_TASK01_SRC}/Model1_3.cpp"
    "${OTIS_TASK01_SRC}/Model2_5.cpp"
    "${OTIS_TASK01_SRC}/Model3_7.cpp"
)
target_include_directories(otis_task_03_core PUBLIC
    "${CMAKE_CURRENT_SOURCE_DIR}"
    "${OTIS_TASK01_SRC}"
)
target_compile_features(otis_task_03_core PUBLIC cxx_std_17)

add_executable(otis_task_03 main.cpp)
target_link_libraries(otis_task_03 PRIVATE otis_task_03_core)
configure_file(plot.py plot.py COPYONLY)

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

    add_executable(otis_task_03_tests ../test/test_pid.cpp)
    target_link_libraries(otis_task_03_tests PRIVATE otis_task_03_core GTest::gtest_main)
    include(GoogleTest)
    gtest_discover_tests(otis_task_03_tests)
endif()
