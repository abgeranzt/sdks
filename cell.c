#include "sdks.h"

/* cell.c
 * This file contains procedures that check available numbers on a sudoku cell.
 * The methods used for this currently include:
 * 	- checking the cell's row
 * 	- checking the cell's column
 * 	- checking the cell's 3x3 group
 *
 * TODO add more if necessary
 */

/* Perform all checks on the cell and returns available numbers as bit flags. */
int cell_index_basic(struct sdk *s, int row, int col)
{
	/* TODO is splitting this function necessary?
	 * -> See sdk_index_adv.*/
	return	cell_checkrow(s, row, col) &
		cell_checkcol(s, row, col) &
		cell_checkgrp(s, row, col);
}

/* Attempt to fill the cell and return the filled-in number if successfull.
 * Return 0 otherwise.
 */
int cell_fill(struct sdk *s, int row, int col)
{
	int n, av;
	av = s->rows[row][col].av;
	for (n = 0; n < SDK_W; n++) {
		/* Unambigious match. */
		if ((av & 1 << n) == (av | 1 << n)) {
			s->rows[row][col].num = ++n;
			return n;
		}
	}
	return 0;

}

/* Check row for available numbers and return them as bit flags in av. */
int cell_checkrow(struct sdk *s, int row, int col)
{
	int av, n, i;
	av = 0;
	for (n = 0; n < SDK_W; n++) {
		for (i = 0; i < SDK_W; i++) {
			/* Skip current cell. */
			if (i == col)
				continue;
			/* exists. */
			if (s->rows[row][i].num == n + 1) {
				goto nloop;
			}
		}
		av |= 1 << n;
		nloop:
		continue;
	}
	return av;
}

/* Check column for available numbers and return them as bit flags in av.
 * Logically equivalent to checkrow().
 */
int cell_checkcol(struct sdk *s, int row, int col)
{
	int av, n, i;
	av = 0;
	for (n = 0; n < SDK_W; n++) {
		for (i = 0; i < SDK_W; i++) {
			/* Skip current cell. */
			if (i == row)
				continue;
			/* exists. */
			if (s->rows[i][col].num == n + 1)
				goto nloop;
		}
		av |= 1 << n;
		nloop:
		continue;
	}
	return av;
}

/* Check group of 3x3 cells for available numbers and return them as bit flags in av. */
int cell_checkgrp(struct sdk *s, int row, int col)
{
	int av, n, gr, gc, i, j;
	/* Specify group boundaries. */
	gr = row - row % 3;
	gc = col - col % 3;
	av = 0;
	for (n = 0; n < SDK_W; n++) {
		for (i = gr; i < gr + 3; i++) {
		for (j = gc; j < gc + 3; j++) {
			/* Skip current cell. */
			if (i == row && j == col)
				continue;
			/* exists. */
			if (s->rows[i][j].num == n + 1)
				goto nloop;
		}
		}
		av |= 1 << n;
		nloop:
		continue;
	}
	return av;
}
