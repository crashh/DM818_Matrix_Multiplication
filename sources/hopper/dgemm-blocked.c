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

#ifndef BLOCK_SIZE
#define BLOCK_SIZE 32
#endif

#define min(a,b) (((a)<(b))?(a):(b))

/*
  A is M-by-K
  B is K-by-N
  C is M-by-N
  lda is the leading dimension of the matrix (the M of square_dgemm).
*/
void simd_dgemm(const int lda, const int M, const int N, const int K,
                const double *restrict A, double *restrict B, 
                double *restrict C) {
    __m128d v1, v2, vMul, vRes; // Define 128bit registers.
    const int Kpadded = (K+(K%2));    // Adjust K length to account for padding:
    
    // Pack the B Matrix:
    double bPacked[Kpadded*N] __attribute__ ((aligned(16)));
    for (int col = 0; col < N; col++) {
        for (int row = 0; row < K; row++) {
            bPacked[col * Kpadded + row] = B[col * lda + row];
        }
        // Add padding to B:
        for (int row = K; row < Kpadded; row++) {
            bPacked[col * Kpadded + row] = 0;
        }
    }

    const int mc = 8;
    double aPacked[Kpadded*M] __attribute__ ((aligned(16)));
    // Add padding to A:
    for (int col = K; col < Kpadded; col++) {
        for (int row = 0; row < M; row++) {
            aPacked[col + row * Kpadded] = 0;
        }
    }

    for (int i = 0; i < M; i+=mc) {
    
        // Pack the A Matrix::
        for (int col = 0; col < K; col++) {         // Entire column at a time.
            for (int row = i; row < i+mc; row++) {  // mc rows at a time.
                if (row >= M) {break;}
                aPacked[col + row * Kpadded] = A[col * lda + row];
            }
        }

        // Now do the calculations:    
        for (int z = i; z < i+mc; z++) {
            if (z >= M) {break;}
            for (int j = 0; j < N; j++) {
                const double cij[2] __attribute__ ((aligned(16))) = {C[z+j*lda], 0};
                vRes = _mm_load_pd(cij);
                for (int k = 0; k < K; k += 2) {
                    v1 = _mm_load_pd(&aPacked[k + z * Kpadded]);
                    v2 = _mm_load_pd(&bPacked[k + j * Kpadded]);
                    vMul = _mm_mul_pd(v1, v2);
                    vRes = _mm_add_pd(vRes, vMul);
                }
                vRes = _mm_hadd_pd(vRes, vRes);
                _mm_store_sd(&C[z + j * lda], vRes);
            }
	    }
    }
}



void do_block(int lda, double *A, double *B, double *C, int i, int j, int k) {
    /*
      Remember that you need to deal with the fringes in each
      dimension.
      If the matrix is 7x7 and the blocks are 3x3, you'll have 1x3,
      3x1, and 1x1 fringe blocks.
            xxxoooX
            xxxoooX
            xxxoooX
            oooxxxO
            oooxxxO
            oooxxxO
            XXXOOOX
      You won't get this to go fast until you figure out a `better'
      way to handle the fringe blocks.  The better way will be more
      machine-efficient, but very programmer-inefficient.

      NOTE: Fringe cases should no longer be a problem, since new
      pattern looks like this:
            xxxoooo     xxxxxxx
            xxxoooo     xxxxxxx
            xxxoooo     ooooooo
            xxxoooo     ooooooo
            xxxoooo     ooooooo
            xxxoooo     ooooooo
            xxxoooo     ooooooo
      This does however make it harder to set constant values for
      BLOCK_SIZE combined with "mc", in order to fully utilize cache 
      at smaller matrix sizes, without thrashing at higher matrix 
      sizes.
    */
    int M = min(BLOCK_SIZE, lda - i);
    int N = min(BLOCK_SIZE, lda - j);
    int K = k;

    simd_dgemm(lda, M, N, K, A + i, B + j * lda, C + i + j * lda);
}

void square_dgemm( int M, double *A, double *B, double *C )
{
     for( int i = 0; i < M; i += BLOCK_SIZE )
        for( int j = 0; j < M; j += BLOCK_SIZE )
            do_block( M, A, B, C, i, j, M );
}
