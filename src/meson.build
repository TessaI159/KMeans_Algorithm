sources = ['Lab.cpp', 'Lch.cpp', 'XYZ.cpp', 'centroid.cpp', 'color.cpp', 'decode.cpp',
	  'difference.cpp', 'encode.cpp', 'image.cpp', 'kmeans.cpp', 'sRGB.cpp', 'videoProcess.cpp']

libTiss = shared_library('Tiss', sources, include_directories : [include_dir, opencv_include_dir])
								       
executable('main.run', 'main.cpp', link_with : libTiss, include_directories :
		       [include_dir, opencv_include_dir, opencv_lib_dir])
