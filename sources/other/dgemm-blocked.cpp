/*
  In case you're wondering, dgemm stands for Double-precision, GEneral
  Matrix-Matrix multiplication.
*/

const char* dgemm_desc = "Simple blocked dgemm.";

#if !defined(BLOCK_SIZE)
#define BLOCK_SIZE 41
#endif

#define min(a,b) (((a)<(b))?(a):(b))

/*
  A is M-by-K
  B is K-by-N
  C is M-by-N

  lda is the leading dimension of the matrix (the M of square_dgemm).
*/

void basic_dgemm( int lda, int M, int N, int K,
                  double *A, double *B, double *C )
{
  /*
    To optimize this, think about loop unrolling and software
    pipelining.  Hint:  For the majority of the matmuls, you
    know exactly how many iterations there are (the block size)...
  */
  for( int i = 0; i < M; i++ )
       for( int j = 0; j < N; j++ ) 
       {
            double cij = C[i+j*lda];
            for( int k = 0; k < K; k++ )
                 cij += A[i+k*lda] * B[k+j*lda];
            C[i+j*lda] = cij;
       }
}

void do_block( int lda, double *A, double *B, double *C,
               int i, int j, int k )
{
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
     int M = min( BLOCK_SIZE, lda-i );
     int N = min( BLOCK_SIZE, lda-j );
     int K = min( BLOCK_SIZE, lda-k );

     basic_dgemm( lda, M, N, K, A + i + k*lda, B + k + j*lda, C + i + j*lda);
}

void square_dgemm( int M, double *A, double *B, double *C )
{
     for( int i = 0; i < M; i += BLOCK_SIZE )
          for( int j = 0; j < M; j += BLOCK_SIZE )
               for( int k = 0; k < M; k += BLOCK_SIZE )
                    do_block( M, A, B, C, i, j, k );
}
