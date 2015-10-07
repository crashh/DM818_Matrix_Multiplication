/*
  In case you're wondering, dgemm stands for Double-precision, General
  Matrix-Matrix multiplication.
*/
#include <x86intrin.h>
#include <emmintrin.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

const char *dgemm_desc = "Simple blocked dgemm.";
int BLOCK_SIZE;

//#ifndef BLOCK_SIZE
//#define BLOCK_SIZE 24
//#endif

#define MC 4
#define min(a,b) (((a)<(b))?(a):(b))

/*
  A is M-by-K
  B is K-by-N
  C is M-by-N
  lda is the leading dimension of the matrix (the M of square_dgemm).
*/
void simd_dgemm_4n(const int lda, const int M, const int N, const int K,
                const double *__restrict A, double *__restrict B, 
                double *__restrict C) {
    __m128d vA, vB1, vB2, vB3, vB4, vMul, vRes1, vRes2, vRes3, vRes4; // Define 128bit registers.
    const int Kpadded = (K+(K%2));    // Adjust K length to account for padding:
    double cAux[K*(N+4)] __attribute__ ((aligned(16)));
    
    // Pack the B Matrix:
    // TODO: Refactor this.
    double bPacked[Kpadded*(N+4)] __attribute__ ((aligned(16)));
    for (int col = 0; col < N; col++) {
        for (int row = 0; row < K; row++) {
            bPacked[col * Kpadded + row] = B[col * lda + row];
        }
        // Add padding to B for extra K elements:
        for (int row = K; row < Kpadded; row++) {
            bPacked[col * Kpadded + row] = 0;
        }
    }
    // More padding to B for extra N elements:
    for (int col = N; col < (N+4); col++) {
        for (int row = 0; row < Kpadded; row++) {
            bPacked[col * Kpadded + row] = 0;
        }
    }

    double aPacked[Kpadded*M] __attribute__ ((aligned(16)));
    // Add padding to A:
    for (int col = K; col < Kpadded; col++) {
        for (int row = 0; row < M; row++) {
            aPacked[col + row * Kpadded] = 0;
        }
    }

    for (int i = 0; i < M; i+=MC) {
    
        // Pack the A Matrix::
        for (int col = 0; col < K; col++) {         // Entire column at a time.
            for (int row = i; row < MC+i; row++) {  // mc rows at a time.
                if (row >= M) {break;}
                aPacked[col + row * Kpadded] = A[col * lda + row];
            }
        }

        // Now do the calculations:    
        for (int z = i; z < i+MC; z++) { // TODO: Should attempt to unroll this.
            if (z >= M) {break;}
            // Unrolling access to the B matrix, since it is accessed
            // multiple time for every element in A:
            for (int j = 0; j < N; j+=4) {
                // If BLOCK_SIZE%4 != 0 this will most likely seg fault:
                vRes1 = _mm_load_sd(&C[z+j*lda]);
                vRes2 = _mm_load_sd(&C[z+(j+1)*lda]);            
                vRes3 = _mm_load_sd(&C[z+(j+2)*lda]);      
                vRes4 = _mm_load_sd(&C[z+(j+3)*lda]);
                for (int k = 0; k < K; k += 2) {
                    vA = _mm_load_pd(&aPacked[k + z * Kpadded]);
                    vB1 = _mm_load_pd(&bPacked[k + j * Kpadded]);
                    vB2 = _mm_load_pd(&bPacked[k + (j+1) * Kpadded]);
                    vB3 = _mm_load_pd(&bPacked[k + (j+2) * Kpadded]);
                    vB4 = _mm_load_pd(&bPacked[k + (j+3) * Kpadded]);
                    vMul = _mm_mul_pd(vA, vB1);
                    vRes1 = _mm_add_pd(vRes1, vMul);
                    vMul = _mm_mul_pd(vA, vB2);
                    vRes2 = _mm_add_pd(vRes2, vMul);
                    vMul = _mm_mul_pd(vA, vB3);
                    vRes3 = _mm_add_pd(vRes3, vMul);
                    vMul = _mm_mul_pd(vA, vB4);
                    vRes4 = _mm_add_pd(vRes4, vMul);
                }
                vRes1 = _mm_hadd_pd(vRes1, vRes1);
                vRes2 = _mm_hadd_pd(vRes2, vRes2);
                vRes3 = _mm_hadd_pd(vRes3, vRes3);       
                vRes4 = _mm_hadd_pd(vRes4, vRes4);           
                _mm_store_sd(&cAux[z + j * K], vRes1);
                _mm_store_sd(&cAux[z + (j+1) * K], vRes2);             
                _mm_store_sd(&cAux[z + (j+2) * K], vRes3);
                _mm_store_sd(&cAux[z + (j+3) * K], vRes4);
                // Ide: Smid resultater i en tmp C og til sidst smid 
                // for N og M over i korrekt C matrix. (Overskydende kommer
                // således ikke med over).
            }
	    }
    }
    // Move from aux matrix to result matrix:
    for (int i = 0; i < M; i++) {
        for (int j = 0; j < N; j++) {
            C[i+j*lda] = cAux[i+j*K];
        }
    }
}

void do_block(int lda, double *A, double *B, double *C, int i, int j, int k) {
    int M = min(BLOCK_SIZE, lda - i);
    int N = min(BLOCK_SIZE, lda - j);
    int K = k;
    simd_dgemm_4n(lda, M, N, K, A + i, B + j * lda, C + i + j * lda);
}

void square_dgemm( int M, double *A, double *B, double *C )
{

     if(M<192) {BLOCK_SIZE = 100;}
     else if (M<319) {BLOCK_SIZE = 72;}
     else if (M<500) {BLOCK_SIZE = 60;}
     else if (M<650) {BLOCK_SIZE = 40;}
     else {BLOCK_SIZE = 36;}

     for( int i = 0; i < M; i += BLOCK_SIZE )
        for( int j = 0; j < M; j += BLOCK_SIZE )
            do_block( M, A, B, C, i, j, M );
}
