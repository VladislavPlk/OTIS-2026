add_library(otis_core
    Model1_3.cpp
    Model2_5.cpp
    Model3_7.cpp
    InputSignal.cpp
    StabilityAnalyzer.cpp
    Simulation.cpp
)
target_include_directories(otis_core PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}")

add_executable(otis_task_02 main.cpp)
target_link_libraries(otis_task_02 PRIVATE otis_core)
configure_file(plot.py plot.py COPYONLY)

