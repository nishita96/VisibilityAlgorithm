all: 
	# nvcc  src/gpu_version_1.cu src/vray.h src/segment.hpp -o myapp
	nvcc  src/gpu_version_1.cu -I./openFrameworks  -o myapp


