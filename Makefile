CC = gcc
CFLAGS = -Wall -Wextra -pedantic -g -Iinclude

SRC = $(wildcard src/*.c)
OBJ = $(SRC:.c=.o)

.PHONY: all clean test

all: mfind

mfind: $(OBJ)
	$(CC) $(CFLAGS) -o mfind $(OBJ)

clean:
	rm -f $(OBJ) mfind
	
test: all
	./tests/run_tests.sh