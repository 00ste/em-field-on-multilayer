CPP_SOURCES = field-init.cpp $(wildcard ../../src/*.cpp)
CPP_HEADERS = $(wildcard ../../include/*.h)

all: field-init

field-init: $(CPP_SOURCES) $(CPP_HEADERS)
	g++ -o field-init $(CPP_SOURCES)

clean:
	rm field-init
