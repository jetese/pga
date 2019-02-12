#include <cstdio>
#include <cstdlib>
#include <string>

#include "kernel.h"

void initData(int *a, int w, int h);
void print(int *a, int w, int h);
void verify(int *a, int *b, int w, int h);
void convolution3x3(int *dst, const int *src, int w, int h, const int *filter);
	
int main() {
	printf("Hola Mundo!\n");

	// Tamaño de la imagen. Minimo 18
	int w = 18;
	int h = 18;
	int size = w * h * sizeof(int);

	int *src = (int*)malloc(size);
	int *filter = (int*)malloc(3*3*sizeof(int));
	int *dstcpu = (int*)malloc(size);
	int *dstgpu = (int*)malloc(size);

	filter[0] = 0; filter[1] = 1; filter[2] = 0;
	filter[3] = 1; filter[4] = 5; filter[5] = 1;
	filter[6] = 0; filter[7] = 1; filter[8] = 0;

	initData(src, w, h);
	memset(dstcpu, 0, size);

	printf("--------A---------\n");
	print(src, w, h);
	printf("--------C---------\n");
	print(filter, 3, 3);

	convolution3x3(dstcpu, src, w, h, filter);
	convolution(dstgpu, src, h, w, filter);

	printf("--------CPU---------\n");
	print(dstcpu, w, h);
	printf("--------GPU---------\n");
	print(dstgpu, w, h);
	printf("--------------------\n");

	verify(dstcpu, dstgpu, w, h);

	getchar();
	return (0);
}

void initData(int *a, int w, int h) {
	for (int i = 0; i < w*h; ++i) {
		a[i] = rand() % 100;
	}
}

void print(int *a, int w, int h) {
	for (int i = 0; i < h; ++i) {
		for (int j = 0; j < w; ++j) {
			printf("%2d ", a[i * w + j]);
		}
		printf("\n");
	}
}

void verify(int *a, int *b, int w, int h) {
	for (int i = 0; i < h; ++i) {
		for (int j = 0; j < w; ++j) {
			if (a[i*w+j] != b[i*w+j]) {
				printf("Fallo en (%d, %d), %d != %d.\n", i, j, a[i*w + j], b[i*w + j]);
				return;
			}
		}
	}
	printf("Test Correcto\n");
}

void convolution3x3(int *dst, const int *src, int w, int h, const int *filter) {
	for(int i = 1; i < h-1; ++i) {
		for (int j = 1; j < w-1; ++j) {
			// Convolucion CPU
		}
	}
}
