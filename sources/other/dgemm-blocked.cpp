/*
  In case you're wondering, dgemm stands for Double-precision, GEneral
  Matrix-Matrix multiplication.
*/
#include <x86intrin.h>
#include <emmintrin.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

const char *dgemm_desc = "Simple blocked dgemm.";

#ifndef BLOCK_SIZE
#define BLOCK_SIZE 22
#endif

#define min(a,b) (((a)<(b))?(a):(b))

/*
  A is M-by-K
  B is K-by-N
  C is M-by-N

  lda is the leading dimension of the matrix (the M of square_dgemm).
*/

void basic_dgemm( int lda, int M, int N, int K,
                  double *A, double *B, double *C )
{
  /*
    To optimize this, think about loop unrolling and software
    pipelining.  Hint:  For the majority of the matmuls, you
    know exactly how many iterations there are (the block size)...
  */
  for( int i = 0; i < M; i++ )
       for( int j = 0; j < N; j++ ) 
       {
            double cij = C[i+j*lda];
            for( int k = 0; k < K; k++ )
                 cij += A[i+k*lda] * B[k+j*lda];
            C[i+j*lda] = cij;
       }
}

void simd_dgemm(int lda, int M, int N, int K,
                double *A, double *B, double *C) {
    __m128d v1, v2, vMul, vRes; // Define 128bit registers.
    
    // Pack the B Matrix:
    double bPacked[K*N] __attribute__ ((aligned(16)));
    int idx = 0;
    for (int col = 0; col < N; col++) {
        for (int row = 0; row < K; row++) {
            bPacked[idx++] = B[col * lda + row];
        }
    }

    int mc = 4;
    double aPacked[K*M] __attribute__ ((aligned(16)));
    for (int i = 0; i < M; i+=mc) {
    
        // Pack the A Matrix::
        for (int row = i; row < i+mc; row++) {        // mc rows at a time.
            for (int col = 0; col < K; col++) {       // Entire column at a time.
                aPacked[col + row * K] = A[col * lda + row];
            }
        }
    
        for (int i2 = i; i2 < i+mc; i2++) {
            for (int j = 0; j < N; j++) {
                const double cij[2] __attribute__ ((aligned (16))) = {C[i2+j*lda], 0};
                vRes = _mm_load_pd(cij);
                for (int k = 0; k < K; k += 2) {
                    v1 = _mm_load_pd(&aPacked[k + i2 * K]);
                    v2 = _mm_load_pd(&bPacked[k + j * K]);
                    vMul = _mm_mul_pd(v1, v2);

                    vRes = _mm_add_pd(vRes, vMul);
                }
                vRes = _mm_hadd_pd(vRes, vRes);
                _mm_store_sd(&C[i2 + j * lda], vRes);
            }
	}
    }
}

void square_dgemm( int M, double *A, double *B, double *C )
{
    for(int k = 0; k < M; k += BLOCK_SIZE)
        simd_dgemm( M, M, M, BLOCK_SIZE, A+k*BLOCK_SIZE, B+k*BLOCK_SIZE*M, C+k*BLOCK_SIZE);
}
