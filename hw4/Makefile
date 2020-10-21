CC=g++ 
CFLAGS=-std=c++14
LINK=g++
OPT = -O3
DEBUG = -g
LIBS = -lcblas
LIB_OMP=-fopenmp

SOURCECPP = main.cpp matrix.cpp
OBJECTS = $(SOURCECPP:.cpp=.o)
DEBOBJECTS = $(SOURCECPP:.cpp=d.o)
BLASOBJECTS = $(SOURCECPP:.cpp=b.o)
OMPOBJECTS = $(SOURCECPP:.cpp=omp.o)	

%.o: %.cpp
	$(CC) $(CFLAGS) $(OPT) -c $< -o $@

%d.o: %.cpp
	$(CC) $(DEBUG) -c $< -o $@

%b.o: %.cpp
	$(CC) $(OPT) -DUSE_CBLAS -c $< -o $@	

%omp.o: %.cpp
	$(CC) $(OPT) -DUSE_OMP $(LIB_OMP) -c $< -o $@	


all: matrix matrix_debug matrix_blas matrix_omp

run: matrix
	./matrix

matrix: $(OBJECTS)
	$(LINK) $(OBJECTS) $(OPT) -o matrix

matrix_debug: $(DEBOBJECTS)
	$(LINK) -g $(DEBOBJECTS) $(DEBUG) -o matrix_debug

matrix_blas: $(BLASOBJECTS)
	$(LINK) $(BLASOBJECTS) $(OPT) $(LIBS) -o matrix_blas

matrix_omp: $(OMPOBJECTS)
	$(LINK) $(OMPOBJECTS) $(OPT) $(LIB_OMP) -o matrix_omp

clean:
	rm -rf *.o matrix matrix_debug matrix_blas matrix_omp
