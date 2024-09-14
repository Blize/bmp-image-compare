# Compiler
CC = gcc

# Compiler flags
CFLAGS = -Wall -Wextra -pedantic -std=c11

# Source files
SRCS = src/main.c src/util/util.c src/compare/compare.c

# Object files
OBJS = main.o util.o compare.o

# Executable name
TARGET = bmp_compare

# Default target
all: $(TARGET)

# Rule to build the executable
$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS)

# Rule to compile main.o
main.o: src/main.c src/util/util.h src/compare/compare.h
	$(CC) $(CFLAGS) -c src/main.c

# Rule to compile util.o
util.o: src/util/util.c src/util/util.h
	$(CC) $(CFLAGS) -c src/util/util.c

# Rule to compile compare.o
compare.o: src/compare/compare.c src/compare/compare.h
	$(CC) $(CFLAGS) -c src/compare/compare.c

# Clean the build
clean:
	rm -f $(OBJS) $(TARGET)
	rm -f image1.txt
	rm -f image2.txt
