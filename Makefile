all: 
	nvcc  src/gpu_version_1.cu src/segment.cpp src/segment.hpp  src/vray.cpp -o myapp


