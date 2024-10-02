CPP_SOURCES = field-init.cpp $(wildcard ../../src/*.cpp)

all: field-init

field-init: $(CPP_SOURCES) data.txt
	g++ -o field-init $(CPP_SOURCES)

clean:
	rm field-init
