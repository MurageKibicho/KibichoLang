#include "KibichoBatchPrime.h"
//clear && gcc testBatchPrime.c -lm -lgmp -lmpfr -lflint -mavx2 -O3 -march=native -o m.o && ./m.o

void TestBatchPrime()
{
	Kibicho_EnsureAVX2();
	size_t startNormalBitcount = 10;
	size_t startSpecialQBitcount = 62;
	size_t maxPrimeCount = 1 << 10;
	KibichoBatchPrime kibichoBatchPrime = Kibicho_CreateBatchPrimeList(startNormalBitcount, startSpecialQBitcount, maxPrimeCount);
	Kibicho_DestroyBatchPrimeList(kibichoBatchPrime);
}
int main()
{
	TestBatchPrime();
	flint_cleanup();
	return 0;
}

