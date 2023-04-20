all: 
	# nvcc  src/gpu_version_1.cu src/vray.h src/segment.hpp -o myapp
	nvcc  src/gpu_version_1.cu -I./openFrameworks/ -I./openFrameworks/utils -I./openFrameworks/types -I./openFrameworks/math -I./openFrameworks/graphics   -o myapp


