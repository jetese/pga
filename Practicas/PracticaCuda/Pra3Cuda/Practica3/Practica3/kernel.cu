#include "kernel.h"

#include <cstdio>

__global__ void convolution1Kernel(int *dst, int *src, int rows, int cols, int *filter) {
	// Convolucion en memoria global, similar a la convolucion en CPU
}

__global__ void convolution2Kernel(int *dst, int *src, int rows, int cols, int *filter) {
	// Convolucion con memoria compartida
}

void convolution(int *dst, int *src, int rows, int cols, int *filter) {
	size_t size = rows * cols * sizeof(int);
	size_t fsize = 3 * 3 * sizeof(int);

	int *d_src = NULL;
	int *d_filter = NULL;
	int *d_dst = NULL;

	//malloc en cuda para d_src, d_filter y d_dst

	//copiar los datos de src y filter a d_src y d_filter respectivamente

	//Poner d_dst a ceros
	cudaMemset(d_dst, 0, size);

	//llamada al kernel

	//copiar los datos de vuelta. Los datos calculados y guardados en d_dst han de ser copiados a dst.

	//liberar memoria que ya no se necesita
}
