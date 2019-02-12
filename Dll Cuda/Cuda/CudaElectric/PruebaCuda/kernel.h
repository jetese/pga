#ifdef TESTFUNCDLL_EXPORT
#define TESTFUNCDLL_API __declspec(dllexport) 
#else
#define TESTFUNCDLL_API __declspec(dllimport) 
#endif

extern "C" {
	TESTFUNCDLL_API extern "C" __declspec(dllexport) float* __cdecl kernelArritmias(int numeroElectrodos, int sizePintado, int numeroTiempo, int sizeVoltaje, float * contenidoEgm, int *indicex, int *indicey, float valorMinimo, float valorMaximo, float escalaMaxima, float escalaMinima);
}