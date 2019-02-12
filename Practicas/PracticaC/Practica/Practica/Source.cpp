
#include <cstdio>
#include <stdio.h>
#include <stdlib.h>
#include<time.h>
struct estructura {
	char c;
	char c2;
	long l;
	double d;
};

void ejercicio1() {
	char c;
	int num = 3;
	//*****************EJERCICIO 1*****************
	printf("Los char ocupan %d bytes en memoria.\n", sizeof(char));
	printf("Los unsigned char ocupan %d bytes en memoria.\n", sizeof(unsigned char));
	printf("Los short int ocupan %d bytes en memoria.\n", sizeof(short int));
	printf("Los int ocupan %d bytes en memoria.\n", sizeof(int));
	printf("Los unsigned int ocupan %d bytes en memoria.\n", sizeof(unsigned int));
	printf("Los long ocupan %d bytes en memoria.\n", sizeof(long));
	printf("Los unsigned long ocupan %d bytes en memoria.\n", sizeof(unsigned long));
	printf("Los long long ocupan %d bytes en memoria.\n", sizeof(long long));
	printf("Los unsigned long ocupan %d bytes en memoria.\n", sizeof(unsigned long));
	printf("Los float ocupan %d bytes en memoria.\n", sizeof(float));
	printf("Los double ocupan %d bytes en memoria.\n", sizeof(double));
	printf("Los long double ocupan %d bytes en memoria.\n", sizeof(long double));
	printf("Los char* ocupan %d bytes en memoria.\n", sizeof(char*));
	printf("Los long long* ocupan %d bytes en memoria.\n", sizeof(long long*));
	printf("Los double* ocupan %d bytes en memoria.\n", sizeof(double*));
	printf("La variable num ocupa %d bytes en memoria.\n", sizeof(num));

	//a) Los unsigned afectan a la forma en que se distribuyen los números disponibles
	//b)Ocupan lo mismo (1 byte)
	//c)El puntero ocupa lo que la dirección de memoria (4 bytes)
}

void ejercicio2() {
	//*****************EJERCICIO 2*****************
	estructura estru;
	estructura *estra;

	printf("La variable struct ocupa %d bytes en memoria.\n", sizeof(estru));
	printf("La variable struct ocupa %d bytes en memoria.\n", sizeof(estra));

	//a)13
	//b) 16
	//c) 16
	//d) 4
}

void ejercicio3() {
	//*****************EJERCICIO 3*****************
	int num = 3;
	int *pi = &num;
	printf("A -> %d\n", num);
	printf("B -> %p\n", pi);
	printf("C -> %p\n", &num);
	printf("D -> %p\n", &pi);

	//a) Se imprime el número de num, la dirección de memoria de la variable num,
	//la dirección de memoria de num, la dirección de memoria de la variable pi.
	//b) Porque B es el valor de pi que es la dirección de memoria de num y C es la dirección de memoria de num. 

}

void ejercicio4() {
	//*****************EJERCICIO 4*****************
	char a[10];
	char * puntero;

	for (int i = 0; i < 10; i++) {
		a[i] = i;
	}

	puntero = a;
	for (int i = 0;i < 9;i++) {
		printf("El puntero apunta a ---> %d.\n", puntero);
		printf("El puntero recorre ---> %d byte.\n", puntero - (puntero - 1));
		printf("El valor al que apunta el puntero es ---> %d.\n", *puntero);
		puntero++;
	}

	printf("El puntero apunta a ---> %d.\n", *puntero);

	//a) Recorre 1 byte;
}

void ejercicio5() {
	//*****************EJERCICIO 5*****************

	int dasd[40];
	char * inpun = (char*)dasd;
	float * flopun = (float*)dasd;
	for (int i = 0; i < 10; i++) {
		dasd[i] = i;

		printf("El puntero inpun apunta a ---> %d.\n", inpun);
		printf("El puntero inpun recorre ---> %d byte.\n", inpun - (inpun - 1));
		printf("El valor al que apunta el puntero inpun es ---> %d.\n", *inpun);

		printf("El puntero flopun apunta a ---> %d.\n", flopun);
		printf("El puntero flopun recorre ---> %d byte.\n", flopun - (flopun - 1));
		printf("El valor al que apunta el puntero flopun es ---> %d.\n", *flopun);

		inpun++;
		flopun++;
	}
}

void ejercicio6() {
	int num = 16777216;

	//a) 1000000000000000000000000
	char *pun1 = (char*)&num;
	char *pun2 = (char*)&num +1;
	char *pun3 = (char*)&num +2;
	char *pun4 = (char*)&num +3;

	printf("El valor al que apunta el puntero pun1 es ---> %d.\n", *pun1);
	printf("El valor al que apunta el puntero pun2 es ---> %d.\n", *pun2);
	printf("El valor al que apunta el puntero pun3 es ---> %d.\n", *pun3);
	printf("El valor al que apunta el puntero pun4 es ---> %d.\n\n", *pun4);

	//Si concuerda pero está en little endian (0000000000000000000000001)
	num = 16777217;


	printf("El valor al que apunta el puntero pun1 es ---> %d.\n", *pun1);
	printf("El valor al que apunta el puntero pun2 es ---> %d.\n", *pun2);
	printf("El valor al que apunta el puntero pun3 es ---> %d.\n", *pun3);
	printf("El valor al que apunta el puntero pun4 es ---> %d.\n\n", *pun4);
}

void ejercicio7() {
	char c;
	int i2[2];
	double d;


	printf("La dirección de memoria del char es ---> %p.\n", &c);
	printf("La dirección de memoria del array[0] es ---> %p.\n", &i2[0]);
	printf("La dirección de memoria del array[1] es ---> %p.\n", &i2[1]);
	printf("La dirección de memoria del double es ---> %p.\n", &d);

	//0113F6	0	1	2	3	4	5	6	7	8	9	A	B	C	D	E	F				
	//	1	  [  	a[1]	  ]			   								   [c]
	//	0	   	              ]								   [     a[0]     ]								
	//	F													   [              d

	//00CFF9	0	1	2	3	4	5	6	7	8	9	A	B	C	D	E	F				
	//	2	  [  	a[1]	  ]											   [c]
	//	1				      ]								   [     a[0]     ]				
	//	0					   								   [           d   	

	//Las variables se colocan con las mismas distancias de memoria

	//En modo release al no meter información adicional puede disminuir la distancia de memoria entre variables

}

void ejercicio8() {
	int *pi = (int*)malloc(sizeof(char) * 10 + sizeof(long) * 10 + sizeof(double) * 10);

	char *pc = (char*)pi;
	long *pl = (long*)(pc+10);
	double *pd = (double*)(pl + 10);

	printf("El puntero pc apunta a ---> %p.\n", pc);
	printf("El puntero pl apunta a ---> %p.\n", pl);
	printf("El puntero pd apunta a ---> %p.\n", pd);

	//La distancia entre pc y pl sale 10
	//La distancia entre pl y pd sale 40
}

int add(int value, int increment) {
	value += increment;
	return value;
}
void add2(int* value, int increment) {
	*value += increment;
}
void ejercicio9(){
	int i = 3;
	i = add(i, 4);
	printf("i: %d\n", i);
	add2(&i, 4);
	printf("i: %d\n", i);
	//Se imprime mal porque la función add no tiene return e i no cambia su valor 
}

void mallocv2(int ** pe, int cantidad) {
	*pe = (int*)malloc(sizeof(char) * cantidad );
}
void ejercicio10() {
	int *pi;
	mallocv2(&pi, 10);
	printf("Es null? ---> %p.\n", pi);	
}

void vectorAdd(int c[10], int a[10], int b[10]) {
	for (int i = 0; i < 10; i++) {
		c[i] = a[i] + b[i];
	} 
}

void vectorAdd(int *c, int *a, int *b, int n) {
	srand(0);
	for (int i = 0; i < n; i++) {
		*a = abs(rand())%10;
		*b = abs(rand())%10;
		*c = *a + *b;
		a++;
		b++;
		c++;
	}
}


void ejercicio11() {
	srand(0);
	const int N = 10;
	int A[N];
	int B[N];
	int C[N];
	
	vectorAdd(&C[0], &A[0], &B[0], N);
	for (int i = 0; i < N; i++) {
		printf("A ---> %d.\n", A[i]);
	}
	for (int i = 0; i < N; i++) {
		printf("B ---> %d.\n", B[i]);
	}
	for (int i = 0; i < N; i++) {
		printf("C ---> %d.\n", C[i]);
	}
}

void vectorAdd(int *c, int *a, int *b, int n, int m) {
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < m; j++) {
			c[i*m + j] = a[i*m + j]+ b[i*m + j];
		}
	}
}

void ejercicio12() {
	srand(0);
	const int N = 6;
	const int M = 5;
	int *pa = (int*)malloc(sizeof(int)*N*M);
	int *pb = (int*)malloc(sizeof(int)*N*M);
	int *pc = (int*)malloc(sizeof(int)*N*M);
	for (int i = 0; i < N; i++) {
		for (int j = 0; j < M; j++) {
			pa[i*M+j] = abs(rand()) % 10;
			pb[i*M+j] = abs(rand()) % 10;
		}
	}
	vectorAdd(pc, pb, pa, N, M);

	for (int i = 0; i < N*M; i++) {
		printf("A ---> %d.\n", pa[i]);
		printf("B ---> %d.\n", pb[i]);
		printf("C ---> %d.\n", pc[i]);
	}
	//¿Qué byte corresponde con el elemento de la columna 3 y fila 4 ?  M*4+3
	//¿y de la fila 3 y columna 4 ?	M*3+4
	//¿Y si la matriz es de 37x45, columna 3 y fila 4 ?	45*4 + 3;
	//¿Y si la matriz es de 37x45, fila 3 y columna 4 ?	45*3 + 4;
}

int main(int arc, char *argv[]) {
	ejercicio1();
	ejercicio2();
	ejercicio3();
	ejercicio4();
	ejercicio5();
	ejercicio6();
	ejercicio7();
	ejercicio8();
	ejercicio9();
	ejercicio10();
	ejercicio11();
	ejercicio12();

	getchar();
	return (0);
}