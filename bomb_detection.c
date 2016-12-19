#include "main.h"

int nearby_bombs(char (*grid)[GRIDSIZE], struct point xy);

/*
 * Basic floodfill function
 * 
 * The formula is:
 * If the current position on the grid's value is not equal to 0, return
 * If there are nearby bombs, set the value of the current position to the amount
 * of bombs + 3
 *
 * Set the current position on the grid's value to 3 (ie, 0 bombs)
 *
 * If there are no nearby bombs:
 * Call flood (down)
 * Call flood (up)
 * Call flood (left)
 * Call flood (right)
 */
int flood(char (*grid)[GRIDSIZE], struct point xy)
{
	/* If we've stepped onto a bomb or an already-checked area */
	if (grid[xy.y][xy.x] != 0)
		return 0;

	/* If there are bombs nearby */
	if (nearby_bombs(grid, xy) != 0) {
		grid[xy.y][xy.x] = nearby_bombs(grid, xy) + 3;
		return 0;
	}

	/* 3 is the arbitrary value for "already checked blank spot with no nearby bombs" */
	grid[xy.y][xy.x] = 3;

	if (nearby_bombs(grid, xy) == 0) {
		/* Down */
		if (xy.y < GRIDSIZE-1) {
			xy.y++;
			flood(grid, xy);
			xy.y--;
		}
		/* Up */
		if (xy.y > 0) {
			xy.y--;
			flood(grid, xy);
			xy.y++;
		}
		/* Left */
		if (xy.x > 0) {
			xy.x--;
			flood(grid, xy);
			xy.x++;
		}
		/* Right */
		if (xy.x < GRIDSIZE-1) {
			xy.x++;
			flood(grid, xy);
			xy.x--;
		}
	}
	return 0;
}

/* Not sure if there's a better way to do this */
int nearby_bombs(char (*grid)[GRIDSIZE], struct point xy)
{
	int nearby;

	nearby = 0;
	/* Right */
	if ((xy.x + 1) < GRIDSIZE && grid[xy.y][xy.x + 1] == 1)
		++nearby;
	/* Down */
	if ((xy.y + 1) < GRIDSIZE && grid[xy.y + 1][xy.x] == 1)
		++nearby;
	/* Downright */
	if ((xy.x + 1) < GRIDSIZE && (xy.y + 1) < GRIDSIZE  && grid[xy.y + 1][xy.x + 1] == 1)
		++nearby;
	/* Left */
	if ((xy.x - 1) >= 0 && grid[xy.y][xy.x - 1] == 1)
		++nearby;
	/* Up */
	if ((xy.y - 1) >= 0 && grid[xy.y - 1][xy.x] == 1)
		++nearby;
	/* Left-up */
	if ((xy.x - 1) >= 0 && (xy.y - 1) >= 0 && grid[xy.y - 1][xy.x - 1] == 1)
		++nearby;
	/* Right-up */
	if ((xy.x + 1) < GRIDSIZE && (xy.y - 1) >= 0 && grid[xy.y - 1][xy.x + 1] == 1)
		++nearby;
	/* Left-down */
	if ((xy.x - 1) >= 0 && (xy.y + 1) < GRIDSIZE && grid[xy.y + 1][xy.x - 1] == 1)
		++nearby;

	return nearby;
}
