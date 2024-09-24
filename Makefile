# Makefile
CC = gcc
CFLAGS = -Wall -Wextra -g
LIBS = -lreadline   # Make sure readline is included here
SRC = src/main.c src/shell.c
OBJ = main

all: $(OBJ)

$(OBJ): $(SRC)
	$(CC) $(CFLAGS) -o $(OBJ) $(SRC) $(LIBS)

clean:
	rm -f $(OBJ)
