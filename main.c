/*
 * Licensed under the MIT license.
 * Read LICENSE for more information.
 *
 * github: https://github.com/YoureWaifuABest/small-c-minesweeper
 */
#include <stdio.h>
#include <sodium.h>
#include <string.h>
#include <ncurses.h>
#include "main.h"

#define MINES 1

int select_square(struct point *);
void shift_left(char *);
int render_grid(char (*grid)[GRIDSIZE], struct point, WINDOW *);

int main(void)
{
	/*
	 * GRIDSIZExGRIDSIZE grid,
	 * 2d array
	 */
	char grid[GRIDSIZE][GRIDSIZE];
	int i, n, count, sp;
	struct point xy;
	WINDOW *game;
	WINDOW *help;

	/* 
	 * Initialize libsodium
	 * If libsodium did not correctly initialize,
	 * abort program.
	 */
	if (sodium_init() == -1)
		return -1;

	/*
	 * Creates exactly n bombs
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

	/* Start ncurses */
	initscr();

	/* Line buffering disabled */
	cbreak();

	/* Necessary for arrow keys */
	keypad(stdscr, TRUE);

	xy.x = 0;
	xy.y = 0;

	game = newwin(GRIDSIZE, (GRIDSIZE*2)+2, 1, 0);
	help = newwin(3, 31, GRIDSIZE+2, 0);

	if (has_colors()) {
		use_default_colors();
		start_color();
		init_pair(1, COLOR_RED, -1);
		init_pair(2, COLOR_GREEN, -1);
		init_pair(3, COLOR_BLUE, -1);
	}

	wmove(help,0,0);
	wattron (help, COLOR_PAIR(1));
	wprintw(help, "q ");
	wattroff(help, COLOR_PAIR(1));
	wprintw(help,"to quit,\n");
	wattron (help, COLOR_PAIR(3));
	wprintw(help, "arrow keys ");
	wattroff(help, COLOR_PAIR(3));
	wprintw(help, "to move to an area,\n");
	wattron (help, COLOR_PAIR(2));
	wprintw(help, "spacebar ");
	wattroff(help, COLOR_PAIR(2));
	wprintw(help, "to select the area");
	refresh();
	wrefresh(help);

	/* Print horizontal legend */
	move(0,0);
	for (i = 0; i != GRIDSIZE; ++i)
		printw("%i ", i+1);
	printw("\n");
	refresh();

	/* Main loop to run the game */
	while (1) {
		/* Output initial grid */
		render_grid(grid, xy, game);
		sp = select_square(&xy);

		if (sp == 1) {
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
						render_grid(grid, xy, game);
						move(GRIDSIZE+1,0);
						attrset(COLOR_PAIR(1));
						printw("You lose!\n");
						getch();
						endwin();
						return 0;
					/* 
					 * This results in debug mode outputting
					 * "Already Selected"
					 * Not a big deal, though, so I have no desire to fix it
					 */
					default:
						move(GRIDSIZE+1,0);
						attrset(COLOR_PAIR(1));
						printw("Already selected!\n");
						break;
				}
			}
		} else if (sp == -1) {
			endwin();
			return 0;
		}
		/* This can also be improved, like the lose condition */
		count = n = 0;
		for (i = 0; i != GRIDSIZE && grid[i][n] != 0; ++i) 
			for (n = 0; n != GRIDSIZE && grid[i][n] != 0; ++n) 
				++count;
		if (count == GRIDSIZE*GRIDSIZE) {
			render_grid(grid, xy, game);
			move(GRIDSIZE+1,0);
			attrset(COLOR_PAIR(2));
			printw("You won!\n");
			getch();
			endwin();
			return 0;
		}
	}
	return 0;
}

/* 
 * Code for rendering game on console.
 * There are a few different ways to do this, 
 * this requires the most if statements.
 */
int render_grid(char (*grid)[GRIDSIZE], struct point xy, WINDOW *game)
{
	int i, n;

	wmove(game,0,0);

	/*
	 * Iterate through each grid element,
	 * encoding certain ints to certain characters.
	 */
	for (i = 0; i != GRIDSIZE; i++) {
		for (n = 0; n != GRIDSIZE; n++) {
			/* Don't reveal unrevealed squares */
			if (grid[i][n] == 0 || grid[i][n] == 1)
				wprintw(game, "x ");
			else if (grid[i][n] == 2)
				wprintw(game, "b ");
			else if (grid[i][n] == 3)
				wprintw(game, "0 ");
			else if (grid[i][n] == 4)
				wprintw(game, "1 ");
			else if (grid[i][n] == 5)
				wprintw(game, "2 ");
			else if (grid[i][n] == 6)
				wprintw(game, "3 ");
			else if (grid[i][n] == 7)
				wprintw(game, "4 ");
			else if (grid[i][n] == 8)
				wprintw(game, "5 ");
			else if (grid[i][n] == 9)
				wprintw(game, "6 ");
		}
		/* Vertical Legend */
		wprintw(game, "%c", 'A' + i);
		wprintw(game, "\n");
	}

	wmove(game, xy.y, xy.x*2);

	wrefresh(game);

	return 0;
}

/* Selects a square and returns it's coordinates */
int select_square(struct point *xy)
{
	int ch;

	ch = getch();
	switch(ch) {
		case KEY_LEFT:
			if ((xy->x) > 0)
				(xy->x)--;
			break;
		case KEY_RIGHT:
			if ((xy->x) + 1 < GRIDSIZE)
				(xy->x)++;
			break;
		case KEY_UP:
			if ((xy->y) > 0)
				(xy->y)--;
			break;
		case KEY_DOWN:
			if ((xy->y) + 1 < GRIDSIZE)
				(xy->y)++;
			break;
		case ' ':
			return 1;
		case 'q':
			return -1;
		}
	return 0;
}
