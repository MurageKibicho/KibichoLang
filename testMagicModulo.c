#include "KibichoMagicModulo.h"
//clear && gcc testMagicModulo.c -lm -lgmp -lmpfr -lflint -mavx2 -O3 -march=native -o m.o && ./m.o
//Check divisions: perf stat -e cycles,instructions,arith.divider_active ./m.o
//Generate asm : gcc testMagicModulo.c -O3 -march=native -S -o testMagicModulo.s

void TestMagicModulo()
{
	size_t startNormalBitcount = 10;
	size_t startSpecialQBitcount = 62;
	size_t maxPrimeCount = 1 << 20;
	KibichoBatchMagic kibichoBatchMagic = Kibicho_CreateBatchMagicList(startNormalBitcount, startSpecialQBitcount, maxPrimeCount);
	Kibicho_DestroyBatchMagicList(kibichoBatchMagic);
}
int main()
{
	TestMagicModulo();
	flint_cleanup();
	return 0;
}

