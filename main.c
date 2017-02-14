/*
 * Licensed under the MIT license.
 * Read LICENSE for more information.
 *
 * github: https://github.com/YoureWaifuABest/small-c-minesweeper
 */
#include <stdio.h>
#include <sodium.h>
#include <string.h>
#include <stdlib.h>
#include "main.h"

#define MINES 10

struct point select_square(void);
void shift_left(char *);
int render_grid(char (*grid)[GRIDSIZE]);

int main(void)
{
	/*
	 * GRIDSIZExGRIDSIZE grid,
	 * 2d array
	 */
	char grid[GRIDSIZE][GRIDSIZE];
	int i, n, count;
	struct point xy;

	/* 
	 * Initialize libsodium
	 * If libsodium did not correctly initialize,
	 * abort program.
	 */
	if (sodium_init() == -1) {
		exit(EXIT_FAILURE);
	}

	/*
	 * Alg2: Creates exactly n bombs
	 * Distributes by choosing grid location
	 */
	for (i = 0; i != MINES; i++) {
		xy.y = randombytes_uniform(GRIDSIZE);
		xy.x = randombytes_uniform(GRIDSIZE);
		/* If a bomb is placed on a square that already contains a bomb, don't change i */
		if (grid[xy.y][xy.x] != 0)
			i--;
		else
			grid[xy.y][xy.x] = 1;
	}

	/* Initialize all non-bombs as 0 */
	for (i = 0; i != GRIDSIZE; i++)
		for (n = 0; n != GRIDSIZE; n++)
			if (grid[i][n] != 1)
				grid[i][n] = 0;

	/* Main loop to run the game */
	while (1) {
		/* Output initial grid */
		render_grid(grid);
		xy = select_square();

		if (xy.x != -1 && xy.y != -1) {
			switch (grid[xy.y][xy.x]) {
				case 0:
					flood(grid, xy);
					break;
				case 1:
					grid[xy.y][xy.x] = 2;
					/* It's better to have it just render bombs as b rather than this */
					for (i = 0; i != GRIDSIZE; ++i)
						for (n = 0; n != GRIDSIZE; ++n)
							if (grid[i][n] == 1)
								grid[i][n] = 2;
					render_grid(grid);
					printf("You lose!\n");
					exit(EXIT_SUCCESS);
				/* 
				 * This results in debug mode outputting
				 * "Already Selected"
				 * Not a big deal, though, so I have no desire to fix it
				 */
				default:
					printf("Already selected!\n");
					break;
			}
		}
		/* This can also be improved, like the lose condition */
		count = n = 0;
		for (i = 0; i != GRIDSIZE && grid[i][n] != 0; ++i) 
			for (n = 0; n != GRIDSIZE && grid[i][n] != 0; ++n) 
				++count;
		if (count == GRIDSIZE*GRIDSIZE) {
			render_grid(grid);
			printf("You won!\n");
			exit(EXIT_SUCCESS);
		}
	}
	return 0;
}

/* 
 * Code for rendering game on console.
 * There are a few different ways to do this, 
 * this requires the most if statements.
 */
int render_grid(char (*grid)[GRIDSIZE])
{
	int i, n;

	/* Print horizontal legend */
	for (i = 0; i != GRIDSIZE; ++i)
		printf("%i ", i+1);
	printf("\n");

	/*
	 * Iterate through each grid element,
	 * encoding certain ints to certain characters.
	 */
	for (i = 0; i != GRIDSIZE; i++) {
		for (n = 0; n != GRIDSIZE; n++) {
			/* Don't reveal unrevealed squares */
			if (grid[i][n] == 0 || grid[i][n] == 1)
				printf("x ");
			else if (grid[i][n] == 2)
				printf("b ");
			else if (grid[i][n] == 3)
				printf("0 ");
			else if (grid[i][n] == 4)
				printf("1 ");
			else if (grid[i][n] == 5)
				printf("2 ");
			else if (grid[i][n] == 6)
				printf("3 ");
			else if (grid[i][n] == 7)
				printf("4 ");
			else if (grid[i][n] == 8)
				printf("5 ");
			else if (grid[i][n] == 9)
				printf("6 ");
		}
		/* Vertical Legend */
		printf("%c", 'A' + i);
		printf("\n");
	}

	return 0;
}

/* Selects a square and returns it's coordinates */
struct point select_square(void)
{
	int i;
	char c;
	/* 
	 * This length is entirely arbitrary.
	 * Doesn't handle buffer-overflows or anything of the sort.
	 * Perhaps it's not the smartest thing to expect sane user input,
	 * but the alternative is a small amount of work on my part.
	 */
	char string[10];
	struct point xy;

	i = 0;
	while ((c = getchar()) != EOF && c != '\n')
		string[i++] = c;

	string[i] = '\0';

	/* 
	 * 17 is the difference between A and 0
	 * The downside to this is that the letters need to be in caps.
	 * Haven't bothered making it case-insensitive
	 */
	string[0] = string[0] - 17;
	/* Converts from the character value of 0 to the integer value of 0 */
	xy.y = string[0] - '0';

	/* Removes leading 0 */
	shift_left(string);

	/* Convert string to integer */
	xy.x = atoi(string);
	/* 
	 * Values on the grid are 0-indexed, whereas user input is 1-indexed.
	 * This converts the input to be 1-indexed, so 10=9, etc.
	 */
	xy.x -= 1;

	return xy;
}
