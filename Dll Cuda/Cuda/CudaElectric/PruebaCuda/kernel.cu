#include "kernel.h"
#include <stdio.h>
#include <cstdio>
#include <fstream>
#include <iostream>
#include <fstream>
#include <cstdlib>
#include "kernel.h"

#define SIZE 256
#define XSIZE (160)
#define YSIZE (400)
#define ZSIZE (1601)
//float voltajefinal[ZSIZE*YSIZE*XSIZE];


__global__ void initvolt(int *tamx, int *tamy, int *tamz, float* volt) {
	int posx = threadIdx.x + blockIdx.x * blockDim.x;

	if (posx < *tamz * *tamy * *tamx) {
		volt[posx] = 0.9;
	}
}

__global__ void module(int* numElectrodos, int *division, int * resto, int *indiceEgm, int * numTiempo, float *contentEgm, int * indexx, int * indexy, float * minValue, float * maxValue, float * auxLerp, float * maxScale, float * minScale, float* volt, int *tamx, int *tamy, int *tamz, int* indexvolt) {
	int pos = threadIdx.x + blockIdx.x * blockDim.x;
	if (pos < 88055) {
		resto[pos] = pos % *numElectrodos;
		division[pos] = pos / *numElectrodos;
		//x + WIDTH * (y + DEPTH * z)
		indiceEgm[pos] = resto[pos] * *numTiempo + division[pos];
		indexvolt[pos] = indexx[resto[pos]] + *tamx * (indexy[resto[pos]] + division[pos] * *tamy);
		
		auxLerp[pos] = (contentEgm[indiceEgm[pos]] + (-*minValue)) / (*maxValue + (-*minValue));
		volt[indexvolt[pos]] = (1 - auxLerp[pos]) * *minScale + auxLerp[pos] * *maxScale;
	}
}


#define TAM (88064) //Necesitamos un poco as porque trunca
#define THREADS_PER_BLOCK 256
#define TAM2 (102464000)
#define THREADS_PER_BLOCK2 1024
//int main(void) {
float* kernelArritmias(int numeroElectrodos, int sizePintado, int numeroTiempo, int sizeVoltaje, float * contenidoEgm, int *indicex, int *indicey, float valorMinimo, float valorMaximo, float escalaMaxima, float escalaMinima, float * prueba) {
	//Declaraci�n de variables
	float * voltajefinal;
	float *voltaje;
	int *indexvolt;//, indiceegmlol[88055];
	int xtam = XSIZE, ytam = YSIZE, ztam = ZSIZE;
	int * dev_xtam, *dev_ytam, *dev_ztam;
	float *dev_volt;

	int *dev_numeroElectrodos, *dev_division, *dev_resto, *dev_indiceEgm, *dev_numeroTiempo, *dev_indicex, *dev_indicey, *dev_indexvolt;
	float  *dev_contenidoEgm, *dev_valorMinimo, *dev_valorMaximo, *dev_escalaMinima, *dev_escalaMaxima, *dev_auxlerp;
	int size = sizePintado * sizeof(int);
	int sizefloat = sizePintado * sizeof(float);
	int sizevolt = sizeVoltaje * sizeof(float);
	int sizeElectros = numeroElectrodos * sizeof(int);

	voltajefinal = (float*)malloc(XSIZE*YSIZE*ZSIZE * sizeof(float));
	indexvolt = (int*)malloc(88055 * sizeof(int));
	cudaMalloc(&dev_volt, XSIZE*YSIZE*ZSIZE * sizeof(float));
	//cudaMemset(dev_volt, 0.9, XSIZE*YSIZE*ZSIZE * sizeof(float));
	cudaMalloc((void **)&dev_xtam, sizeof(int));
	cudaMalloc((void **)&dev_ytam, sizeof(int));
	cudaMalloc((void **)&dev_ztam, sizeof(int));

	cudaMalloc((void **)&dev_numeroElectrodos, sizeof(int));
	cudaMalloc((void **)&dev_division, size);
	cudaMalloc((void **)&dev_resto, size);
	cudaMalloc((void **)&dev_indiceEgm, size);
	cudaMalloc((void **)&dev_numeroTiempo, sizeof(int));
	cudaMalloc((void **)&dev_contenidoEgm, sizefloat);
	cudaMalloc((void **)&dev_indicex, sizeElectros);
	cudaMalloc((void **)&dev_indicey, sizeElectros);
	cudaMalloc((void **)&dev_valorMinimo, sizeof(float));
	cudaMalloc((void **)&dev_valorMaximo, sizeof(float));
	cudaMalloc((void **)&dev_escalaMinima, sizeof(float));
	cudaMalloc((void **)&dev_escalaMaxima, sizeof(float));
	cudaMalloc((void **)&dev_auxlerp, sizefloat);
	cudaMalloc((void **)&dev_indexvolt, 88055 * sizeof(int));


	cudaMemcpy(dev_xtam, &xtam, sizeof(int), cudaMemcpyHostToDevice);
	cudaMemcpy(dev_ytam, &ytam, sizeof(int), cudaMemcpyHostToDevice);
	cudaMemcpy(dev_ztam, &ztam, sizeof(int), cudaMemcpyHostToDevice);

	cudaMemcpy(dev_numeroElectrodos, &numeroElectrodos, sizeof(int), cudaMemcpyHostToDevice);
	cudaMemcpy(dev_numeroTiempo, &numeroTiempo, sizeof(int), cudaMemcpyHostToDevice);
	cudaMemcpy(dev_contenidoEgm, contenidoEgm, sizefloat, cudaMemcpyHostToDevice);
	cudaMemcpy(dev_indicex, indicex, sizeElectros, cudaMemcpyHostToDevice);
	cudaMemcpy(dev_indicey, indicey, sizeElectros, cudaMemcpyHostToDevice);
	cudaMemcpy(dev_valorMinimo, &valorMinimo, sizeof(float), cudaMemcpyHostToDevice);
	cudaMemcpy(dev_valorMaximo, &valorMaximo, sizeof(float), cudaMemcpyHostToDevice);
	cudaMemcpy(dev_escalaMinima, &escalaMinima, sizeof(float), cudaMemcpyHostToDevice);
	cudaMemcpy(dev_escalaMaxima, &escalaMaxima, sizeof(float), cudaMemcpyHostToDevice);

	//dim3 threadsPerBlock(8, 8,4); //16x16x1 threads = 256 < 1024
	//dim3 numBlocks(ZSIZE / threadsPerBlock.x, YSIZE/ threadsPerBlock.y,XSIZE/threadsPerBlock.z);

	cudaDeviceSynchronize();
	initvolt << < TAM2 / THREADS_PER_BLOCK, THREADS_PER_BLOCK >> >(dev_xtam, dev_ytam, dev_ztam, dev_volt);
	cudaDeviceSynchronize();
	//cudaMemcpy(voltajefinal, dev_volt, XSIZE*YSIZE*ZSIZE * sizeof(float), cudaMemcpyDeviceToHost);
	module << < TAM / THREADS_PER_BLOCK, THREADS_PER_BLOCK >> >(dev_numeroElectrodos, dev_division, dev_resto, dev_indiceEgm, dev_numeroTiempo, dev_contenidoEgm, dev_indicex, dev_indicey, dev_valorMinimo, dev_valorMaximo, dev_auxlerp, dev_escalaMaxima, dev_escalaMinima, dev_volt, dev_xtam, dev_ytam, dev_ztam, dev_indexvolt);

	cudaMemcpy(voltajefinal, dev_volt, XSIZE*YSIZE*ZSIZE * sizeof(float), cudaMemcpyDeviceToHost);

	//cudaMemcpy(indexvolt, dev_indexvolt, 88055 * sizeof(int), cudaMemcpyDeviceToHost);
	//cudaMemcpy(indiceegmlol, dev_indiceEgm, 88055 * sizeof(int), cudaMemcpyDeviceToHost);
	cudaDeviceSynchronize();
	cudaFree(dev_xtam);
	cudaFree(dev_ytam);
	cudaFree(dev_ztam);
	cudaFree(dev_volt);


	cudaFree(dev_numeroElectrodos);
	cudaFree(dev_division);
	cudaFree(dev_numeroTiempo);
	cudaFree(dev_resto);
	cudaFree(dev_indiceEgm);
	cudaFree(dev_contenidoEgm);
	cudaFree(dev_indicex);
	cudaFree(dev_indicey);
	cudaFree(dev_valorMinimo);
	cudaFree(dev_valorMaximo);
	cudaFree(dev_escalaMinima);
	cudaFree(dev_escalaMaxima);
	cudaFree(dev_auxlerp);

	return voltajefinal;
}
