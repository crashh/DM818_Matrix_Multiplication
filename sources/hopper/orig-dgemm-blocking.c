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
#define BLOCK_SIZE 38
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
            for (int k = 0; k < K; k++)
                cij += A[i + k * lda] * B[k + j * lda];
            C[i + j * lda] = cij;
        }
    }
}

/**
 * A is M-by-K (height-by-width)
 * B is K-by-N
 * C is M-by-N
 */
void blocked_gepp(int lda, int M, int N, int K, double *A, double *B, double *C) {
    int nr = 4; // The width of each B submatrix (Thus each submatrix will be nr * K)

    // Pack B into a contiguous column-major matrix
    // TODO inline and align and other performance stuff
    double *bPacked = malloc(sizeof(double) * K * N + // allocate space for the part existing in B
                                     sizeof(double) * ((N % nr) * K)); // allocate space for additional padding
    int idx = 0;
    for (int col = 0; col < N; col++) {
        for (int row = 0; row < K; row++) {
            bPacked[idx++] = B[col * lda + row];
        }
    }
    for (int col = K * N; col < K * N + (N % nr) * K; col++) { // Padding
        for (int row = 0; row < K; row++) { // TODO unroll
            bPacked[idx++] = 0;
        }
    }

    for (int i = 0; i < M; i += K) {
        // Pack A[i] (i.e. the ith submatrix, not some element) into a contiguous row-major work matrix
        double *aPacked = malloc(sizeof(double) * K * N);
        idx = 0;
        int rows = min(M, K);
        for (int row = 0; row < rows; row++) {
            for (int col = 0; col < N; col++) {
                aPacked[idx++] = A[col * lda + row + i];
            }
        }
        for (int row = M - i; row < K; row++) { // Add padding
            for (int col = 0; col < N; col++) { // TODO Unroll
                aPacked[idx++] = 0;
            }
        }

        for (int j = 0; j < N; j++) {
            // TODO Multiply A[i] * B[j]
        }
    }
}

void do_block(int lda, double *A, double *B, double *C, int i, int j, int k) {
    // TODO Change the way blocking is done, to match the Goto paper.

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

    if (K % 2 != 0 || M % 2 != 0 || N % 2 != 0) {
        basic_dgemm(lda, M, N, K, A + k + i * lda, B + k + j * lda, C + i + j * lda);
    } else {
        blocked_gepp(lda, M, N, K, A + k + i * lda, B + k + j * lda, C + i + j * lda);
    }
}

void square_dgemm(int M, double *A, double *B, double *C) {
    for (int i = 0; i < M; i += BLOCK_SIZE) {
        for (int j = 0; j < M; j += BLOCK_SIZE) {
            for (int k = 0; k < M; k += BLOCK_SIZE) {
                do_block(M, A, B, C, i, j, k);
            }
        }
    }
}
