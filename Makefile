# Compiler
CC = gcc

# Compiler flags
CFLAGS = -Wall -Wextra -pedantic -std=c11

# Source files
SRCS = main.c util.c

# Object files
OBJS = main.o util.o

# Executable name
TARGET = bmp_compare

# Default target
all: $(TARGET)

# Rule to build the executable
$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS)

# Rule to compile main.o
main.o: main.c util.h
	$(CC) $(CFLAGS) -c main.c

# Rule to compile util.o
util.o: util.c util.h
	$(CC) $(CFLAGS) -c util.c

# Clean the build
clean:
	rm -f $(OBJS) $(TARGET)
	rm -f image1.txt
	rm -f image2.txt
