#include <stdio.h>
#include <sodium.h>
#include <string.h>
#include <stdlib.h>
#include "main.h"

int debug = 0;

struct point select_square(void);
void shift_left(char *);
int render_grid(char (*grid)[GRIDSIZE]);

int main(void)
{
	/*
	 5x5 grid,
	 2d array
	 */
	char grid[GRIDSIZE][GRIDSIZE];
	int i, n;
	struct point xy;

	if (sodium_init() == -1) {
		return -1;
	}

	/*
	 * Alg2: Exactly n bombs (in this case 10)
	 * Distributes by choosing grid location
	 */
	for (i = 0; i != 10; i++) 
		grid[randombytes_uniform(GRIDSIZE)][randombytes_uniform(GRIDSIZE)] = 1;

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

	while (1) {
		render_grid(grid);
		xy = select_square();

		if (debug) {
			for (i = 0; i != GRIDSIZE; ++i)
				printf("%i ", i+1);
			printf("\n");
			for (i = 0; i != GRIDSIZE; ++i) {
				for (n = 0; n != GRIDSIZE; ++n)
					printf("%d ", grid[i][n]);
				printf("%c", 'A' + i);
				printf("\n");
			}
			printf("\n");
			printf("y: %d x: %d\n", xy.y, xy.x);
			printf("Grid Value: %d\n", grid[xy.y][xy.x]);
		}

		if (xy.x != -1 && xy.y != -1) {
			switch (grid[xy.y][xy.x]) {
				case 0:
					flood(grid, xy);
					break;
				case 1:
					grid[xy.y][xy.x] = 2;
					render_grid(grid);
					printf("You lose!\n");
					exit(EXIT_SUCCESS);
				default:
					printf("Already selected!\n");
					break;
			}
		}
	}
	return 0;
}

int render_grid(char (*grid)[GRIDSIZE])
{
	int i, n;

	for (i = 0; i != GRIDSIZE; ++i)
		printf("%i ", i+1);
	printf("\n");
	for (i = 0; i != GRIDSIZE; i++) {
		for (n = 0; n != GRIDSIZE; n++) {
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
		printf("%c", 'A' + i);
		printf("\n");
	}

	return 0;
}

struct point select_square(void)
{
	int i;
	char c;
	char string[10];
	struct point xy;

	i = 0;
	while ((c = getchar()) != EOF && c != '\n')
		string[i++] = c;

	string[i] = '\0';

	if (string[0] == 'd' && string[1] == 'b') {
		printf("Debug Mode Activated!\n");
		debug = 1;
		xy.y = -1;
		xy.x = -1;
		return xy;
	}

	string[0] = string[0] - 17;
	xy.y = string[0] - '0';

	shift_left(string);

	xy.x = atoi(string);
	xy.x -= 1;

	return xy;
}
