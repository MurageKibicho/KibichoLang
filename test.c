#define STB_DS_IMPLEMENTATION
#include "KibichoKoblitz.h"
//clear && gcc test.c -lm -lgmp -lflint -o m.o && ./m.o
int main()
{
	TestKoblitzCurve();
	flint_cleanup();
	return 0;
}
