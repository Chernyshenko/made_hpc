CC=g++ 
CFLAGS=-std=c++14
LINK=g++
OPT = -O3
DEBUG = -g
LIBS = -lcblas
LIB_OMP=-fopenmp

SOURCECPP = main.cpp ../matrix.cpp
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


all: pagerank pagerank_debug pagerank_blas pagerank_omp

run: pagerank
	./pagerank

pagerank: $(OBJECTS)
	$(LINK) $(OBJECTS) $(OPT) -o pagerank

pagerank_debug: $(DEBOBJECTS)
	$(LINK) -g $(DEBOBJECTS) $(DEBUG) -o pagerank_debug

pagerank_blas: $(BLASOBJECTS)
	$(LINK) $(BLASOBJECTS) $(OPT) $(LIBS) -o pagerank_blas

pagerank_omp: $(OMPOBJECTS)
	$(LINK) $(OMPOBJECTS) $(OPT) $(LIB_OMP) -o pagerank_omp

clean:
	rm -rf *.o pagerank pagerank_debug pagerank_blas pagerank_omp
