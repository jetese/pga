#include <stdio.h>
#include "kernel.h"
#ifdef __cplusplus
extern "C" {
#endif
	void __declspec (dllimport) kernelArritmias(int x, int y);

#ifdef __cplusplus
}
#endif

int main(void) {
	kernelArritmias(2, 3);
	return 0;
}