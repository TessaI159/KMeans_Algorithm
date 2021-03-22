project('KMeans_Algorithm', 'cpp', version: 'beta', license: 'LGPL')
add_global_arguments(language: 'cpp')

include_dir = include_directories('include')
opencv_include_dir = include_directories('deps/opencv4/include')
opencv_lib_dir = include_directories('deps/opencv4/lib')

add_project_link_arguments('-L../deps/opencv4/lib', '-lopencv_core', '-lopencv_imgproc',
			   '-lopencv_imgcodecs', '-lopencv_video', '-lopencv_videoio',
			   '-lopencv_highgui', language : 'cpp')


subdir('src')
