<<<<<<< HEAD
cmake_minimum_required (VERSION 3.5)
project (OTIS)

# Search for directories simular to "trunk/ii0xxyy/task_0x/src/"
file(GLOB V_GLOB RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} "src/")
foreach(item ${V_GLOB})
	message( "Find \"${item}\"" )
	add_subdirectory(${item})
endforeach()
=======
cmake_minimum_required(VERSION 3.15)

project(OTIS LANGUAGES CXX)

add_subdirectory(src)
>>>>>>> ce45832df4742b3b2b23a98ce2870aab38cf4f5d
