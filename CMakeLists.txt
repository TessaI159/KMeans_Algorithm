cmake_minimum_required(VERSION 3.12)

project(KMeans VERSION 0.0
LANGUAGES CXX)


add_library(header-only STATIC include/header-only/constants.h /include/header-only/pixel.h)

add_library(color STATIC include/color/Lab.h include/color/Lch.h include/color/XYZ.h
		  	 include/color/color.h include/color/difference.h include/color/sRGB.h)
			 
add_library(ffmpeg STATIC include/ffmpeg/decode.h include/ffmpeg/encode.h
		   	  src/ffmpeg/decode.cpp src/ffmpeg/encode.cpp)
			  
add_library(logic STATIC include/logic/kmeans.h include/logic/centroid.h /src/logic/kmeans.cpp
		  	 src/logic/centroid.cpp)

add_library(image STATIC include/image/image.h src/image/image.cpp)

add_library(video STATIC include/video/videoProcess.h src/video/videoProcess.cpp)


target_include_directories(color PUBLIC color)
target_include_directories(logic PUBLIC logic)
target_include_directories(ffmpeg PUBLIC ffmpeg)

add_executable(main main.cpp color.h videoprocess.h constants.h)
