/*
  In case you're wondering, dgemm stands for Double-precision, GEneral
  Matrix-Matrix multiplication.
*/
const char *dgemm_desc = "Simple blocked dgemm.";

#define BLOCK_SIZE_1 168
#define BLOCK_SIZE_2 56
#define BLOCK_SIZE_3 4
#define min(a, b) (((a) < (b)) ? (a) : (b))

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
            for (int k = 0; k < K; k++) {
                cij += A[i + k * lda] * B[k + j * lda];
            }
            C[i + j * lda] = cij;
        }
    }
}

void do_block(int lda, double *A, double *B, double *C, int i, int j, int k) {
    // TODO Deal with fringe blocks
    int M = min(BLOCK_SIZE_3, lda - i);
    int N = min(BLOCK_SIZE_3, lda - j);
    int K = min(BLOCK_SIZE_3, lda - k);

    /*
      Added note (Lars):
      Each iteration here, is iterating the missing indexes of the
      ones we skipped in square_dgemm.
     */
    basic_dgemm(lda, M, N, K,
                A + i + k * lda, // Pointer to A, B and C
                B + k + j * lda,
                C + i + j * lda);
}

void square_dgemm(int M, double *A, double *B, double *C) {
    for (int i = 0; i < M; i += BLOCK_SIZE_1) {
        for (int j = 0; j < M; j += BLOCK_SIZE_1) {
            for (int k = 0; k < M; k += BLOCK_SIZE_1) {

                for (int i2 = 0; i2 < BLOCK_SIZE_1; i2 += BLOCK_SIZE_2) {
                    for (int j2 = 0; j2 < BLOCK_SIZE_1; j2 += BLOCK_SIZE_2) {
                        for (int k2 = 0; k2 < BLOCK_SIZE_1; k2 += BLOCK_SIZE_2) {

                            for (int i3 = 0; i3 < BLOCK_SIZE_2; i3 += BLOCK_SIZE_3) {
                                for (int j3 = 0; j3 < BLOCK_SIZE_2; j3 += BLOCK_SIZE_3) {
                                    for (int k3 = 0; k3 < BLOCK_SIZE_2; k3 += BLOCK_SIZE_3) {
                                        // Probably going way past what we should
                                        do_block(BLOCK_SIZE_2, A, B, C,
                                                 i * BLOCK_SIZE_1 + i2 * BLOCK_SIZE_2 + i3 * BLOCK_SIZE_3,
                                                 j * BLOCK_SIZE_1 + j2 * BLOCK_SIZE_2 + j3 * BLOCK_SIZE_3,
                                                 k * BLOCK_SIZE_1 + k2 * BLOCK_SIZE_2 + k3 * BLOCK_SIZE_3);
                                    }
                                }
                            }

                        }
                    }
                }

            }
        }
    }
}

