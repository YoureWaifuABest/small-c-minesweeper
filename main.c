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

/* Not sure if global variable for debug mode is the right call */
int debug = 0;

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
	int i, n;
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
	 * Alg2: Creates exactly n bombs (in this case 10)
	 * Distributes by choosing grid location
	 *
	 * This actually results in 11 bombs. Not sure why.
	 */
	for (i = 0; i != 10; i++) {
		xy.y = randombytes_uniform(GRIDSIZE);
		xy.x = randombytes_uniform(GRIDSIZE);
		/* If a bomb is placed on a square that already contains a bomb, don't change i */
		if (grid[xy.y][xy.x] == 1)
			i--;
		else
			grid[xy.y][xy.x] = 1;
	}

	for (i = 0; i != GRIDSIZE; i++)
		for (n = 0; n != GRIDSIZE; n++)
			if (grid[i][n] != 1)
				grid[i][n] = 0;

	/* 
	 * Alg1: 50% for each square
	 * This is not used for obvious reasons.
	 *
	for (i = 0; i != GRIDSIZE; i++)
		for (n = 0; n != GRIDSIZE; n++) {
			grid[i][n] = randombytes_uniform(2);
		}
	 */

	/*
	 * Infinite loop to run the game
	 * Currently only a failure state, no success
	 */
	while (1) {
		/* Output initial grid */
		render_grid(grid);
		xy = select_square();

		/* 
		 * Debug mode is synonymous with cheater mode
		 * Don't use it if you want to have "fun"
		 */
		if (debug) {
			/* Print horizontal legend */
			for (i = 0; i != GRIDSIZE; ++i)
				printf("%i ", i+1);
			printf("\n");

			/* Print cheater grid */
			for (i = 0; i != GRIDSIZE; ++i) {
				for (n = 0; n != GRIDSIZE; ++n)
					printf("%d ", grid[i][n]);
				/* 
				 * Print vertical legend 
				 * There is probably a better way of 
				 * incrementing these characters
				 */
				printf("%c", 'A' + i);
				printf("\n");
			}
			printf("\n");
			/*
			 * Print selected position on grid.
			 * This will default to (-1, -1) when debug mode is first initialized
			 * Don't worry about it.
			 */
			printf("y: %d x: %d\n", xy.y, xy.x);
			/* Value of this position on grid */
			printf("Grid Value: %d\n", grid[xy.y][xy.x]);
		}

		if (xy.x != -1 && xy.y != -1) {
			switch (grid[xy.y][xy.x]) {
				case 0:
					flood(grid, xy);
					break;
				case 1:
					grid[xy.y][xy.x] = 2;
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
	 * db = debug
	 * returns -1 for xy to signify that the values are unusual,
	 * and I have to return a value of type struct point.
	 */
	if (string[0] == 'd' && string[1] == 'b') {
		printf("Debug Mode Activated!\n");
		debug = 1;
		xy.y = -1;
		xy.x = -1;
		return xy;
	}

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
