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
#define min(a,b) (((a)<(b))?(a):(b))

/*
  A is M-by-K
  B is K-by-N
  C is M-by-N
  lda is the leading dimension of the matrix (the M of square_dgemm).
*/
void simd_dgemm_8n(const int lda, const int M, const int N, const int K,
                const double *restrict A, double *restrict B, 
                double *restrict C) {
    __m128d vA, vB1, vB2, vB3, vB4, vB5, vB6, vB7, vB8;
    __m128d vMul; 
    __m128d vRes1, vRes2, vRes3, vRes4, vRes5, vRes6, vRes7, vRes8;
    const int Kpadded = (K+(K%2));

    // Pack the B Matrix:
    // TODO: Refactor this.
    double bPacked[Kpadded*(N+(N%8))] __attribute__ ((aligned(16)));
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
    for (int col = N; col < (N+(N%8)); col++) {
        for (int row = 0; row < Kpadded; row++) {
            bPacked[col * Kpadded + row] = 0;
        }
    }

    double aPacked[Kpadded*M] __attribute__ ((aligned(16))); //tmp addon
    // Add padding to A:
    for (int col = K; col < Kpadded; col++) {
        for (int row = 0; row < M; row++) {
            aPacked[col + row * Kpadded] = 0;
        }
    }
    
    for (int i = 0; i < M; i+=MC) {
        // Pack the A Matrix::
        for (int col = 0; col < K; col++) {         // Entire column at a time.
            for (int row = i; row < min(M,i+MC); row++) {  // mc rows at a time.
                aPacked[col + row * Kpadded] = A[col * lda + row];
            }
        }
        // Now do the calculations:    
        for (int z = i; z < min(M,i+MC); z++) {
            // Unrolling access to the B matrix, since it is accessed
            // multiple time for every element in A:
            for (int j = 0; j < N; j+=8) {
                // If BLOCK_SIZE%4 != 0 this will most likely seg fault:
                vRes1 = _mm_load_sd(&C[z+j*lda]);
                vRes2 = _mm_load_sd(&C[z+(j+1)*lda]);            
                vRes3 = _mm_load_sd(&C[z+(j+2)*lda]);      
                vRes4 = _mm_load_sd(&C[z+(j+3)*lda]);
                vRes5 = _mm_load_sd(&C[z+(j+4)*lda]);
                vRes6 = _mm_load_sd(&C[z+(j+5)*lda]);
                vRes7 = _mm_load_sd(&C[z+(j+6)*lda]);
                vRes8 = _mm_load_sd(&C[z+(j+7)*lda]);
                for (int k = 0; k < K; k += 2) {
                    vA = _mm_load_pd(&aPacked[k + z * Kpadded]);
                    vB1 = _mm_load_pd(&bPacked[k + j * Kpadded]);
                    vB2 = _mm_load_pd(&bPacked[k + (j+1) * Kpadded]);
                    vB3 = _mm_load_pd(&bPacked[k + (j+2) * Kpadded]);
                    vB4 = _mm_load_pd(&bPacked[k + (j+3) * Kpadded]);
                    vB5 = _mm_load_pd(&bPacked[k + (j+4) * Kpadded]);
                    vB6 = _mm_load_pd(&bPacked[k + (j+5) * Kpadded]);
                    vB7 = _mm_load_pd(&bPacked[k + (j+6) * Kpadded]);
                    vB8 = _mm_load_pd(&bPacked[k + (j+7) * Kpadded]);
                    vMul = _mm_mul_pd(vA, vB1);
                    vRes1 = _mm_add_pd(vRes1, vMul);
                    vMul = _mm_mul_pd(vA, vB2);
                    vRes2 = _mm_add_pd(vRes2, vMul);
                    vMul = _mm_mul_pd(vA, vB3);
                    vRes3 = _mm_add_pd(vRes3, vMul);
                    vMul = _mm_mul_pd(vA, vB4);
                    vRes4 = _mm_add_pd(vRes4, vMul);
                    vMul = _mm_mul_pd(vA, vB5);
                    vRes5 = _mm_add_pd(vRes5, vMul);
                    vMul = _mm_mul_pd(vA, vB6);
                    vRes6 = _mm_add_pd(vRes6, vMul);
                    vMul = _mm_mul_pd(vA, vB7);
                    vRes7 = _mm_add_pd(vRes7, vMul);
                    vMul = _mm_mul_pd(vA, vB8);
                    vRes8 = _mm_add_pd(vRes8, vMul);
                }
                vRes1 = _mm_hadd_pd(vRes1, vRes1);
                vRes2 = _mm_hadd_pd(vRes2, vRes2);
                vRes3 = _mm_hadd_pd(vRes3, vRes3);       
                vRes4 = _mm_hadd_pd(vRes4, vRes4);
                vRes5 = _mm_hadd_pd(vRes5, vRes5);
                vRes6 = _mm_hadd_pd(vRes6, vRes6);
                vRes7 = _mm_hadd_pd(vRes7, vRes7);
                vRes8 = _mm_hadd_pd(vRes8, vRes8);         
                
                _mm_store_sd(&C[(z+j*lda)], vRes1);             
                _mm_store_sd(&C[(z+(j+1)*lda)], vRes2);
                _mm_store_sd(&C[(z+(j+2)*lda)], vRes3);
                _mm_store_sd(&C[(z+(j+3)*lda)], vRes4);
                _mm_store_sd(&C[(z+(j+4)*lda)], vRes5);
                _mm_store_sd(&C[(z+(j+5)*lda)], vRes6);
                _mm_store_sd(&C[(z+(j+6)*lda)], vRes7);
                _mm_store_sd(&C[(z+(j+7)*lda)], vRes8);
            }
	    }
    }
}


void simd_dgemm_4n(const int lda, const int M, const int N, const int K,
                const double *restrict A, double *restrict B, 
                double *restrict C) {
    __m128d vA, vB1, vB2, vB3, vB4, vMul, vRes1, vRes2, vRes3, vRes4; // Define 128bit registers.
    const int Kpadded = (K+(K%2));    // Adjust K length to account for padding:

    // Pack the B Matrix:
    // TODO: Refactor this.
    double bPacked[Kpadded*(N+(N%4))] __attribute__ ((aligned(16)));
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
    for (int col = N; col < (N+(N%4)); col++) {
        for (int row = 0; row < Kpadded; row++) {
            bPacked[col * Kpadded + row] = 0;
        }
    }

    double aPacked[Kpadded*M] __attribute__ ((aligned(16))); //tmp addon
    // Add padding to A:
    for (int col = K; col < Kpadded; col++) {
        for (int row = 0; row < M; row++) {
            aPacked[col + row * Kpadded] = 0;
        }
    }
    
    for (int i = 0; i < M; i+=MC) {
        // Pack the A Matrix::
        for (int col = 0; col < K; col++) {         // Entire column at a time.
            for (int row = i; row < min(M,i+MC); row++) {  // mc rows at a time.
                aPacked[col + row * Kpadded] = A[col * lda + row];
            }
        }
        // Now do the calculations:    
        for (int z = i; z < min(M,i+MC); z++) {
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
                _mm_store_sd(&C[(z+j*lda)], vRes1);             
                _mm_store_sd(&C[(z+(j+1)*lda)], vRes2);
                _mm_store_sd(&C[(z+(j+2)*lda)], vRes3);
                _mm_store_sd(&C[(z+(j+3)*lda)], vRes4);
            }
	    }
    }
}

void simd_dgemm_2n(const int lda, const int M, const int N, const int K,
                const double *restrict A, double *restrict B, 
                double *restrict C) {
    __m128d vA, vB1, vB2, vMul, vRes1, vRes2; // Define 128bit registers.
    const int Kpadded = (K+(K%2));    // Adjust K length to account for padding:
    double cAux[N+4] __attribute__ ((aligned(16)));

    // Pack the B Matrix:
    // TODO: Refactor this.
    double bPacked[Kpadded*(N+(N%4))] __attribute__ ((aligned(16)));
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
    for (int col = N; col < (N+(N%4)); col++) {
        for (int row = 0; row < Kpadded; row++) {
            bPacked[col * Kpadded + row] = 0;
        }
    }

    double aPacked[Kpadded*M] __attribute__ ((aligned(16))); //tmp addon
    // Add padding to A:
    for (int col = K; col < Kpadded; col++) {
        for (int row = 0; row < M; row++) {
            aPacked[col + row * Kpadded] = 0;
        }
    }
    
    for (int i = 0; i < M; i+=MC) {
        // Pack the A Matrix::
        for (int col = 0; col < K; col++) {         // Entire column at a time.
            for (int row = i; row < min(M,i+MC); row++) {  // mc rows at a time.
                aPacked[col + row * Kpadded] = A[col * lda + row];
            }
        }
        // Now do the calculations:    
        for (int z = i; z < min(M,i+MC); z++) {
            // Unrolling access to the B matrix, since it is accessed
            // multiple time for every element in A:
            for (int j = 0; j < N; j+=2) {
                // If BLOCK_SIZE%4 != 0 this will most likely seg fault:
                vRes1 = _mm_load_sd(&C[z+j*lda]);
                vRes2 = _mm_load_sd(&C[z+(j+1)*lda]);
                for (int k = 0; k < K; k += 2) {
                    vA = _mm_load_pd(&aPacked[k + z * Kpadded]);
                    vB1 = _mm_load_pd(&bPacked[k + j * Kpadded]);
                    vB2 = _mm_load_pd(&bPacked[k + (j+1) * Kpadded]);
                    vMul = _mm_mul_pd(vA, vB1);
                    vRes1 = _mm_add_pd(vRes1, vMul);
                    vMul = _mm_mul_pd(vA, vB2);
                    vRes2 = _mm_add_pd(vRes2, vMul);
                }
                vRes1 = _mm_hadd_pd(vRes1, vRes1);
                vRes2 = _mm_hadd_pd(vRes2, vRes2);           
                _mm_store_sd(&cAux[j], vRes1);
                _mm_store_sd(&cAux[(j+1)], vRes2);
            }
        // Ide: Smid resultater i en tmp C og til sidst smid 
        // for N og M over i korrekt C matrix. (Overskydende kommer
        // således ikke med over).
        // Move from aux matrix to result matrix:
        for (int j = 0; j < N; j++) {
             C[z+j*lda] = cAux[j];
        }
	    }
    }
}

void square_dgemm (int lda, double* A, double* B, double* C)
{
  /* For each block-row of A */ 
  for (int i = 0; i < lda; i += BLOCK_SIZE)
    /* For each block-column of B */
    for (int j = 0; j < lda; j += BLOCK_SIZE)
      /* Accumulate block dgemms into block of C */
      for (int k = 0; k < lda; k += BLOCK_SIZE)
      {
	    /* Correct block dimensions if block "goes off edge of" the matrix */
	    int M = min(BLOCK_SIZE, lda-i);
	    int N = min(BLOCK_SIZE, lda-j);
	    int K = min(BLOCK_SIZE, lda-k);

	    /* Perform individual block dgemm */
	    if (N%8 == 0)
            	simd_dgemm_8n(lda, M, N, K, A + i + k*lda, B + k + j*lda, C + i + j*lda);
	    else if (N%4 == 0)
            	simd_dgemm_4n(lda, M, N, K, A + i + k*lda, B + k + j*lda, C + i + j*lda);
            else
            	simd_dgemm_2n(lda, M, N, K, A + i + k*lda, B + k + j*lda, C + i + j*lda);
        //do_block(lda, M, N, K, A + i + k*lda, B + k + j*lda, C + i + j*lda);
      }
}
