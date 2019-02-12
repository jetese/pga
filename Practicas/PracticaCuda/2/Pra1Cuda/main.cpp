#include <cstdio>
#include <cstdlib>

#include <cmath>
#include <iostream>
#include "kernel.h"

void initData(float *a, float *b, int numElements) {
	for (int i = 0; i < numElements; i++) {
		a[i] = rand() / (float)RAND_MAX;
		b[i] = rand() / (float)RAND_MAX;
	}
};

void verify(float *a, float *b, float *c, int numElements) {
	for (int i = 0; i < numElements; i++) {
		if (fabs(a[i] + b[i] - c[i]) > 1e-5) {
			printf("ERROR en %f: Suma Incorrecta\n",i);
			return;
		}
	}
	printf("Test correcto!\n");
};

int main(int arc, char *argv[]) {
	printf("Hola Mundo!\n");
	int M = 4, N = 4;
	int numElements = 16;
	size_t size = numElements * sizeof(float);

	float *h_a = (float*)malloc(size);
	float *h_b = (float*)malloc(size);
	float *h_c = (float*)malloc(size);

	initData(h_a, h_b, M*N);

	vectorAdd(h_a,h_b,h_c,M,N);
	//::cout << h_c[9] << std::endl;
	verify(h_a, h_b, h_c, M*N);

	getchar();

	free(h_a);
	free(h_b);
	free(h_c);

	return (0);
}