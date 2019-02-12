#include "kernel.h"

#include <cstdio>

__global__ void convolution1Kernel(int *dst, int *src, int rows, int cols, int *filter) {
	// Convolucion en memoria global, similar a la convolucion en CPU
	int posx = threadIdx.x + blockIdx.x * blockDim.x;
	int posy = threadIdx.y + blockIdx.y * blockDim.y;
	if (posx > 0 && posy > 0 && posx < rows - 1 && posy < cols - 1) {
		for (int k = 0; k < 3; ++k) {
			for (int l = 0; l < 3; ++l) {
				dst[posy * cols + posx] += src[(posy + k - 1) * cols + (posx + l - 1)] * filter[k * 3 + l];
				//printf("Fuente = %i \n", src[(posy + k - 1) * cols + (posx + l - 1)]);
				//printf("Filtro = %i \n", filter[k * 3 + l]);

			}
		}
	}
	//printf("Destino = %i \n", dst[posy * cols + posx]);

}
__shared__ int smem[324];
__global__ void convolution2Kernel(int *dst, int *src, int rows, int cols, int *filter) {
	// Convolucion con memoria compartida
	int posx = threadIdx.x + blockIdx.x * blockDim.x;
	int posy = threadIdx.y + blockIdx.y * blockDim.y;
	//smem[threadIdx.y * (blockDim.x + 2) + threadIdx.x] = src[(idy - 1) * cols + (idx - 1)];
	if (threadIdx.x < 2 && threadIdx.y < 2) 
		smem[(threadIdx.y + blockDim.y) * (blockDim.x + 2) + threadIdx.x + blockDim.x] = src[(posy – 1 + blockDim.y) * cols + (posx – 1 + blockDim.x)];
}

void convolution(int *dst, int *src, int rows, int cols, int *filter) {
	size_t size = rows * cols * sizeof(int);
	size_t fsize = 3 * 3 * sizeof(int);

	int *d_src = NULL;
	int *d_filter = NULL;
	int *d_dst = NULL;

	//malloc en cuda para d_src, d_filter y d_dst
	cudaMalloc(&d_src, size);
	cudaMalloc(&d_filter, fsize);
	cudaMalloc(&d_dst, size);

	//copiar los datos de src y filter a d_src y d_filter respectivamente
	cudaMemcpy(d_src, src, size, cudaMemcpyHostToDevice);
	cudaMemcpy(d_filter, filter, fsize, cudaMemcpyHostToDevice);


	//Poner d_dst a ceros
	cudaMemset(d_dst, 0, size);

	//llamada al kernel
	dim3 threads(18, 18);
	dim3 blocks(1, 1);
	convolution2Kernel<<<blocks,threads>>>(d_dst, d_src, rows, cols, d_filter);

	//copiar los datos de vuelta. Los datos calculados y guardados en d_dst han de ser copiados a dst.
	cudaMemcpy(dst, d_dst, size, cudaMemcpyDeviceToHost);
	//liberar memoria que ya no se necesita
}
