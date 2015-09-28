#
# on Franklin:
# type "module load acml" to use ACML (required)
# type "module swap PrgEnv-pgi PrgEnv-gnu" to use GNU compiler
# type "module swap PrgEnv-pgi PrgEnv-pathscale" to use PathScale compiler
#
# On other machines you might need to use other library than ACML
# this could be MKL on an Intel platform or ESSL on an IBM platform.
#
#CC = CC
CC = g++
#LIBS = -lacml
#LIBS = $(shell pkg-config cblas --libs)

#multi-thread cblas for IMADA i5 3.2 GHz machine
#use the following line only with a hand compiled atlas
#LIBS = -L/home/daniel/atlas/lib -lptcblas -latlas -lpthread

#otherwise use
LIBS = -lcblas -latlas -lpthread

#single-thread cblas for IMADA i5 3.2 GHz machine
#LIBS = -L/home/daniel/atlas/lib -lcblas -latlas

#not optimized Ubuntu version
#LIBS= -lcblas



#not optimized Ubuntu version
#LIBS= -lcblas

#https://gcc.gnu.org/onlinedocs/gcc-3.4.4/gcc/Optimize-Options.html
# -funroll-loops    Gives quite a big performance improvement.
# -funit-at-a-time  No immediate noticable improvement. Needs more testing.
# -march=native     Small performance gain. 
#                   (http://stackoverflow.com/questions/16064288/gcc-optimization-flags-for-matrix-vector-operations)
# -mfpmath=sse      Small performance gain. 
#                   (http://stackoverflow.com/questions/16064288/gcc-optimization-flags-for-matrix-vector-operations)
CFLAGS = -O2 -funroll-loops -funit-at-a-time -march=native -mfpmath=sse -fstrict-aliasing -ftree-vectorize

all:	benchmark-naive benchmark-blocked benchmark-blas benchmark-transpose benchmark-orig-blocking

benchmark-naive: benchmark.o dgemm-naive.o
	$(CC) -o $@ benchmark.o dgemm-naive.o $(LIBS)
benchmark-blocked: benchmark.o dgemm-blocked.o
	$(CC) -o $@ benchmark.o dgemm-blocked.o $(LIBS)
benchmark-blas: benchmark.o dgemm-blas.o
	$(CC) -o $@ benchmark.o dgemm-blas.o $(LIBS)
benchmark-transpose: benchmark.o dgemm-transpose.o
	$(CC) -o $@ benchmark.o dgemm-transpose.o $(LIBS)
benchmark-orig-blocking: benchmark.o orig-dgemm-blocking.o
	$(CC) -o $@ benchmark.o orig-dgemm-blocking.o $(LIBS)

%.o: %.cpp
	$(CC) -c $(CFLAGS) $<

%.o: %.c
	$(CC) -c $(CFLAGS) $<

clean:
	rm -f benchmark-naive benchmark-blocked benchmark-blas benchmark-transpose benchmark-orig-blocking *.o
