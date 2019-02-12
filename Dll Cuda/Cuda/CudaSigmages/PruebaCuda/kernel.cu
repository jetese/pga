#include "kernel.h"
#include <stdio.h>
#include <cstdio>
#include <fstream>
#include <iostream>
#include <fstream>
#include <cstdlib>
#include "kernel.h"


__global__ void module(int *indiceEgm, float *contentEgm, float * minValue, float * maxValue, float * auxLerp, float * maxScale, float * minScale, float* midScale, float * volt) {
	int pos = threadIdx.x + blockIdx.x * blockDim.x;
	if (pos < *indiceEgm) {
		auxLerp[pos] = (contentEgm[pos] + (-*minValue)) / (*maxValue + (-*minValue));
		volt[pos] = (1 - auxLerp[pos]) * *minScale + auxLerp[pos] * *maxScale;
	}
}




#define THREADS_PER_BLOCK 256

float * kernelArritmias(int sizeVoltaje, float * contenidoEgm, float valorMaximo, float valorMinimo, float escalaMaxima, float escalaMinima, float escalaMedia) {
	//Declaraci�n de variables
	float * voltajefinal;

	int *dev_indiceEgm;
	float  *dev_contenidoEgm, *dev_valorMinimo, *dev_valorMaximo, *dev_escalaMinima, *dev_escalaMaxima, *dev_auxlerp, *dev_escalaMedia, *dev_volt;

	int sizevolt = sizeVoltaje * sizeof(float);

	voltajefinal = (float*)malloc(sizevolt);


	cudaMalloc((void **)&dev_indiceEgm, sizeof(int));
	cudaMalloc((void **)&dev_contenidoEgm, sizevolt);
	cudaMalloc((void **)&dev_valorMinimo, sizeof(float));
	cudaMalloc((void **)&dev_valorMaximo, sizeof(float));
	cudaMalloc((void **)&dev_escalaMedia, sizeof(float));
	cudaMalloc((void **)&dev_escalaMinima, sizeof(float));
	cudaMalloc((void **)&dev_escalaMaxima, sizeof(float));
	cudaMalloc((void **)&dev_auxlerp, sizevolt);
	cudaMalloc((void **)&dev_volt, sizevolt);

	cudaMemcpy(dev_indiceEgm, &sizeVoltaje, sizeof(int), cudaMemcpyHostToDevice);
	cudaMemcpy(dev_contenidoEgm, contenidoEgm, sizevolt, cudaMemcpyHostToDevice);
	cudaMemcpy(dev_valorMinimo, &valorMinimo, sizeof(float), cudaMemcpyHostToDevice);
	cudaMemcpy(dev_valorMaximo, &valorMaximo, sizeof(float), cudaMemcpyHostToDevice);
	cudaMemcpy(dev_escalaMinima, &escalaMinima, sizeof(float), cudaMemcpyHostToDevice);
	cudaMemcpy(dev_escalaMaxima, &escalaMaxima, sizeof(float), cudaMemcpyHostToDevice);
	cudaMemcpy(dev_escalaMedia, &escalaMedia, sizeof(float), cudaMemcpyHostToDevice);

	int nblocks = sizeVoltaje / THREADS_PER_BLOCK + sizeVoltaje % THREADS_PER_BLOCK;

	module << < nblocks, THREADS_PER_BLOCK >> >(dev_indiceEgm, dev_contenidoEgm, dev_valorMinimo, dev_valorMaximo, dev_auxlerp, dev_escalaMaxima, dev_escalaMinima, dev_escalaMedia, dev_volt);

	cudaMemcpy(voltajefinal, dev_volt, sizevolt, cudaMemcpyDeviceToHost);

	cudaFree(dev_indiceEgm);
	cudaFree(dev_contenidoEgm);
	cudaFree(dev_valorMinimo);
	cudaFree(dev_valorMaximo);
	cudaFree(dev_escalaMinima);
	cudaFree(dev_escalaMaxima);
	cudaFree(dev_escalaMedia);
	cudaFree(dev_auxlerp);
	cudaFree(dev_volt);

	return voltajefinal;
}
