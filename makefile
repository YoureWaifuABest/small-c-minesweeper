CC=gcc
CFLAGS=-Wall -lsodium -lncurses

all: 
	$(CC) -o minesweeper main.c $(CFLAGS)
