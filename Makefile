CPP_SOURCES = $(wildcard src/*.cpp)
CPP_HEADERS = $(wildcard include/*.h)

NAME = fields

all: $(NAME)

fields: $(CPP_SOURCES) $(CPP_HEADERS)
	g++ -o $(NAME) $(CPP_SOURCES)

clean:
	rm $(NAME)
