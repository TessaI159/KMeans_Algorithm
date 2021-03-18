cmake_minimum_required(VERSION 3.12)

project(KMeans VERSION 0.0
LANGUAGES CXX)

add_executable(main main.cpp color.h videoprocess.h constants.h)
add_library(extras STATIC pixel.h constants.h image.cpp image.h)
add_library(color STATIC Lab.cpp Lab.h Lch.cpp Lch.h XYZ.cpp XYZ.h color.cpp color.h sRGB.cpp sRGB.h)
add_library(logic STATIC centroid.cpp centroid.h difference.cpp difference.h kmeans.cpp kmeans.h videoProcess.cpp videoProcess.h)
add_library(ffmpeg_api STATIC decode.cpp decode.h encode.cpp encode.h)

target_include_directories(color PUBLIC color)
target_include_directories(logic PUBLIC logic)
target_include_directories(ffmpeg PUBLIC ffmpeg)
