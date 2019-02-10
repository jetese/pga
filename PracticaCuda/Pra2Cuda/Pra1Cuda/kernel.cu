#include "kernel.h"
#include <stdio.h>
#include <cstdio>

__global__ void matrixAdd(float *a, float *b, float *c, int n, int m) {
	// Escribir en c la suma de a y b
	int posx = threadIdx.x + blockIdx.x * blockDim.x;
	int posy = threadIdx.y + blockIdx.y * blockDim.y;
	if (posx < n && posy < m) {
		c[posx + posy *n] = a[posx + posy*n] + b[posx + posy*n];
		printf("posx %d \n", posx);
		printf("posy %d \n", posy);
	}
};

__global__ void matrixAddPitch(float *a, float *b, float *c, int n,int m, size_t pitch) {
	// Escribir en c la suma de a y b
	int posx = threadIdx.x + blockIdx.x * blockDim.x;
	int posy = threadIdx.y + blockIdx.y * blockDim.y;
	if (posx < n && posy < m) {
		c[posx + posy *(pitch /sizeof(*a))] = a[posx + posy*(pitch / sizeof(*a))] + b[posx + posy*pitch / sizeof(*a)];
	}
};
	

void vectorAdd(float *a, float *b, float *c, int n, int m) {
	size_t size = n *m* sizeof(float);
	size_t size2 = n*m;
	//byte a;
	float *d_a = NULL;
	float *d_b = NULL;
	float *d_c = NULL;
	int dev_n,dev_m;
	size_t d_pitch=NULL;
	size_t pitch=NULL;
	size_t width=n;
	size_t height=m;
	//malloc en cuda para d_a, d_b y d_c
	/*cudaMalloc(&d_a,size);
	cudaMalloc(&d_b,size);
	cudaMalloc(&d_c,size);*/
	//Cambiar variables, los pitch no pueden ser los mismos
	cudaMallocPitch(&d_a, &pitch, sizeof(float)* width, height);
	cudaMallocPitch(&d_b, &pitch, sizeof(float)* width, height);
	cudaMallocPitch(&d_c, &pitch, sizeof(float)* width, height);//Me devuelve el ancho de fila que ha reservado realmente en el pitch ( lo devuelve en bytes)

	//copiar los datos de a y b a d_a y d_b respectivamente
	//cudaMemcpy(d_a, a, size, cudaMemcpyHostToDevice);
	//cudaMemcpy(d_b, b, size, cudaMemcpyHostToDevice);
	cudaMemcpy2D(d_a, pitch, a, sizeof(float)* width, sizeof(float)* width, height, cudaMemcpyHostToDevice);
	cudaMemcpy2D(d_b, pitch, b, sizeof(float)* width, sizeof(float)* width, height, cudaMemcpyHostToDevice);

	//llamada al kernel
	dim3 threads(4, 4);
	//dim3 blocks((size2 + threads.x - 1) / threads.x, (size2 + threads.y - 1)/threads.y, 1);
	//(col-1)/thread.x +1 , (row-1)/thread.y +1
	dim3 blocks((m - 1) / threads.x + 1, (n - 1) / threads.y + 1);
	//matrixAdd << < blocks, threads >> >(d_a, d_b, d_c, n, m);
	matrixAddPitch << < blocks, threads >> >(d_a, d_b, d_c,n,m, pitch);

	//copiar los datos de vuelta. Los datos calculados y guardados en d_c han de ser copiados a c.
	//cudaMemcpy2D(c,d_c , size, cudaMemcpyDeviceToHost);
	cudaMemcpy2D(c, sizeof(float)*width, d_c, pitch, sizeof(float)* width, height, cudaMemcpyDeviceToHost);
	//liberar memoria que ya no se necesita
	cudaFree(d_a);
	cudaFree(d_b);
	cudaFree(d_c);
};
