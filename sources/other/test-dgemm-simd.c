const char* dgemm_desc = "Optimised, three-loop dgemm.";

/* This routine performs a optimised dgemm operation
 *  C := C + A * B
 * where A, B, and C are lda-by-lda matrices stored in column-major format.
 * On exit, A and B maintain their input values. */
 
 /* NOTE: This is example code, slightly modified.
  * Source: http://www.akkadia.org/drepper/cpumemory.pdf */
#include <stdlib.h>
#include <stdio.h>
#include <emmintrin.h>
double res[N][N] __attribute__ ((aligned (64)));
double mul1[N][N] __attribute__ ((aligned (64)));
double mul2[N][N] __attribute__ ((aligned (64)));

// Set cache line size:
#define SM (CLS / sizeof (double))

void square_dgemm (int n, double* A, double* B, double* C)
{
    int i, i2, j, j2, k, k2;
    /* The restrict keyword is a declaration of intent given by the 
     * programmer to the   compiler. It says that for the lifetime of the 
     * pointer, only it or a value directly derived from it (such as 
     * pointer + 1) will be used to access the object to which it points. 
     * This limits the effects of pointer aliasing, aiding optimizations. */
    double *restrict rres = A;
    double *restrict rmul1 = B;
    double *restrict rmul2 = C;
    for (i = 0; i < n; i += SM) {
        for (j = 0; j < n; j += SM) {
            for (k = 0; k < n; k += SM) {
                for (i2 = 0, rres = &res[i][j], rmul1 = &mul1[i][k]; i2 < SM;
                    ++i2, rres += n, rmul1 += n)
                {
                    _mm_prefetch (&rmul1[8], _MM_HINT_NTA);
                    for (k2 = 0, rmul2 = &mul2[k][j]; k2 < SM; ++k2, rmul2 += n)
                    {
                        __m128d m1d = _mm_load_sd (&rmul1[k2]);
                        m1d = _mm_unpacklo_pd (m1d, m1d);
                        for (j2 = 0; j2 < SM; j2 += 2)
                        {
                            __m128d m2 = _mm_load_pd (&rmul2[j2]);
                            __m128d r2 = _mm_load_pd (&rres[j2]);
                            _mm_store_pd (&rres[j2],
                                _mm_add_pd (_mm_mul_pd (m2, m1d), r2));
                        }
                    }
                }
            }
        }
    }
}
