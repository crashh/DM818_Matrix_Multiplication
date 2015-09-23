const char* dgemm_desc = "Optimised, three-loop dgemm.";

/* 
    THIS IS JUST EXPERIMENTING, THIS IS NOT THE GIVEN ALGORTIHM
*/

/* This routine performs a optimised dgemm operation
 *  C := C + A * B
 * where A, B, and C are lda-by-lda matrices stored in column-major format.
 * On exit, A and B maintain their input values. */
 
 /* We transpose the B matrix instead, this yields additional calculations,
  * but the payoff is worth it for bigger matrices. */

// Set cache line size: 
#define CLS 32
#define SM (CLS / sizeof (double))
  
void square_dgemm (int n, double* A, double* B, double* C)
{
    /* For each row i of A */
        for (int i = 0; i < n; i += SM) {
	    /* For each column j of B */
		for (int j = 0; j < n; j += SM) {
		    /* Compute C */
			for (int k = 0; k < n; k += SM) {
			    /* Iterate missing indexes of previous loop I */
				for (int i2 = 0; i2 < SM; ++i2) {
				    double* rC = &C[i+j*n]; // Helping gcc compiler optimising
				    double* rA = &A[i+k*n]; // Helping gcc compiler optimising
				    rC += n; 
				    rA += n;    
				    /* Iterate missing indexes of previous loop K */
				    for (int k2 = 0; k2 < SM; ++k2) {
				        double* rB = &B[k+j*n]; // Helping gcc compiler optimising
				        rB += n;
				        /* Iterate missing indexes of previous loop J */
					    for (int j2 = 0; j2 < SM; ++j2) {
						    C[j2] += rA[k2] * rB[j2];
						}                        
				    }
			    }
		    }
	    }
    }
}
