#ifndef KIBICHOBATCHPRIME_H
#define KIBICHOBATCHPRIME_H
/*
Prefix sum: https://en.algorithmica.org/hpc/algorithms/prefix/
Upto 64 bits the largest prime gap is (maybe) not bigger than 588. SO we bit pack upto 10 bits 
//Bitpacker from simdcomp:https://github.com/fast-pack/simdcomp/blob/master/src/avxbitpacking.c
*/
#include <stdio.h>
#include <stdint.h>
#include <assert.h>
#include <gmp.h>
#include <flint/flint.h>
#include <flint/fmpz.h>

#if defined(__x86_64__) || defined(__i386__)
#include <immintrin.h>
#define AVXBLOCKSIZE  256
#endif
typedef struct kibicho_primebatch_struct *KibichoBatchPrime;
struct kibicho_primebatch_struct 
{
	uint64_t normalPrimeOffset;/*Starting prime for normal*/
	uint64_t specialQOffset;/*Starting prime for specialQ*/
	size_t maxPrimeCount;
	uint32_t maxNormalGap;
	uint32_t maxSpecialQGap;
	uint8_t normalGapBits;
	uint8_t specialQGapBits;
	__m256i *normalCompressed;
};

//Taken from https://github.com/fast-pack/simdcomp/blob/master/src/avxbitpacking.c
/*static uint32_t maxbitas32int(const __m256i accumulator)
{
	const __m256i _tmp1 =_mm256_or_si256(_mm256_srli_si256(accumulator, 8), accumulator);
	const __m256i _tmp2 = _mm256_or_si256(_mm256_srli_si256(_tmp1, 4), _tmp1);
	uint32_t ans1 = _mm256_extract_epi32(_tmp2, 0);
	uint32_t ans2 = _mm256_extract_epi32(_tmp2, 4);
	uint32_t ans = ans1 | ans2;
	return bits(ans);
}

uint32_t Kibicho_AVXFindLargestBitWidthInArray(const uint32_t *begin)
{
	//begin is assumed to be an array of length AVXBLOCKSIZE  256
	const __m256i *pin = (const __m256i *)(begin);
	__m256i accumulator = _mm256_lddqu_si256(pin);
	uint32_t k = 1;
	for(; 8 * k < AVXBLOCKSIZE; ++k)
	{	
		//Loads 8 uint32_t, or all together
		__m256i newvec = _mm256_lddqu_si256(pin + k);
		accumulator = _mm256_or_si256(accumulator, newvec);
	}
	return maxbitas32int(accumulator);
}*/
static void avxpackblock5(const uint32_t *pin, __m256i *compressed);

KibichoBatchPrime Kibicho_CreateBatchPrimeList(size_t startNormalBitcount, size_t startSpecialQBitcount, size_t maxPrimeCount)
{
	//Ensure the size is a power of 2 and greater than 127
	assert(maxPrimeCount > 127 && (maxPrimeCount % AVXBLOCKSIZE) == 0 && (maxPrimeCount & (maxPrimeCount - 1)) == 0);

	KibichoBatchPrime kibichoBatchPrime = malloc(sizeof(struct kibicho_primebatch_struct));
	mpz_t normalPrime,specialQPrime;mpz_init(normalPrime);mpz_init(specialQPrime);
	//Set normal prime offset
	mpz_set_ui(normalPrime, 1);
	mpz_mul_2exp(normalPrime, normalPrime, startNormalBitcount); 
	mpz_nextprime(normalPrime, normalPrime);
	size_t bitsNormal = mpz_sizeinbase(normalPrime, 2);assert(bitsNormal < 64);
	kibichoBatchPrime->normalPrimeOffset = mpz_get_ui(normalPrime);
	
	//Set specialQ prime offset
	mpz_set_ui(specialQPrime, 1);
	mpz_mul_2exp(specialQPrime, specialQPrime, startSpecialQBitcount); 
	mpz_nextprime(specialQPrime, specialQPrime);
	size_t bitsSpecialQ = mpz_sizeinbase(specialQPrime, 2);assert(bitsSpecialQ < 64);
	kibichoBatchPrime->specialQOffset = mpz_get_ui(specialQPrime);
	
	//Find all primes
	uint32_t *normalPrimesGap = calloc(maxPrimeCount, sizeof(uint32_t));
	uint32_t *specialQPrimesGap = calloc(maxPrimeCount, sizeof(uint32_t));
	
	kibichoBatchPrime->maxPrimeCount  = maxPrimeCount;
	kibichoBatchPrime->maxNormalGap   = 0;
	kibichoBatchPrime->maxSpecialQGap = 0;
	uint64_t prevNormal   = kibichoBatchPrime->normalPrimeOffset;
	uint64_t prevspecialQ = kibichoBatchPrime->specialQOffset;
	for(size_t i = 0; i < kibichoBatchPrime->maxPrimeCount; i++)
	{
		mpz_nextprime(specialQPrime, specialQPrime);
		mpz_nextprime(normalPrime, normalPrime);
		
		uint64_t currentNormal = mpz_get_ui(normalPrime);
		uint64_t currentspecialQ = mpz_get_ui(specialQPrime);
		
		uint32_t normalGap   = currentNormal - prevNormal;
		uint32_t specialQGap = currentspecialQ - prevspecialQ;
		
		if(kibichoBatchPrime->maxNormalGap < normalGap)
		{
			kibichoBatchPrime->maxNormalGap = normalGap;
		}
		if(kibichoBatchPrime->maxSpecialQGap < specialQGap)
		{
			kibichoBatchPrime->maxSpecialQGap = specialQGap;
		}
		printf("(%5u, %5u): (%5u, %5u)\n", normalGap, kibichoBatchPrime->maxNormalGap, specialQGap, kibichoBatchPrime->maxSpecialQGap);
		//All primes are odd so we can divide the bit gaps by 2, ie save a bit per gap
		normalPrimesGap[i] = normalGap >> 1;
		specialQPrimesGap[i] = specialQGap >> 1;
		//Update previous
		prevNormal = currentNormal;
		prevspecialQ = currentspecialQ;
	}
	//Find bit widths
	kibichoBatchPrime->maxNormalGap   = kibichoBatchPrime->maxNormalGap >> 1;
	kibichoBatchPrime->maxSpecialQGap = kibichoBatchPrime->maxSpecialQGap >> 1;	

	kibichoBatchPrime->normalGapBits   = kibichoBatchPrime->maxNormalGap ? 64 - __builtin_clzll(kibichoBatchPrime->maxNormalGap) : 1;
	kibichoBatchPrime->specialQGapBits = kibichoBatchPrime->maxSpecialQGap ? 64 - __builtin_clzll(kibichoBatchPrime->maxSpecialQGap) : 1;
	size_t avxBlockCount = maxPrimeCount / AVXBLOCKSIZE;

	assert(kibichoBatchPrime->normalGapBits == 5);
	kibichoBatchPrime->normalCompressed =aligned_alloc(32, avxBlockCount * kibichoBatchPrime->normalGapBits * sizeof(__m256i));
	for(size_t block = 0;block < avxBlockCount;block++)
	{
		avxpackblock5(normalPrimesGap + block * 256,kibichoBatchPrime->normalCompressed + block * kibichoBatchPrime->normalGapBits);
	}
	free(normalPrimesGap);free(specialQPrimesGap);
	mpz_clear(normalPrime);mpz_clear(specialQPrime);
	return kibichoBatchPrime;
}

void Kibicho_PrintBatchPrimeList(KibichoBatchPrime kibichoBatchPrime)
{
	printf("normalPrimeOffset: %lu\n", kibichoBatchPrime->normalPrimeOffset);
	printf("specialQOffset   : %lu\n", kibichoBatchPrime->specialQOffset);
}

void Kibicho_DestroyBatchPrimeList(KibichoBatchPrime kibichoBatchPrime)
{
	free(kibichoBatchPrime->normalCompressed);
	free(kibichoBatchPrime);
}
static inline int Kibicho_EnsureAVX2()
{
	assert(__builtin_cpu_supports("avx2"));
	return 0;
}

/* we are going to pack 256 6-bit values, touching 6 256-bit words, using 96
 * bytes */
static void avxpackblock5(const uint32_t *pin, __m256i *compressed) {
  const __m256i *in = (const __m256i *)pin;
  /* we are going to touch  5 256-bit words */
  __m256i w0, w1;
  __m256i tmp; /* used to store inputs at word boundary */
  w0 = _mm256_lddqu_si256(in + 0);
  w0 = _mm256_or_si256(w0, _mm256_slli_epi32(_mm256_lddqu_si256(in + 1), 5));
  w0 = _mm256_or_si256(w0, _mm256_slli_epi32(_mm256_lddqu_si256(in + 2), 10));
  w0 = _mm256_or_si256(w0, _mm256_slli_epi32(_mm256_lddqu_si256(in + 3), 15));
  w0 = _mm256_or_si256(w0, _mm256_slli_epi32(_mm256_lddqu_si256(in + 4), 20));
  w0 = _mm256_or_si256(w0, _mm256_slli_epi32(_mm256_lddqu_si256(in + 5), 25));
  tmp = _mm256_lddqu_si256(in + 6);
  w0 = _mm256_or_si256(w0, _mm256_slli_epi32(tmp, 30));
  w1 = _mm256_srli_epi32(tmp, 2);
  _mm256_storeu_si256(compressed + 0, w0);
  w1 = _mm256_or_si256(w1, _mm256_slli_epi32(_mm256_lddqu_si256(in + 7), 3));
  w1 = _mm256_or_si256(w1, _mm256_slli_epi32(_mm256_lddqu_si256(in + 8), 8));
  w1 = _mm256_or_si256(w1, _mm256_slli_epi32(_mm256_lddqu_si256(in + 9), 13));
  w1 = _mm256_or_si256(w1, _mm256_slli_epi32(_mm256_lddqu_si256(in + 10), 18));
  w1 = _mm256_or_si256(w1, _mm256_slli_epi32(_mm256_lddqu_si256(in + 11), 23));
  tmp = _mm256_lddqu_si256(in + 12);
  w1 = _mm256_or_si256(w1, _mm256_slli_epi32(tmp, 28));
  w0 = _mm256_srli_epi32(tmp, 4);
  _mm256_storeu_si256(compressed + 1, w1);
  w0 = _mm256_or_si256(w0, _mm256_slli_epi32(_mm256_lddqu_si256(in + 13), 1));
  w0 = _mm256_or_si256(w0, _mm256_slli_epi32(_mm256_lddqu_si256(in + 14), 6));
  w0 = _mm256_or_si256(w0, _mm256_slli_epi32(_mm256_lddqu_si256(in + 15), 11));
  w0 = _mm256_or_si256(w0, _mm256_slli_epi32(_mm256_lddqu_si256(in + 16), 16));
  w0 = _mm256_or_si256(w0, _mm256_slli_epi32(_mm256_lddqu_si256(in + 17), 21));
  w0 = _mm256_or_si256(w0, _mm256_slli_epi32(_mm256_lddqu_si256(in + 18), 26));
  tmp = _mm256_lddqu_si256(in + 19);
  w0 = _mm256_or_si256(w0, _mm256_slli_epi32(tmp, 31));
  w1 = _mm256_srli_epi32(tmp, 1);
  _mm256_storeu_si256(compressed + 2, w0);
  w1 = _mm256_or_si256(w1, _mm256_slli_epi32(_mm256_lddqu_si256(in + 20), 4));
  w1 = _mm256_or_si256(w1, _mm256_slli_epi32(_mm256_lddqu_si256(in + 21), 9));
  w1 = _mm256_or_si256(w1, _mm256_slli_epi32(_mm256_lddqu_si256(in + 22), 14));
  w1 = _mm256_or_si256(w1, _mm256_slli_epi32(_mm256_lddqu_si256(in + 23), 19));
  w1 = _mm256_or_si256(w1, _mm256_slli_epi32(_mm256_lddqu_si256(in + 24), 24));
  tmp = _mm256_lddqu_si256(in + 25);
  w1 = _mm256_or_si256(w1, _mm256_slli_epi32(tmp, 29));
  w0 = _mm256_srli_epi32(tmp, 3);
  _mm256_storeu_si256(compressed + 3, w1);
  w0 = _mm256_or_si256(w0, _mm256_slli_epi32(_mm256_lddqu_si256(in + 26), 2));
  w0 = _mm256_or_si256(w0, _mm256_slli_epi32(_mm256_lddqu_si256(in + 27), 7));
  w0 = _mm256_or_si256(w0, _mm256_slli_epi32(_mm256_lddqu_si256(in + 28), 12));
  w0 = _mm256_or_si256(w0, _mm256_slli_epi32(_mm256_lddqu_si256(in + 29), 17));
  w0 = _mm256_or_si256(w0, _mm256_slli_epi32(_mm256_lddqu_si256(in + 30), 22));
  w0 = _mm256_or_si256(w0, _mm256_slli_epi32(_mm256_lddqu_si256(in + 31), 27));
  _mm256_storeu_si256(compressed + 4, w0);
}

#endif

