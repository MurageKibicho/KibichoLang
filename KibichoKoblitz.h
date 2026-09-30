/*
Filename:KibichoKoblitz.h
Usage: In exactly one C file, like where your main is,
       #define KIBICHO_KOBLITZ_IMPLEMENTATION
       #include "KibichoKoblitz.h"
*/
#ifndef KIBICHO_KOBLITZ_H
#define KIBICHO_KOBLITZ_H
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include <stdbool.h>
#include <time.h>
#include <flint/flint.h>
#include <flint/fmpz.h>
#include "stb_ds.h"
#define KOBLITZ_HASHTABLE_CAPACITY 10003
typedef struct koblitz_prime_curve_struct *KoblitzCurve;
typedef struct koblitz_prime_point_struct *KoblitzPoint;
typedef struct koblitz_prime_generator_struct *KoblitzPointGenerator;
typedef struct koblitz_hash_table_struct *Koblitz_HashTable;
typedef struct koblitz_hash_table_entry_struct *Koblitz_HashTableEntry;

struct koblitz_prime_point_struct
{
	fmpz_t x;fmpz_t y;fmpz_t z;int infinity;
};
struct koblitz_prime_curve_struct
{
	fmpz_t fieldCharacteristic;
	fmpz_t pointOrder;
	fmpz_t b;//ie y2=x3+b
	KoblitzPointGenerator generator;
};

struct koblitz_prime_generator_struct
{
	KoblitzPoint xyz;
	size_t cacheBitcount;
	KoblitzPoint *cache;
};

struct koblitz_hash_table_entry_struct
{
	fmpz_t key;
	fmpz_t value;
};

struct koblitz_hash_table_struct
{
	Koblitz_HashTableEntry **entry;//Handle collisions
	fmpz_t modulo;
	fmpz_t inverseY2;
	size_t capacity;
};
KoblitzPoint Koblitz_CreatePoint();
void Koblitz_PrintPoint(KoblitzPoint point);
bool Koblitz_IsValidPoint(fmpz_t x, fmpz_t y, fmpz_t b,fmpz_t p);
void Koblitz_CopyPoint(KoblitzPoint source, KoblitzPoint destination);
void Koblitz_DestroyPoint(KoblitzPoint point);
bool Koblitz_AddCurvePoints_XY(KoblitzPoint R, KoblitzPoint P, KoblitzPoint Q, fmpz_t primeNumber);
KoblitzPointGenerator Koblitz_CreateGenerator(fmpz_t pointOrder);
void Koblitz_SetGenerator_str(char *xString, char *yString, size_t baseString, KoblitzPointGenerator generator, KoblitzCurve curve);
void Koblitz_DestroyGenerator(KoblitzPointGenerator generator);
KoblitzCurve Koblitz_CreateCurve(char *fieldCharacteristicString, char *pointOrderString, char *curveBString, size_t baseFieldCharacteristic, size_t basePointOrder);
void Koblitz_DestroyCurve(KoblitzCurve curve);
Koblitz_HashTableEntry CreateEntry();
void Koblitz_FreeHashTableEntry(Koblitz_HashTableEntry entry);
Koblitz_HashTable Koblitz_CreateHashTable(size_t capacity);
void Koblitz_InitializeHashTable(Koblitz_HashTable table, fmpz_t x, fmpz_t y, fmpz_t temp0, fmpz_t prime);
void Koblitz_DestroyHashTable(Koblitz_HashTable table);
void Koblitz_PrintTableContents(Koblitz_HashTable table);
bool Koblitz_FindElementHashTable(Koblitz_HashTable table, fmpz_t search, fmpz_t temp0, fmpz_t result);
void Koblitz_SetElementHashTable(Koblitz_HashTable table, fmpz_t n, fmpz_t temp0, fmpz_t result);
void Koblitz_DivisionPolynomialRecursive(Koblitz_HashTable table, fmpz_t result, fmpz_t n, fmpz_t x, fmpz_t y, fmpz_t prime, fmpz_t temp0);
void Koblitz_DivisionPolynomial(Koblitz_HashTable table, fmpz_t result, fmpz_t n, fmpz_t x, fmpz_t y, fmpz_t prime, fmpz_t temp0);
void Koblitz_DivisionPolynomialABC(fmpz_t a, fmpz_t b, fmpz_t c, KoblitzPoint point, Koblitz_HashTable tableP, KoblitzCurve curve);

//Integers
Koblitz_HashTable LogarithmTable(fmpz_t base, fmpz_t ell, fmpz_t prime);
#ifdef KIBICHO_KOBLITZ_IMPLEMENTATION
#ifndef KIBICHO_KOBLITZ_IMPLEMENTATION_ONCE
#define KIBICHO_KOBLITZ_IMPLEMENTATION_ONCE
KoblitzPoint Koblitz_CreatePoint()
{
	KoblitzPoint point = malloc(sizeof(struct koblitz_prime_point_struct));
	fmpz_init(point->x);
	fmpz_init(point->y);
	fmpz_init(point->z);
	point->infinity = 0;
	return point;
}

void Koblitz_PrintPoint(KoblitzPoint point)
{
	fmpz_print(point->x);printf(" ");fmpz_print(point->y);printf("\n");
}

bool Koblitz_IsValidPoint(fmpz_t x, fmpz_t y, fmpz_t b,fmpz_t p)
{
	//y^2 = x^3 = b mod p
	fmpz_t lhs, rhs, x_cubed;fmpz_init(lhs);fmpz_init(rhs);fmpz_init(x_cubed);
	//Compute y^2 mod p
	fmpz_powm_ui(lhs, y, 2, p);
	//Compute x^3 mod p
	fmpz_powm_ui(x_cubed, x, 3, p);
	//Compute x^3 + b mod p
	fmpz_add(rhs, x_cubed, b);
	fmpz_mod(rhs, rhs, p);
	//Compare lhs and rhs
	bool result = (fmpz_cmp(lhs, rhs) == 0);
	//Clean up
	fmpz_clear(lhs);fmpz_clear(rhs);fmpz_clear(x_cubed);
	return result;
}

void Koblitz_CopyPoint(KoblitzPoint source, KoblitzPoint destination)
{
	fmpz_set(destination->x, source->x);
	fmpz_set(destination->y, source->y);
	fmpz_set(destination->z, source->z);
	destination->infinity = source->infinity;
}

void Koblitz_DestroyPoint(KoblitzPoint point)
{
	fmpz_clear(point->x);
	fmpz_clear(point->y);
	fmpz_clear(point->z);
	free(point);
}


bool Koblitz_AddCurvePoints_XY(KoblitzPoint R, KoblitzPoint P, KoblitzPoint Q, fmpz_t primeNumber)
{
	//Case 0: Handle Points at infinity
	if(P->infinity != 0){Koblitz_CopyPoint(Q,R);return true;}
	if(Q->infinity != 0){Koblitz_CopyPoint(P,R);return true;}
	
	//Case 1: Handle P->x == Q->x
	if(fmpz_cmp(P->x, Q->x) == 0) 
	{
		//Case 1.1: X values similar but Y differ or 0
		if(fmpz_cmp(P->y, Q->y) != 0 || fmpz_cmp_ui(P->y, 0) == 0){R->infinity = 1;return true;}
		//Case 1.2: Point Doubling
		fmpz_t s, num, den, denominatorInverse, tmp;
		fmpz_init(s);fmpz_init(num);fmpz_init(den);fmpz_init(denominatorInverse);fmpz_init(tmp);
		//num = x^2
		fmpz_mul(num, P->x, P->x);
		//num = 3x^2   
		fmpz_mul_ui(num, num, 3);       
		//den = 2y
		fmpz_mul_ui(den, P->y, 2); 
		//Find denominator inverse     
		if(!fmpz_invmod(denominatorInverse, den, primeNumber))
		{
			R->infinity = 1;
			fmpz_clear(s);fmpz_clear(num);fmpz_clear(den);fmpz_clear(denominatorInverse);fmpz_clear(tmp);
			return false;
		}
		//s = (3x^2)/(2y)
		fmpz_mul(s, num, denominatorInverse);fmpz_mod(s, s, primeNumber);
		//x3 = s^2 - 2x
		fmpz_mul(tmp, s, s);fmpz_sub(tmp, tmp, P->x);fmpz_sub(tmp, tmp, Q->x);fmpz_mod(R->x, tmp, primeNumber);

		//y3 = s*(x - x3) - y
		fmpz_sub(tmp, P->x, R->x);fmpz_mul(tmp, s, tmp);fmpz_sub(tmp, tmp, P->y);fmpz_mod(R->y, tmp, primeNumber);
		R->infinity = 0;
		fmpz_clear(s);fmpz_clear(num);fmpz_clear(den);fmpz_clear(denominatorInverse);fmpz_clear(tmp);
		return true;
	}
	//Case 2: Handle P != Q (Point Addition)
	else
	{
		fmpz_t s, num, den, denominatorInverse, tmp;
		fmpz_init(s);fmpz_init(num);fmpz_init(den);fmpz_init(denominatorInverse);fmpz_init(tmp);
		//num = y2 - y1
		fmpz_sub(num, Q->y, P->y);
		//den = x2 - x1   
		fmpz_sub(den, Q->x, P->x); 
		//Find inverse of denominator mod p    
		if(!fmpz_invmod(denominatorInverse, den, primeNumber))
		{
			R->infinity = 1;
			fmpz_clear(s);fmpz_clear(num);fmpz_clear(den);fmpz_clear(denominatorInverse);fmpz_clear(tmp);
			return true;
		}
		//s = (y2 - y1)/(x2 - x1) mod p
		fmpz_mul(s, num, denominatorInverse);fmpz_mod(s, s, primeNumber);
		//x3 = s^2 - x1 - x2 mod p
		fmpz_mul(tmp, s, s);fmpz_sub(tmp, tmp, P->x);fmpz_sub(tmp, tmp, Q->x);fmpz_mod(R->x, tmp, primeNumber);
		//y3 = s*(x1 - x3) - y1
		fmpz_sub(tmp, P->x, R->x);fmpz_mul(tmp, s, tmp);fmpz_sub(tmp, tmp, P->y);fmpz_mod(R->y, tmp, primeNumber);
		R->infinity = 0;
		//Free memory
		fmpz_clear(s);fmpz_clear(num);fmpz_clear(den);fmpz_clear(denominatorInverse);fmpz_clear(tmp);
		return true;
	}
}

void Koblitz_LSBCachedMultiplication(KoblitzPoint result, fmpz_t privateKey, int bitCount, KoblitzPoint *cachedDoubles, KoblitzPoint generator, fmpz_t primeNumber)
{
	//Assumes privateKey is positive
	//Create pointAtInfinity, temp0, temp1
	KoblitzPoint pointAtInfinity = Koblitz_CreatePoint();
	pointAtInfinity->infinity = 1;  
	
	KoblitzPoint temp0 = Koblitz_CreatePoint();
	//Save generator to temp0
	Koblitz_CopyPoint(generator, temp0); 
	KoblitzPoint temp1 = Koblitz_CreatePoint();

	size_t binaryLength = fmpz_sizeinbase(privateKey, 2);
	//loop from LSB to MSB
	for(size_t i = 0; i < binaryLength; ++i)
	{
		int bit = fmpz_tstbit(privateKey, i);
		if(bit)
		{
			//temp1 = pointAtInfinity + currentGenerator
			Koblitz_AddCurvePoints_XY(temp1, pointAtInfinity, cachedDoubles[i], primeNumber);
			Koblitz_CopyPoint(temp1, pointAtInfinity);
		}
	}
	//Save pointAtInfinity to the result variable
	Koblitz_CopyPoint(pointAtInfinity, result);
	Koblitz_DestroyPoint(pointAtInfinity);
	Koblitz_DestroyPoint(temp0);
	Koblitz_DestroyPoint(temp1);
}


KoblitzPointGenerator Koblitz_CreateGenerator(fmpz_t pointOrder)
{
	KoblitzPointGenerator generator = malloc(sizeof(struct koblitz_prime_generator_struct));
	generator->xyz = Koblitz_CreatePoint();
	generator->cacheBitcount = fmpz_sizeinbase(pointOrder, 2);
	generator->cache = malloc(generator->cacheBitcount * sizeof(KoblitzPoint));
	for(size_t  i = 0; i < generator->cacheBitcount; i++)
	{
		generator->cache[i] = Koblitz_CreatePoint();
	}
	return generator;
}

void Koblitz_SetGenerator_str(char *xString, char *yString, size_t baseString, KoblitzPointGenerator generator, KoblitzCurve curve)
{
	KoblitzPoint temp0 = Koblitz_CreatePoint();
	KoblitzPoint temp1 = Koblitz_CreatePoint();
	fmpz_set_str(generator->xyz->x, xString, baseString);
	fmpz_set_str(generator->xyz->y, yString, baseString);
	fmpz_set_ui(generator->xyz->z , 0);
	Koblitz_CopyPoint(generator->xyz, temp0);
	bool isValidGenerator = Koblitz_IsValidPoint(generator->xyz->x, generator->xyz->y, curve->b,curve->fieldCharacteristic);
	assert(isValidGenerator);
	for(size_t i = 0; i < generator->cacheBitcount; i++)
	{	
		//Store current power of 2 * G
		Koblitz_CopyPoint(temp0, generator->cache[i]); 
		//Double temp0 ie temp1 = temp0+temp1
		Koblitz_AddCurvePoints_XY(temp1, temp0, temp0, curve->fieldCharacteristic);		
		//Compute next power of 2: temp = 2 * temp
		Koblitz_CopyPoint(temp1, temp0);
	}
	Koblitz_DestroyPoint(temp0);
	Koblitz_DestroyPoint(temp1);
}

void Koblitz_DestroyGenerator(KoblitzPointGenerator generator)
{
	for(size_t  i = 0; i < generator->cacheBitcount; i++)
	{
		Koblitz_DestroyPoint(generator->cache[i]);
	}
	Koblitz_DestroyPoint(generator->xyz);
	free(generator->cache);
	free(generator);
}



KoblitzCurve Koblitz_CreateCurve(char *fieldCharacteristicString, char *pointOrderString, char *curveBString, size_t baseFieldCharacteristic, size_t basePointOrder)
{
	KoblitzCurve curve = malloc(sizeof(struct koblitz_prime_curve_struct));
	fmpz_init(curve->fieldCharacteristic);fmpz_init(curve->pointOrder);fmpz_init(curve->b);
	fmpz_set_str(curve->b, curveBString, 10);
	fmpz_set_str(curve->pointOrder, pointOrderString, basePointOrder);
	fmpz_set_str(curve->fieldCharacteristic, fieldCharacteristicString, baseFieldCharacteristic);
	curve->generator = Koblitz_CreateGenerator(curve->pointOrder);
	return curve;	
}

void Koblitz_DestroyCurve(KoblitzCurve curve)
{
	Koblitz_DestroyGenerator(curve->generator);
	fmpz_clear(curve->fieldCharacteristic);fmpz_clear(curve->pointOrder);fmpz_clear(curve->b);
	free(curve);
}


Koblitz_HashTableEntry CreateEntry()
{
	Koblitz_HashTableEntry entry = malloc(sizeof(struct koblitz_hash_table_entry_struct));
	fmpz_init(entry->key);
	fmpz_init(entry->value);
	return entry;
}

void Koblitz_FreeHashTableEntry(Koblitz_HashTableEntry entry)
{
	if(entry)
	{
		fmpz_clear(entry->key);
		fmpz_clear(entry->value);
		free(entry);
	}
}

Koblitz_HashTable Koblitz_CreateHashTable(size_t capacity)
{
	Koblitz_HashTable table = malloc(sizeof(struct koblitz_hash_table_struct));
	fmpz_init(table->modulo);
	fmpz_init(table->inverseY2);
	assert(capacity > 0);
	fmpz_set_ui(table->modulo, capacity);
	table->capacity = capacity;
	table->entry = malloc(capacity * sizeof(Koblitz_HashTableEntry*));
	for(size_t i = 0; i < capacity; i++)
	{
		table->entry[i] = NULL;
	}
	return table;
}

void Koblitz_DestroyHashTable(Koblitz_HashTable table)
{
	for(size_t i = 0; i < table->capacity; i++)
	{
		for(size_t j = 0; j < arrlen(table->entry[i]); j++)
		{
			Koblitz_FreeHashTableEntry(table->entry[i][j]);
		}
		arrfree(table->entry[i]);
	}
	fmpz_clear(table->inverseY2);
	fmpz_clear(table->modulo);
	free(table->entry);
	free(table);
}

void Koblitz_InitializeHashTable(Koblitz_HashTable table, fmpz_t x, fmpz_t y, fmpz_t temp0, fmpz_t prime)
{
	//Set y inverse square
	fmpz_mul_ui(table->inverseY2, y, 2);
	fmpz_invmod(table->inverseY2, table->inverseY2, prime);
	//n=0
	Koblitz_HashTableEntry entry0 = CreateEntry();
	fmpz_set_ui(entry0->key, 0);
	fmpz_set_ui(entry0->value, 0);
	arrput(table->entry[0],entry0);
	
	//n=1
	Koblitz_HashTableEntry entry1 = CreateEntry();
	fmpz_set_ui(entry1->key, 1);
	fmpz_set_ui(entry1->value, 1);
	arrput(table->entry[1],entry1);
	
	//n=2
	Koblitz_HashTableEntry entry2 = CreateEntry();
	fmpz_set_ui(entry2->key, 2);
	fmpz_mul_ui(entry2->value, y, 2);
	fmpz_mod(entry2->value, entry2->value, prime);
	arrput(table->entry[2],entry2);
	
	//n=3, ψ3 = 3x^4 + 84x
	Koblitz_HashTableEntry entry3 = CreateEntry();
	fmpz_set_ui(entry3->key, 3);
	fmpz_powm_ui(entry3->value, x, 4, prime);
	fmpz_mul_ui(entry3->value, entry3->value, 3);
	fmpz_mul_ui(temp0, x, 84);
	fmpz_add(entry3->value, entry3->value, temp0);
	fmpz_mod(entry3->value, entry3->value, prime);
	arrput(table->entry[3],entry3);
	
	//n=4, ψ₄ = 4y(x⁶ + 140x³ − 392)
	Koblitz_HashTableEntry entry4 = CreateEntry();
	fmpz_set_ui(entry4->key, 4);
	fmpz_powm_ui(temp0, x, 3, prime);
	fmpz_mul(entry4->value, temp0, temp0);
	
	fmpz_mul_ui(temp0, temp0, 140);
	fmpz_add(entry4->value, entry4->value, temp0);
	
	fmpz_sub_ui(entry4->value, entry4->value, 392);
	
	fmpz_mul(entry4->value, entry4->value, y);
	fmpz_mul_ui(entry4->value, entry4->value, 4);

	fmpz_mod(entry4->value, entry4->value, prime);
	arrput(table->entry[4],entry4);
	
}

void Koblitz_PrintTableContents(Koblitz_HashTable table)
{
	for(size_t i = 0; i < table->capacity; i++)
	{
		if(arrlen(table->entry[i]) > 0)
		{
			printf("Mod %3ld\n", i);
			for(size_t j = 0; j < arrlen(table->entry[i]); j++)
			{
				printf("\t");
				fmpz_print(table->entry[i][j]->key);
				printf(" : ");
				fmpz_print(table->entry[i][j]->value);
				printf("\n");
			}
		}
	}
}

bool Koblitz_FindElementHashTable(Koblitz_HashTable table, fmpz_t search, fmpz_t temp0, fmpz_t result)
{
	bool found = false;
	//Find modulo
	fmpz_mod(temp0, search, table->modulo);
	int searchIndex = fmpz_get_ui(temp0);
	for(size_t i = 0; i < arrlen(table->entry[searchIndex]); i++)
	{
		if(fmpz_cmp(search, table->entry[searchIndex][i]->key) == 0)
		{
			fmpz_set(result, table->entry[searchIndex][i]->value);
			found = true;
			break;
		}
	}
	return found;
}

void Koblitz_SetElementHashTable(Koblitz_HashTable table, fmpz_t n, fmpz_t temp0, fmpz_t result)
{
	//Find modulo
	fmpz_mod(temp0, n, table->modulo);
	int searchIndex = fmpz_get_ui(temp0);
	Koblitz_HashTableEntry entry = CreateEntry();
	fmpz_set(entry->key, n);
	fmpz_set(entry->value, result);
	arrput(table->entry[searchIndex],entry);
}

void Koblitz_DivisionPolynomialRecursive(Koblitz_HashTable table, fmpz_t result, fmpz_t n, fmpz_t x, fmpz_t y, fmpz_t prime, fmpz_t temp0)
{
	bool found = Koblitz_FindElementHashTable(table, n, temp0, result);
	if(found == true)
	{
		return;
	}
	else
	{
		//Recurse
		if(fmpz_is_odd(n))
		{
			fmpz_t k, oddTemp, psikMinus1, psik, psikPlus1, psikPlus2;
			fmpz_init(k);fmpz_init(oddTemp);fmpz_init(psikMinus1);fmpz_init(psik);fmpz_init(psikPlus1);fmpz_init(psikPlus2);
			fmpz_sub_ui(k, n, 1);
			fmpz_divexact_ui(k,n,2);
			
			//ψk-1
			fmpz_sub_ui(k, k , 1);
			Koblitz_DivisionPolynomialRecursive(table, psikMinus1, k, x, y, prime, oddTemp);	
		
			//ψk
			fmpz_add_ui(k, k , 1);
			Koblitz_DivisionPolynomialRecursive(table, psik, k, x, y, prime, oddTemp);	
		
			//ψk
			fmpz_add_ui(k, k , 1);
			Koblitz_DivisionPolynomialRecursive(table, psikPlus1, k, x, y, prime, oddTemp);	
		
			//ψk+2
			fmpz_add_ui(k,k , 1);
			Koblitz_DivisionPolynomialRecursive(table, psikPlus2, k, x, y, prime, oddTemp);	
	
			//term = (compute_psi(k+2)*pow(compute_psi(k),3,p) - compute_psi(k-1)*pow(compute_psi(k+1),3,p)) % p
			fmpz_powm_ui(psik, psik, 3, prime);
			fmpz_powm_ui(psikPlus1, psikPlus1, 3, prime);
			
			fmpz_mul(result, psikPlus2, psik);
			fmpz_mul(oddTemp,psikPlus1, psikMinus1);
			fmpz_sub(result, result, oddTemp);
			fmpz_mod(result, result, prime);
			
			fmpz_clear(k);fmpz_clear(oddTemp);fmpz_clear(psikMinus1);fmpz_clear(psik);fmpz_clear(psikPlus1);fmpz_clear(psikPlus2);
		}
		else
		{
			fmpz_t k, oddTemp, psikMinus2, psikMinus1, psik, psikPlus1, psikPlus2;
			fmpz_init(k);fmpz_init(oddTemp);fmpz_init(psikMinus2);fmpz_init(psikMinus1);fmpz_init(psik);fmpz_init(psikPlus1);fmpz_init(psikPlus2);
			
			fmpz_divexact_ui(k,n,2);
			
			//ψk-2
			fmpz_sub_ui(k, k , 2);
			Koblitz_DivisionPolynomialRecursive(table, psikMinus2, k, x, y, prime, oddTemp);	
		
			//ψk-1
			fmpz_add_ui(k, k , 1);
			Koblitz_DivisionPolynomialRecursive(table, psikMinus1, k, x, y, prime, oddTemp);	
		
			//ψk
			fmpz_add_ui(k, k , 1);
			Koblitz_DivisionPolynomialRecursive(table, psik, k, x, y, prime, oddTemp);	
		
			//ψk
			fmpz_add_ui(k, k , 1);
			Koblitz_DivisionPolynomialRecursive(table, psikPlus1, k, x, y, prime, oddTemp);	
		
			//ψk+2
			fmpz_add_ui(k,k , 1);
			Koblitz_DivisionPolynomialRecursive(table, psikPlus2, k, x, y, prime, oddTemp);	
	
			//numerator = (compute_psi(k+2)*pow(compute_psi(k-1),2,p) - compute_psi(k-2)*pow(compute_psi(k+1),2,p)) % p
           		//denom_inv = pow(2*y,p-2,p)
           		//term = (compute_psi(k)*numerator*denom_inv) % p
			fmpz_powm_ui(psikMinus1, psikMinus1, 2, prime);
			fmpz_powm_ui(psikPlus1, psikPlus1, 2, prime);
			
			fmpz_mul(psikMinus2, psikMinus2, psikPlus1);
			fmpz_mul(psikPlus2, psikPlus2, psikMinus1);
			
			fmpz_sub(psikPlus2, psikPlus2, psikMinus2);
			
			fmpz_mul(result, psik, psikPlus2);
			fmpz_mul(result, result, table->inverseY2);
			fmpz_mod(result, result, prime);
			
			fmpz_clear(k);fmpz_clear(oddTemp);fmpz_clear(psikMinus2);fmpz_clear(psikMinus1);fmpz_clear(psik);fmpz_clear(psikPlus1);fmpz_clear(psikPlus2);
		}
		//Store result 
		Koblitz_SetElementHashTable(table, n, temp0, result);
	}
}


void Koblitz_DivisionPolynomial(Koblitz_HashTable table, fmpz_t result, fmpz_t n, fmpz_t x, fmpz_t y, fmpz_t prime, fmpz_t temp0)
{
	if(fmpz_cmp_ui(n, 0) < 0)
	{
		fmpz_abs(n, n);
		Koblitz_DivisionPolynomialRecursive(table, result, n, x, y, prime, temp0);				
		fmpz_neg(result, result);
		fmpz_mod(result, result, prime);
		fmpz_neg(n, n);
	}
	else
	{
		Koblitz_DivisionPolynomialRecursive(table, result, n, x, y, prime, temp0);						
	}

}


void Koblitz_DivisionPolynomialABC(fmpz_t a, fmpz_t b, fmpz_t c, KoblitzPoint point, Koblitz_HashTable tableP, KoblitzCurve curve)
{
	fmpz_t psi_M_minus1_P, psi_M_minus2_P, psi_2_P,temp0,temp1;
	fmpz_init(psi_M_minus1_P);fmpz_init(psi_M_minus2_P);fmpz_init(psi_2_P);fmpz_init(temp0);fmpz_init(temp1);
	
	fmpz_sub_ui(temp1, curve->pointOrder, 1);
	Koblitz_DivisionPolynomial(tableP, psi_M_minus1_P, temp1, point->x, point->y, curve->fieldCharacteristic, temp0);	
	
	fmpz_sub_ui(temp1, curve->pointOrder, 2);
	Koblitz_DivisionPolynomial(tableP, psi_M_minus2_P, temp1, point->x, point->y, curve->fieldCharacteristic, temp0);	
	
	fmpz_set_ui(temp1, 2);
	Koblitz_DivisionPolynomial(tableP, psi_2_P, temp1, point->x, point->y, curve->fieldCharacteristic, temp0);	
	
	//a = (psi_M_minus2_P * pow(psi_M_minus1_P * psi_2_P,-1,p)) % p
	fmpz_mul(temp0, psi_M_minus1_P, psi_2_P);fmpz_invmod(temp0, temp0, curve->fieldCharacteristic);
	fmpz_mul(a, psi_M_minus2_P, temp0);fmpz_mod(a, a, curve->fieldCharacteristic);
	//c = (psi_M_minus1_P * psi_2_P * pow(psi_M_minus2_P,-1,p)) % p
	
	fmpz_invmod(temp0, psi_M_minus2_P, curve->fieldCharacteristic);
	fmpz_mul(temp0, temp0, psi_2_P);fmpz_mul(temp0, temp0, psi_M_minus1_P);
	fmpz_mod(c, temp0, curve->fieldCharacteristic);
	//b = (psi_M_minus1_P * c) % p
	fmpz_mul(temp0, c, psi_M_minus1_P);
	fmpz_mod(b, temp0, curve->fieldCharacteristic);
	
	fmpz_clear(psi_M_minus1_P);fmpz_clear(psi_M_minus2_P);fmpz_clear(psi_2_P);fmpz_clear(temp0);fmpz_clear(temp1);
}

/*Integers*/
Koblitz_HashTable LogarithmTable(fmpz_t base, fmpz_t ell, fmpz_t prime)
{
	Koblitz_HashTable lookup = Koblitz_CreateHashTable(KOBLITZ_HASHTABLE_CAPACITY);
	fmpz_t temp0,c_projection_ell, key,value;
	fmpz_init(temp0);fmpz_init(c_projection_ell);fmpz_init(key);fmpz_init(value);
	
	fmpz_sub_ui(c_projection_ell, prime, 1);
	fmpz_mod(temp0, c_projection_ell, ell);
	assert(fmpz_cmp_ui(temp0, 0) == 0);
	fmpz_divexact(c_projection_ell, c_projection_ell, ell);
	//Project to ell
	fmpz_powm(c_projection_ell, base, c_projection_ell,prime);
	fmpz_set_ui(key, 1);
	int maxCount = fmpz_get_ui(prime);
	for(int i = 0; i < maxCount; i++)
	{
		fmpz_set_ui(value, i);
		Koblitz_SetElementHashTable(lookup, key, temp0, value);	
		fmpz_mul(key, key, c_projection_ell);
		fmpz_mod(key, key, prime);
	}

	
	fmpz_clear(temp0);fmpz_clear(c_projection_ell);fmpz_clear(key);fmpz_clear(value);
	return lookup;	
}

void TestKoblitzCurve()
{
	char *fieldCharacteristicString = "209959";
	char *pointOrderString = "209317";
	char *curveBString = "7";
	char *xGeneratorString = "86301";
	char *yGeneratorString = "75133";
	size_t baseFieldCharacteristic = 10;
	size_t basePointOrder = 10;
	size_t baseGenerator = 10;
	KoblitzCurve curve = Koblitz_CreateCurve(fieldCharacteristicString, pointOrderString, curveBString, baseFieldCharacteristic, basePointOrder);
	Koblitz_SetGenerator_str(xGeneratorString, yGeneratorString, baseGenerator, curve->generator, curve);	
	
	
	//Test Division Polynomials 
	fmpz_t divA,divB,divC,temp0,temp1,k,psi_kp1;
	fmpz_init(divA);fmpz_init(divB);fmpz_init(divC);fmpz_init(temp0);fmpz_init(temp1);fmpz_init(k);fmpz_init(psi_kp1);
	Koblitz_HashTable tableP = Koblitz_CreateHashTable(KOBLITZ_HASHTABLE_CAPACITY);	
	Koblitz_InitializeHashTable(tableP, curve->generator->xyz->x, curve->generator->xyz->y, temp0, curve->fieldCharacteristic);
	//Test a, b, c constants
	Koblitz_DivisionPolynomialABC(divA,divB,divC, curve->generator->xyz, tableP, curve);	
	
	//Test negative ie temp1 = k-p+1
	fmpz_set_ui(k, 1843);
	fmpz_add_ui(temp1,temp1, 1);fmpz_sub(temp1,temp1,curve->fieldCharacteristic);
	Koblitz_DivisionPolynomial(tableP, psi_kp1, temp1, curve->generator->xyz->x, curve->generator->xyz->y, curve->fieldCharacteristic, temp0);
	
	//Test scalar multiplication
	
	KoblitzPoint kP = Koblitz_CreatePoint();
	Koblitz_LSBCachedMultiplication(kP, k, curve->generator->cacheBitcount, curve->generator->cache, curve->generator->xyz, curve->fieldCharacteristic);
	assert(Koblitz_IsValidPoint(kP->x, kP->y, curve->b, curve->fieldCharacteristic));	
	
	printf("a:");fmpz_print(divA);printf("\n");
	printf("b:");fmpz_print(divB);printf("\n");
	printf("c:");fmpz_print(divC);printf("\n");
	printf("k:");fmpz_print(k);printf("\n");
	printf("psi_kp1:");fmpz_print(psi_kp1);printf("\n");
	Koblitz_PrintPoint(kP);
	
	Koblitz_DestroyHashTable(tableP);Koblitz_DestroyPoint(kP);
	fmpz_clear(divA);fmpz_clear(divB);fmpz_clear(divC);fmpz_clear(temp0);fmpz_clear(temp1);fmpz_clear(k);fmpz_clear(psi_kp1);
	Koblitz_DestroyCurve(curve);
}
#endif /* KIBICHO_KOBLITZ_IMPLEMENTATION_ONCE */
#endif /* KIBICHO_KOBLITZ_IMPLEMENTATION */
#endif /* KIBICHO_KOBLITZ_H */   

