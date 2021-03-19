set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

set(COLOR_HEADER_FILES color/Lab.h color/Lch.h color/XYZ.h color/color.h
		       color/difference.h color/sRGB.h)
set(COLOR_SOURCE_FILES ../src/color/Lab.cpp ../src/color/Lch.cpp
		       ../src/color/XYZ.cpp ../src/color/color.cpp
		       ../src/color/difference.cpp ../src/color/sRGB.cpp)

set(FFMPEG_HEADER_FILES ffmpeg/encode.h ffmpeg/decode.h)
set(FFMPEG_SOURCE_FILES ../src/ffmpeg/encode.cpp ../src/ffmpeg/decode.cpp)

set(HEADER-ONLY_HEADER_FILES header-only/constants.h header-only/pixel.h)

set(IMAGE_HEADER_FILES image/image.h)
set(IMAGE_SOURCE_FILES ../src/image/image.cpp)

set(LOGIC_HEADER_FILES logic/kmeans.h logic/centroid.h)
set(LOGIC_SOURCE_FILES ../src/logic/kmeans.cpp ../src/logic/centroid.cpp)

set(VIDEO_HEADER_FILES video/videoProcess.h)
set(VIDEO_SOURCE_FILES ../src/video/videoProcess.cpp)

add_library(color ${COLOR_HEADER_FILES} ${COLOR_SOURCE_FILES} ${HEADER-ONLY_HEADER_FILES})
target_include_directories(color PRIVATE color header-only)

# add_library(ffmpeg ${FFMPEG_HEADER_FILES} ${FFMPEG_SOURCE_FILES})

# add_library(header-only ${HEADER-ONLY_HEADER_FILES})
# target_include_directories(header-only PRIVATE header-only)

add_library(image ${IMAGE_HEADER_FILES} ${IMAGE_SOURCE_FILES} ${HEADER-ONLY_HEADER_FILES})
target_include_directories(image PRIVATE image header-only)

add_library(logic ${LOGIC_HEADER_FILES} ${LOGIC_SOURCE_FILES} ${HEADER-ONLY_HEADER_FILES})
target_include_directories(logic PRIVATE logic header-only)

add_library(video ${VIDEO_HEADER_FILES} ${VIDEO_SOURCE_FILES} ${HEADER-ONLY_HEADER_FILES}
		  ${LOGIC_HEADER_FILES} ${LOGIC_SOURCE_FILES} ${COLOR_HEADER_FILES}
		  ${COLOR_SOURCE_FILES} ${IMAGE_HEADER_FILES} ${IMAGE_SOURCE_FILES})
target_include_directories(video PRIVATE video header-only logic color image)
