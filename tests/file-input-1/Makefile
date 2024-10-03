CPP_SOURCES = FileInput.cpp $(wildcard ../../src/*)
CPP_HEADERS = $(wildcard ../../include/*)

all: fileinput-1

fileinput-1: $(CPP_SOURCES) $(CPP_HEADERS)
	g++ -o fileinput-1 $(CPP_SOURCES)

clean:
	rm fileinput-1