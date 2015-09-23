const char* dgemm_desc = "Optimised, three-loop dgemm.";

/* This routine performs a optimised dgemm operation
 *  C := C + A * B
 * where A, B, and C are lda-by-lda matrices stored in column-major format.
 * On exit, A and B maintain their input values. */
 
 /* We transpose the B matrix instead, this yields additional calculations,
  * but the payoff is worth it for bigger matrices. */
void square_dgemm (int n, double* A, double* B, double* C)
{
	// Create transpose:
	double tmp[n][n];
	for (int i = 0; i < n; ++i) {
		for (int j = 0; j < n; ++j) {
		    //Save transpose in tmp
			tmp[i][j] = B[j][i]; 
		}
	}
	
	// Do the original calculation, now with the transpose:
	for (i = 0; i < n; ++i) {
		for (j = 0; j < n; ++j) {
		    /* Compute C(i,j) */
			for (int k = 0; k < n; ++k) {
				C[i][j] += A[i][k] * tmp[j][k];
			}
		}
	}
}
