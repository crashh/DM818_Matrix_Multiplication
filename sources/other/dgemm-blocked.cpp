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
#define BLOCK_SIZE 64
#endif

#define MC 64
#define min(a, b) (((a) < (b)) ? (a) : (b))

#define LOAD_RES_REGISTER(offset) (                                         \
    vRes ## offset = _mm_load_sd(&C[z + (j + offset) * lda])                \
)

#define LOAD_B_VECTOR(offset) (                                             \
    vB ## offset = _mm_load_pd(&bPacked[k + (j + offset) * kPadded])        \
)

#define MUL_ADD(offset) {                                                   \
    vMul = _mm_mul_pd(vA, vB ## offset);                                    \
    vRes ## offset = _mm_add_pd(vRes ## offset, vMul);                      \
}

#define HADD(offset) (                                                      \
    vRes ## offset = _mm_hadd_pd(vRes ## offset, vRes ## offset)            \
)

#define STORE_IN_C_AUX(offset) (                                            \
    _mm_store_sd(&cAux[j + offset], vRes ## offset)                         \
)

#define STORE_DIRECTLY_IN_C(offset) (                                       \
    _mm_store_sd(&C[(z + (j + offset) * lda)], vRes ## offset)              \
)

#define DEF_B_REGISTER(offset)                                              \
    __m128d vB ## offset

#define DEF_RES_REGISTER(offset)                                            \
    __m128d vRes ## offset

#define REPEAT_2N(body)                                                     \
    body(0); body(1);

#define REPEAT_8N(body)                                                     \
    body(0); body(1); body(2); body (3);                                    \
    body(4); body(5); body(6); body (7);

inline void pack_B_matrix(double *__restrict bPacked, const int N, const int K,
                          const int kPadded, const int lda,
                          const double *__restrict B) {
    for (int col = 0; col < N; col++) {
        for (int row = 0; row < K; row++) {
            bPacked[col * kPadded + row] = B[col * lda + row];
        }
        // Add padding to B for extra K elements:
        for (int row = K; row < kPadded; row++) {
            bPacked[col * kPadded + row] = 0;
        }
    }
    // More padding to B for extra N elements:
    for (int col = N; col < (N + (N % 4)); col++) {
        for (int row = 0; row < kPadded; row++) {
            bPacked[col * kPadded + row] = 0;
        }
    }
}

inline void pack_A_matrix(double *__restrict aPacked, const int K, const int M,
                          const int kPadded, const int lda, const int i,
                          const double *__restrict A) {
    // Entire column at a time.
    for (int col = 0; col < K; col++) {
        // mc rows at a time.
        for (int row = i; row < min(M, i + MC); row++) {
            aPacked[col + row * kPadded] = A[col * lda + row];
        }
    }
}

inline void prepare_A_matrix(double *__restrict aPacked, const int K, const int M,
                             const int kPadded, const int lda) {
    // Add padding to A:
    for (int col = K; col < kPadded; col++) {
        for (int row = 0; row < M; row++) {
            aPacked[col + row * kPadded] = 0;
        }
    }
}

/*
  A is M-by-K
  B is K-by-N
  C is M-by-N
  lda is the leading dimension of the matrix (the M of square_dgemm).
*/
void simd_dgemm_8n(const int lda, const int M, const int N, const int K,
                   const double *__restrict A, double *__restrict B,
                   double *__restrict C) {
    // Define the registers needed for SSE computations
    __m128d vA, vMul;
    REPEAT_8N(DEF_B_REGISTER);
    REPEAT_8N(DEF_RES_REGISTER);

    // Adjust K length to account for padding
    const int kPadded = (K + (K % 2));

    double bPacked[kPadded * N] __attribute__ ((aligned(16)));
    pack_B_matrix(bPacked, N, K, kPadded, lda, B);

    double aPacked[kPadded * M] __attribute__ ((aligned(16)));
    prepare_A_matrix(aPacked, K, M, kPadded, lda);

    for (int i = 0; i < M; i += MC) {
        pack_A_matrix(aPacked, K, M, kPadded, lda, i, A);
        for (int z = i; z < min(M, i + MC); z++) {
            for (int j = 0; j < N; j += 8) {
                REPEAT_8N(LOAD_RES_REGISTER);
                for (int k = 0; k < K; k += 2) {
                    vA = _mm_load_pd(&aPacked[k + z * kPadded]);
                    REPEAT_8N(LOAD_B_VECTOR);
                    REPEAT_8N(MUL_ADD);
                }
                REPEAT_8N(HADD);
                REPEAT_8N(STORE_DIRECTLY_IN_C);
            }
        }
    }
}

void simd_dgemm_2n(const int lda, const int M, const int N, const int K,
                   const double *__restrict A, double *__restrict B,
                   double *__restrict C) {
    // Define the registers needed for SSE computations
    __m128d vA, vMul;
    REPEAT_2N(DEF_B_REGISTER);
    REPEAT_2N(DEF_RES_REGISTER);

    // Adjust K length to account for padding
    const int kPadded = (K + (K % 2));

    // We use an auxiliary matrix for C, to avoid having to do length checks in
    // the actual output matrix
    double cAux[N + 4] __attribute__ ((aligned(16)));

    double bPacked[kPadded * (N + (N % 4))] __attribute__ ((aligned(16)));
    pack_B_matrix(bPacked, N, K, kPadded, lda, B);

    double aPacked[kPadded * M] __attribute__ ((aligned(16)));
    prepare_A_matrix(aPacked, K, M, kPadded, lda);

    for (int i = 0; i < M; i += MC) {
        pack_A_matrix(aPacked, K, M, kPadded, lda, i, A);
        for (int z = i; z < min(M, i + MC); z++) {
            for (int j = 0; j < N; j += 2) {
                REPEAT_2N(LOAD_RES_REGISTER);
                for (int k = 0; k < K; k += 2) {
                    vA = _mm_load_pd(&aPacked[k + z * kPadded]);
                    REPEAT_2N(LOAD_B_VECTOR);
                    REPEAT_2N(MUL_ADD);
                }
                REPEAT_2N(HADD);
                REPEAT_2N(STORE_IN_C_AUX);
            }
            for (int j = 0; j < N; j++) {
                C[z + j * lda] = cAux[j];
            }
        }
    }
}

void square_dgemm(int lda, double *A, double *B, double *C) {
    // For each block-row of A
    for (int i = 0; i < lda; i += BLOCK_SIZE) {
        // For each block-column of B
        for (int j = 0; j < lda; j += BLOCK_SIZE) {
            // Accumulate block dgemms into block of C
            for (int k = 0; k < lda; k += BLOCK_SIZE) {
                // Correct block dimensions if block "goes off edge of"
                // the matrix
                int M = min(BLOCK_SIZE, lda - i);
                int N = min(BLOCK_SIZE, lda - j);
                int K = min(BLOCK_SIZE, lda - k);

                // Perform individual block dgemm
                if (N % 8 == 0) {
                    simd_dgemm_8n(lda, M, N, K, A + i + k * lda,
                                  B + k + j * lda, C + i + j * lda);
                } else {
                    simd_dgemm_2n(lda, M, N, K, A + i + k * lda,
                                  B + k + j * lda, C + i + j * lda);
                }
            }
        }
    }
}
