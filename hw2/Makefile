CC=g++ -std=c++14
LINK=g++
OPT = -O3
DEBUG = -g
LIBS = #-lcblas

SOURCECPP = main.cpp matrix.cpp cblas.cpp
OBJECTS = $(SOURCECPP:.cpp=.o)
DEBOBJECTS = $(SOURCECPP:.cpp=d.o)

%.o: %.cpp
	$(CC) $(CFLAGS) $(OPT) -c $< -o $@

%d.o: %.cpp
	$(CC) $(DEBUG) -c $< -o $@


all: matrix matrix_debug

run: matrix
	./matrix

matrix: $(OBJECTS)
	$(LINK) $(OBJECTS) $(OPT) $(LIBS) -o matrix

matrix_debug: $(DEBOBJECTS)
	$(LINK) -g $(DEBOBJECTS) $(DEBUG) $(LIBS) -o matrix_debug

clean:
	rm -rf *.o matrix matrix_debug
