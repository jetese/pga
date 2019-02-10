#include "kernel.h"

__global__ void vectorAddKernel(float *a, float *b, float *c, int n) {
	// Escribir en c la suma de a y b
	int pos = threadIdx.x + blockIdx.x * blockDim.x;
	if(pos < n)
	c[pos] = a[pos] + b[pos];
};


void vectorAdd(float *a, float *b, float *c, int n) {
	size_t size = n * sizeof(float);

	float *d_a = NULL;
	float *d_b = NULL;
	float *d_c = NULL;

	//malloc en cuda para d_a, d_b y d_c
	cudaMalloc(&d_a,size);
	cudaMalloc(&d_b,size);
	cudaMalloc(&d_c,size);
	//copiar los datos de a y b a d_a y d_b respectivamente
	cudaMemcpy(d_a, a, size, cudaMemcpyHostToDevice);
	cudaMemcpy(d_b, b, size, cudaMemcpyHostToDevice);
	//llamada al kernel
	vectorAddKernel<<<1, n >>>(d_a, d_b, d_c, n);

	//copiar los datos de vuelta. Los datos calculados y guardados en d_c han de ser copiados a c.
	cudaMemcpy(c,d_c , size, cudaMemcpyDeviceToHost);
	//liberar memoria que ya no se necesita
	cudaFree(d_a);
	cudaFree(d_b);
	cudaFree(d_c);
};
