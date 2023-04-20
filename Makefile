all: 
	# nvcc  src/gpu_version_1.cu src/vray.h src/segment.hpp -o myapp
	# nvcc  src/gpu_version_1.cu -I./openFrameworks/ -I./openFrameworks/utils -I./openFrameworks/types -I./openFrameworks/math -I./openFrameworks/graphics -I./openFrameworks/3d -I./openFrameworks/communication -I./openFrameworks/events -I./openFrameworks/gl -I./openFrameworks/sound -I./openFrameworks/video   -o myapp
	nvcc  src/gpu_version_1.cu -o myapp

