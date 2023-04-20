all: 
	nvcc  src/gpu_version_1.cu src/segment.cpp src/vray.h src/segment.hpp -o myapp


