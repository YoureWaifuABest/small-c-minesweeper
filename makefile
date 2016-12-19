CC=gcc
CFLAGS=-Wall -lsodium

all: 
	$(CC) -o minesweeper main.c $(CFLAGS)
