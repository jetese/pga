#include "kernel.h"
#include <stdio.h>
#include <cstdio>

//
__global__ void matrixAddPitch(float *a, float *b, float *c, int n,int m, size_t pitch) {
	// Escribir en c la suma de a y b
	int posx = threadIdx.x + blockIdx.x * blockDim.x;
	int posy = threadIdx.y + blockIdx.y * blockDim.y;
	if (posx < n && posy < m) {
		c[posx + posy *(pitch /sizeof(*a))] = a[posx + posy*(pitch / sizeof(*a))] + b[posx + posy*pitch / sizeof(*a)];
	}
};

__global__ void kernelAdd(float * a, float * b, float * c, int width, int height) {
	int posx = threadIdx.x + blockIdx.x * blockDim.x;
	int posy = threadIdx.y + blockIdx.y * blockDim.y;

	if (posx < width && posy < height) {
		c[posx + posy * width] = a[posx + posy * width] + b[posx + posy * width];
	}
}

__global__ void kernelAddPitch(float * a, float * b, float * c, int width, int height, size_t pitch) {
	int posx = threadIdx.x + blockIdx.x * blockDim.x;
	int posy = threadIdx.y + blockIdx.y * blockDim.y;

	if (posx < width && posy < height) {
		c[posx + posy * (pitch/sizeof(*a))] = a[posx + posy * (pitch / sizeof(*a))] + b[posx + posy * (pitch / sizeof(*a))];
	}
}
	

void vectorAdd(float *a, float *b, float *c, int n, int m) {
	////Declaración de variables
	//float * d_a, *d_b, *d_c;
	//int width = n * sizeof(float);
	//int height = m * sizeof(float);
	//size_t size = n * m * sizeof(float);

	////Reservamos espacio en gpu
	//cudaMalloc(&d_a, size);
	//cudaMalloc(&d_b, size);
	//cudaMalloc(&d_c, size);

	////Copiamos las variables de CPU a GPU
	//cudaMemcpy(d_a, a, size, cudaMemcpyHostToDevice);
	//cudaMemcpy(d_b, b, size, cudaMemcpyHostToDevice);

	////Número de threads y de bloques
	//dim3 threads = (4, 4);
	//dim3 blocks = ((n - 1) / threads.x + 1, (m - 1) / threads.y + 1);

	////LLamada a la funcion
	//kernelAdd << <blocks, threads >> > (d_a, d_b, d_c, n, m);

	////Copiamos datos de GPU -> CPU
	//cudaMemcpy(c, d_c, size, cudaMemcpyDeviceToHost);

	////Liberamos memoria
	//cudaFree(d_a);
	//cudaFree(d_b);
	//cudaFree(d_c);

	//Declaración de variables
	float * d_a, *d_b, *d_c;
	int width = n ;
	int height = m;
	size_t size = n * m * sizeof(float);
	size_t pitch;

	//Reserva de memoria GPU
	//cudaMallocPitch(&d_a, &pitch, width, height);
	//cudaMallocPitch(&d_b, &pitch, width, height);
	//cudaMallocPitch(&d_c, &pitch, width, height);
	cudaMallocPitch(&d_a, &pitch, sizeof(float)* width, height);
	cudaMallocPitch(&d_b, &pitch, sizeof(float)* width, height);
	cudaMallocPitch(&d_c, &pitch, sizeof(float)* width, height);

	// Copia de datos CPU -> GPU
	//cudaMemcpy2D(d_a, pitch, a, width, width, height, cudaMemcpyHostToDevice);
	//cudaMemcpy2D(d_b, pitch, b, width, width, height, cudaMemcpyHostToDevice);
	cudaMemcpy2D(d_a, pitch, a, sizeof(float)* width, sizeof(float)* width, height, cudaMemcpyHostToDevice);
	cudaMemcpy2D(d_b, pitch, b, sizeof(float)* width, sizeof(float)* width, height, cudaMemcpyHostToDevice);

	//Threads y bloques
	dim3 threads (4, 4);
	dim3 blocks ((m - 1) / threads.x + 1, (n - 1) / threads.y + 1);
	//dim3 threads(4, 4);
	//dim3 blocks((m - 1) / threads.x + 1, (n - 1) / threads.y + 1);

	//LLamada a la función
	//kernelAddPitch << <blocks, threads >> > (d_a, d_b, d_c, n, m, pitch);
	matrixAddPitch << < blocks, threads >> >(d_a, d_b, d_c, n, m, pitch);

	// Copia de GPU -> CPU
	//cudaMemcpy2D(c, width * sizeof(float), d_c, pitch, width * sizeof(float), height, cudaMemcpyDeviceToHost);
	cudaMemcpy2D(c, sizeof(float)*width, d_c, pitch, sizeof(float)* width, height, cudaMemcpyDeviceToHost);

	cudaFree(d_a);
};
//size_t size = n *m * sizeof(float);
//size_t size2 = n*m;

//float *d_a = NULL;
//float *d_b = NULL;
//float *d_c = NULL;
//int dev_n, dev_m;
//size_t d_pitch = NULL;
//size_t pitch = NULL;
//size_t width = n;
//size_t height = m;


////Cambiar variables, los pitch no pueden ser los mismos
//cudaMallocPitch(&d_a, &pitch, sizeof(float)* width, height);
//cudaMallocPitch(&d_b, &pitch, sizeof(float)* width, height);
//cudaMallocPitch(&d_c, &pitch, sizeof(float)* width, height);//Me devuelve el ancho de fila que ha reservado realmente en el pitch ( lo devuelve en bytes)

//cudaMemcpy2D(d_a, pitch, a, sizeof(float)* width, sizeof(float)* width, height, cudaMemcpyHostToDevice);
//cudaMemcpy2D(d_b, pitch, b, sizeof(float)* width, sizeof(float)* width, height, cudaMemcpyHostToDevice);
//
////llamada al kernel
//dim3 threads(4, 4);
////dim3 blocks((size2 + threads.x - 1) / threads.x, (size2 + threads.y - 1)/threads.y, 1);
////(col-1)/thread.x +1 , (row-1)/thread.y +1
//dim3 blocks((m - 1) / threads.x + 1, (n - 1) / threads.y + 1);
////matrixAdd << < blocks, threads >> >(d_a, d_b, d_c, n, m);
//matrixAddPitch << < blocks, threads >> >(d_a, d_b, d_c, n, m, pitch);
//
////copiar los datos de vuelta. Los datos calculados y guardados en d_c han de ser copiados a c.
////cudaMemcpy2D(c,d_c , size, cudaMemcpyDeviceToHost);
//cudaMemcpy2D(c, sizeof(float)*width, d_c, pitch, sizeof(float)* width, height, cudaMemcpyDeviceToHost);
////liberar memoria que ya no se necesita
//cudaFree(d_a);
//cudaFree(d_b);
//cudaFree(d_c);