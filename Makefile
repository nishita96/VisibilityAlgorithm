all: 
	nvcc  src/gpu_version_1.cu src/segment.cpp src/segment.hpp src/cpu_version.hpp src/vray.cpp src/vray.hpp -o myapp


