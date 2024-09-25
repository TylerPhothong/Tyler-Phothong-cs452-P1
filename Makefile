# Makefile
CC = gcc
CFLAGS = -Wall -Wextra -g
LIBS = -lreadline 
SRC = src/main.c src/shell.c
OBJ = main

all: $(OBJ)

$(OBJ): $(SRC)
	$(CC) $(CFLAGS) -o $(OBJ) $(SRC) $(LIBS)

clean:
	rm -f $(OBJ)
