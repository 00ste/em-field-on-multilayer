CPP_SOURCES = points-file.cpp $(wildcard ../../src/*.cpp)
CPP_HEADERS = $(wildcard ../../include/*.h)

all: points-file

points-file: $(CPP_SOURCES) $(CPP_HEADERS)
	g++ -o points-file $(CPP_SOURCES)

clean:
	rm points-file
