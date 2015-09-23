const char* dgemm_desc = "Optimised, three-loop dgemm.";

/* This routine performs a optimised dgemm operation
 *  C := C + A * B
 * where A, B, and C are lda-by-lda matrices stored in column-major format.
 * On exit, A and B maintain their input values. */
 
 /* We transpose the B matrix instead, this yields additional calculations,
  * but the payoff is worth it for bigger matrices. */

// Set cache line size: 
#define CLS  32
#define SM (CLS / sizeof (double))
  
void square_dgemm (int n, double* A, double* B, double* C)
{
    /* For each row i of A */
	for (i = 0; i < n; i += SM) {
	    /* For each column j of B */
		for (j = 0; j < n; j += SM) {
		    /* Compute C */
			for (k = 0; k < n; k += SM) {
			    /* Iterate missing indexes of previous loop I */
				for (i2 = 0; i2 < SM; ++i2) {
				    rA = &A[i][j]; // Helping gcc compiler optimising
				    rB = &B[i][k]; // Helping gcc compiler optimising
				    rA += n; 
				    rB += n;
				    /* Iterate missing indexes of previous loop K */
				    for (k2 = 0; k2 < SM; ++k2) {
				        rC = &C[k][j]; // Helping gcc compiler optimising
				        rC += n;
				        /* Iterate missing indexes of previous loop J */
					    for (j2 = 0; j2 < SM; ++j2) {
						    rA[j2] += rB[k2] * rC[j2];
						}
				    }
			    }
		    }
	    }
    }
}
