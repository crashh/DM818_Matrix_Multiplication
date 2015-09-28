# on Franklin and Hopper, we will benchmark you against Cray LibSci, the default vendor-tuned BLAS. The Cray compiler wrappers handle all the linking. If you wish to compare with other BLAS implementations, check the NERSC documentation.
# This makefile is intended for the GNU C compiler. On Franklin and Hopper, the Portland compilers are default, so you must instruct the Cray compiler wrappers to switch to GNU: type "module swap PrgEnv-pgi PrgEnv-gnu"
# Your code must compile (with GCC) with the given CFLAGS. You may experiment with the OPT variable to invoke additional compiler options.

CC = cc 
OPT = -Og
CFLAGS = -g -Wall -std=gnu99 $(OPT)
LDFLAGS = -Wall -funroll-loops -funit-at-a-time -march=native -mfpmath=sse -fstrict-aliasing -ftree-vectorize
# librt is needed for clock_gettime
LDLIBS = -lrt

targets = benchmark-naive benchmark-blocked benchmark-blas benchmark-transpose benchmark-orig-blocking
objects = benchmark.o benchmark_testcase.o dgemm-naive.o dgemm-blocked.o dgemm-blas.o dgemm-transposed.o orig-dgemm-blocking.o

.PHONY : default
default : all

.PHONY : all
all : clean $(targets)

benchmark-naive : benchmark.o dgemm-naive.o 
	$(CC) -o $@ $^ $(LDLIBS)
benchmark-blocked : benchmark.o dgemm-blocked.o
	$(CC) -o $@ $^ $(LDLIBS)
benchmark-blas : benchmark.o dgemm-blas.o
	$(CC) -o $@ $^ $(LDLIBS)
benchmark-transpose : benchmark.o dgemm-transpose.o
	$(CC) -o $@ $^ $(LDLIBS)
benchmark-orig-blocking : benchmark_testcase.o orig-dgemm-blocking.o
	$(CC) -o $@ $^ $(LDLIBS)

%.o : %.c
	$(CC) -c $(CFLAGS) $<

.PHONY : clean
clean:
	rm -f $(targets) $(objects)
