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
void square_dgemm (int n, double* A, double* B, double* C)
{
	// Create transpose:
	double tmp[n*n];
	for (int i = 0; i < n; ++i) {
		for (int j = 0; j < n; ++j) {
		    	//Save transpose in tmp
			tmp[i+j*n] = B[j+i*n]; 
		}
	}
	
	// Do the original calculation, now with the transpose:
	for (int i = 0; i < n; ++i) {
		for (int j = 0; j < n; ++j) {
		        /* Compute C(i,j) */
		        double cij = C[i+j*n];
	                for (int k = 0; k < n; ++k) {
				cij += A[i+k*n] * tmp[j+k*n];
		        }
		        C[i+j*n] = cij;
		}
	}
}
