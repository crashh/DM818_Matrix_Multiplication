//NOTE: #include <acml.h> //assumes AMD platform
extern "C" {
#include <cblas.h> //assumes general CBLAS interface
}

const char* dgemm_desc = "BLAS dgemm.";
void square_dgemm( int n, double *A, double *B, double *C )
{
    //NOTE: dgemm( 'N','N', n,n,n, 1, A,n, B,n, 1, C,n );
    cblas_dgemm(CblasColMajor, CblasNoTrans, CblasNoTrans, n,n,n, 1, A,n, B,n, 1, C,n );
}
