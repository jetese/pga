#ifdef TESTFUNCDLL_EXPORT
#define TESTFUNCDLL_API __declspec(dllexport) 
#else
#define TESTFUNCDLL_API __declspec(dllimport) 
#endif

extern "C" {
	TESTFUNCDLL_API extern "C" __declspec(dllexport) float* __cdecl kernelArritmias(int sizeVoltaje, float * contenidoEgm, float valorMaximo, float valorMinimo, float escalaMaxima, float escalaMinima, float escalaMedia);
}