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
#define BLOCK_SIZE 8
#endif

#define min(a, b) (((a)<(b))?(a):(b))

/*
  A is M-by-K
  B is K-by-N
  C is M-by-N
  lda is the leading dimension of the matrix (the M of square_dgemm).
*/

void basic_dgemm(int lda, int M, int N, int K, double *A, double *B, double *C) {
    /*
      To optimize this, think about loop unrolling and software
      pipelining.  Hint:  For the majority of the matmuls, you
      know exactly how many iterations there are (the block size)...
    */
    for (int i = 0; i < M; i++) {
        for (int j = 0; j < N; j++) {
            double cij = C[i + j * lda];
            #pragma GCC ivdep
            for( int k = 0; k < K; k++ )
                 cij += A[i+k*lda] * B[k+j*lda];
            C[i+j*lda] = cij;
        }
    }
}

void simd_dgemm(int lda, int M, int N, int K,
                double *A, double *B, double *C) {
    __m128d v1, v2, vMul, vRes; // Define 128bit registers.    

    for (int i = 0; i < M; i++) {
        for (int j = 0; j < N; j++) {
            vRes = _mm_setzero_pd();
            for (int k = 0; k < K; k += 2) {
                v1 = _mm_loadu_pd(&A[i + k * lda]);
                v2 = _mm_loadu_pd(&B[k + j * lda]);
                vMul = _mm_mul_pd(v1, v2);

                vRes = _mm_add_pd(vRes, vMul);
            }
            //vRes = _mm_hadd_pd(vRes, vRes);
            _mm_storeu_pd(&C[i + j * lda], vRes);
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
    */
    int M = min(BLOCK_SIZE, lda - i);
    int N = min(BLOCK_SIZE, lda - j);
    int K = min(BLOCK_SIZE, lda - k);

    /*
      Added note (Lars):
      Each iteration here, is iterating the missing indexes of the
      ones we skipped in square_dgemm.
    */
    if (K % 2 != 0 || M % 2 != 0 || N % 2 != 0) {
        basic_dgemm(lda, M, N, K, A + k + i * lda, B + k + j * lda, C + i + j * lda);
    } else {
        simd_dgemm(lda, M, N, K, A + k + i * lda, B + k + j * lda, C + i + j * lda);
    }
}

void square_dgemm(int M, double *A, double *B, double *C) {
    // Create transpose, this costs us some, but makes up in time
    // for bigger matrices. Note that this required a small change in
    // basic_dgemm when accessing the transposed matrix.
	double tmp[M*M];
    for (int i = 0; i < M; ++i) {
        for (int j = 0; j < M; ++j) {
            //Save transpose in tmp
			tmp[i+j*M] = B[j+i*M]; 
		}
	}
	
    // Now we do the original code with the transposed matrix in place of A.
    // A has to be the one transposed since the given matrices are column-major.
    for (int i = 0; i < M; i += BLOCK_SIZE) {
        for (int j = 0; j < M; j += BLOCK_SIZE) {
            for (int k = 0; k < M; k += BLOCK_SIZE) {
                do_block(M, A, tmp, C, i, j, k);
            }
        }
    }
}
