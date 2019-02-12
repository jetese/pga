#include "kernel.h"

__global__ void vectorAddKernel(float * da, float * db, float * dc, int n) {
	int pos = threadIdx.x + blockDim.x * blockIdx.x;
	if (threadIdx.x < n) {
		dc[pos] = da[pos] + db[pos];
	}
}

void vectorAdd(float *a, float *b, float *c, int n) {
	
	float * dev_a ,*dev_b ,* dev_c ;
	size_t size = n * sizeof(float);

	cudaMalloc(&dev_a, size);
	cudaMalloc(&dev_b, size);
	cudaMalloc(&dev_c, size);

	cudaMemcpy(dev_a, a, size, cudaMemcpyHostToDevice);
	cudaMemcpy(dev_b, b, size, cudaMemcpyHostToDevice);

	vectorAddKernel << <2, n/2 >> > (dev_a, dev_b, dev_c, n);

	cudaMemcpy(c, dev_c, size, cudaMemcpyDeviceToHost);

	cudaFree(dev_a);
	cudaFree(dev_b);
	cudaFree(dev_c);

};
