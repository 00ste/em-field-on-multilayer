# Compiler
CC = gcc

# Compiler flags
CFLAGS = -Wall -g

# Source files
SRCS = Field.cpp tests/OneMaterial.cpp

# Object files
OBJS = $(SRCS:.c=.o)

# Executable name
EXEC = run_experiment

# Default target
all: $(EXEC)

# Link the object files to create the executable
$(EXEC): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^

# Compile source files into object files
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Clean up build files
clean:
	rm -f $(OBJS) $(EXEC)

# Phony targets
.PHONY: all clean
