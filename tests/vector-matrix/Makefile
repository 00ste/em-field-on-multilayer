CPP_SOURCES = vector-matrix.cpp ../../src/Matrix2x2.cpp
CPP_HEADERS = ../../include/Matrix2x2.h

all: vector-matrix

vector-matrix: vector-matrix.cpp $(CPP_SOURCES) $(CPP_HEADERS)
	g++ -o vector-matrix $(CPP_SOURCES)

clean:
	rm vector-matrix