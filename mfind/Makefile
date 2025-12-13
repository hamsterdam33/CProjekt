CC = gcc
CFLAGS = -Wall -Wextra -pedantic -g -Iinclude

SRC = $(wildcard src/*.c)
OBJ = $(SRC:.c=.o)

mfind: $(OBJ)
	$(CC) $(CFLAGS) -o mfind $(OBJ)

clean:
	rm -f $(OBJ) mfind
