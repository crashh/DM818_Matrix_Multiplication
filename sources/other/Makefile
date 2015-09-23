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

CFLAGS = -O3

all:	benchmark-naive benchmark-blocked benchmark-blas

benchmark-naive: benchmark.o dgemm-naive.o
	$(CC) -o $@ benchmark.o dgemm-naive.o $(LIBS)
benchmark-blocked: benchmark.o dgemm-blocked.o
	$(CC) -o $@ benchmark.o dgemm-blocked.o $(LIBS)
benchmark-blas: benchmark.o dgemm-blas.o
	$(CC) -o $@ benchmark.o dgemm-blas.o $(LIBS)

%.o: %.cpp
	$(CC) -c $(CFLAGS) $<

%.o: %.c
	$(CC) -c $(CFLAGS) $<

clean:
	rm -f benchmark-naive benchmark-blocked benchmark-blas *.o
