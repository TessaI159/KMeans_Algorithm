cmake_minimum_required(VERSION 3.15 FATAL_ERROR)

project(KMeans VERSION 0.0)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE-CXX_STANDARD_REQUIRED True)

add_subdirectory(include)


add_custom_target(build-time-make-bin-directory ALL COMMAND ${CMAKE_COMMAND} -E make_directory bin)
add_custom_target(build-time-make-lib-directory ALL COMMAND ${CMAKE_COMMAND} -E make_directory lib)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin) 
