CC = gcc
CPPFLAGS = -Iinclude
CFLAGS = -Wall -Wextra -pedantic -g
LDFLAGS = -pthread

SRC = $(wildcard src/*.c)
OBJ = $(SRC:.c=.o)

.PHONY: all clean test

all: mfind

mfind: $(OBJ)
	$(CC) $(CFLAGS) -o mfind $(OBJ) $(LDFLAGS)

clean:
	rm -f $(OBJ) mfind
	
test: all
	bash ./tests/run_tests.sh