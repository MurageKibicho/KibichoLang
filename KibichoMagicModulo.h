#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <assert.h>
#include <gmp.h>
#include <time.h>
#include <flint/flint.h>
#include <flint/fmpz.h>

typedef struct kibicho_magicbatch_struct *KibichoBatchMagic;
typedef struct product_tree_struct *ProductTree;
struct product_tree_struct
{
	size_t inputLength;
	size_t treeLength;
	fmpz_t *product;
	fmpz_t *remainder;
};
struct kibicho_magicbatch_struct 
{
	ProductTree normalPrimes;
	ProductTree specialQPrimes;
	size_t maxPrimeCount;
};

ProductTree InitProductTree(size_t toMulLength)
{
	//Ensure toMulLength is power of 2
	assert(toMulLength > 0 && (toMulLength & (toMulLength - 1)) == 0);
	ProductTree productTree = malloc(sizeof(struct product_tree_struct));
	productTree->inputLength = toMulLength;
	productTree->treeLength  = 2 * toMulLength - 1;
	productTree->product = malloc(productTree->treeLength * sizeof(fmpz_t));
	productTree->remainder = malloc(productTree->treeLength * sizeof(fmpz_t));
	for(size_t i = 0; i < productTree->treeLength; i++)
	{
		fmpz_init(productTree->product[i]);
		fmpz_init(productTree->remainder[i]);
	}
	return productTree;
}

void SetRemainderTree_NaiveTreeTraversal(ProductTree productTree, fmpz_t remainder)
{
	fmpz_set(productTree->remainder[0], remainder);
	for(size_t i = 0; i < productTree->inputLength - 1; i++)
	{
		size_t left = 2 * i + 1;
		size_t right = left + 1;

		fmpz_mod(productTree->remainder[left],productTree->remainder[i],productTree->product[left]);
		fmpz_mod(productTree->remainder[right],productTree->remainder[i],productTree->product[right]);
	}
}
void SetRemainderNaive(ProductTree T, fmpz_t x)
{
	size_t leaf = T->inputLength - 1;
	int64_t xInt = fmpz_get_si(x);
	fmpz_set_si(x, xInt);
	fmpz_t temp0;fmpz_init(temp0);
	for(size_t i = 0; i < T->inputLength; i++)
	{
		fmpz_invmod(temp0, x, T->product[leaf + i]);
		int64_t p = fmpz_get_si(T->product[leaf + i]);
		int64_t remainder = xInt % p;
	}
	fmpz_clear(temp0);
}


void FreeProductTree(ProductTree productTree)
{
	for(size_t i = 0; i < productTree->treeLength; i++)
	{
		fmpz_clear(productTree->product[i]);
		fmpz_clear(productTree->remainder[i]);
	}
	free(productTree->product);
	free(productTree->remainder);
	free(productTree);
}
KibichoBatchMagic Kibicho_CreateBatchMagicList(size_t startNormalBitcount, size_t startSpecialQBitcount, size_t maxPrimeCount)
{
	KibichoBatchMagic kibichoBatchMagic = malloc(sizeof(struct kibicho_magicbatch_struct));
	kibichoBatchMagic->normalPrimes     = InitProductTree(maxPrimeCount);
	kibichoBatchMagic->specialQPrimes   = InitProductTree(maxPrimeCount);
	kibichoBatchMagic->maxPrimeCount = maxPrimeCount;
	
	mpz_t normalPrime,specialQPrime;mpz_init(normalPrime);mpz_init(specialQPrime);
	//Set normal prime offset
	mpz_set_ui(normalPrime, 1);
	mpz_mul_2exp(normalPrime, normalPrime, startNormalBitcount); 
	mpz_nextprime(normalPrime, normalPrime);
	
	//Set specialQ prime offset
	mpz_set_ui(specialQPrime, 1);
	mpz_mul_2exp(specialQPrime, specialQPrime, startSpecialQBitcount); 
	mpz_nextprime(specialQPrime, specialQPrime);
	
	size_t normalProductSize   = 0;
	size_t specialQProductSize = 0;
	size_t leaf = maxPrimeCount - 1;
	for(size_t i = 0; i < kibichoBatchMagic->maxPrimeCount; i++)
	{
		uint64_t currentNormal = mpz_get_ui(normalPrime);
		uint64_t currentSpecialQ = mpz_get_ui(specialQPrime);
		fmpz_set_mpz(kibichoBatchMagic->normalPrimes->product[leaf + i], normalPrime);
		fmpz_set_mpz(kibichoBatchMagic->specialQPrimes->product[leaf + i], specialQPrime);
				
		normalProductSize += currentNormal ? 64 - __builtin_clzll(currentNormal) : 0;
		specialQProductSize += currentSpecialQ ? 64 - __builtin_clzll(currentSpecialQ) : 0;
		
		mpz_nextprime(specialQPrime, specialQPrime);
		mpz_nextprime(normalPrime, normalPrime);
	}
	//Set parents
	for(size_t i = leaf; i-- > 0;)
	{
		fmpz_mul(kibichoBatchMagic->normalPrimes->product[i],kibichoBatchMagic->normalPrimes->product[2 * i + 1],kibichoBatchMagic->normalPrimes->product[2 * i + 2]);
		fmpz_mul(kibichoBatchMagic->specialQPrimes->product[i],kibichoBatchMagic->specialQPrimes->product[2 * i + 1],kibichoBatchMagic->specialQPrimes->product[2 * i + 2]);
	}
	//Set random remainders
	flint_rand_t state;flint_rand_init(state);
	fmpz_t randomNormal, randomSpecialQ;
	fmpz_init(randomNormal);fmpz_init(randomSpecialQ);
	fmpz_randbits(randomNormal, state, normalProductSize);
	fmpz_randbits(randomSpecialQ, state, specialQProductSize);
	
	printf("Sizes: %lu,%lu\n",normalProductSize, specialQProductSize);
	struct timespec t0, t1;
	clock_gettime(CLOCK_MONOTONIC, &t0);
	//SetRemainderTree_NaiveTreeTraversal(kibichoBatchMagic->normalPrimes, randomNormal);
	SetRemainderNaive(kibichoBatchMagic->normalPrimes, randomNormal);
	clock_gettime(CLOCK_MONOTONIC, &t1);
	double ms0 = (t1.tv_sec - t0.tv_sec) * 1000.0 + (t1.tv_nsec - t0.tv_nsec) / 1e6;
	printf("SetRemainderTree %lu: %.3f ms\n", normalProductSize, ms0);
	
	clock_gettime(CLOCK_MONOTONIC, &t0);
	//SetRemainderTree_NaiveTreeTraversal(kibichoBatchMagic->specialQPrimes, randomSpecialQ);
	SetRemainderNaive(kibichoBatchMagic->specialQPrimes, randomSpecialQ);
	clock_gettime(CLOCK_MONOTONIC, &t1);
	double ms1 = (t1.tv_sec - t0.tv_sec) * 1000.0 + (t1.tv_nsec - t0.tv_nsec) / 1e6;
	printf("SetRemainderTree %lu: %.3f ms\n", specialQProductSize, ms1);
	
	//printf("randomNormal: ");fmpz_print(randomNormal);printf("\n");
	//printf("randomSpecialQ: ");fmpz_print(randomSpecialQ);printf("\n");
	mpz_clear(normalPrime);mpz_clear(specialQPrime);
	flint_rand_clear(state);fmpz_clear(randomNormal);fmpz_clear(randomSpecialQ);
	return kibichoBatchMagic;










}

void Kibicho_DestroyBatchMagicList(KibichoBatchMagic kibichoBatchMagic)
{
	FreeProductTree(kibichoBatchMagic->normalPrimes);
	FreeProductTree(kibichoBatchMagic->specialQPrimes);
	free(kibichoBatchMagic);
}
