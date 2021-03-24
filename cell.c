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
int cell_check(struct sdk *s, int row, int col)
{
	return	cell_checkrow(s, row, col) &
		cell_checkcol(s, row, col) &
		cell_checkgrp(s, row, col);
}

/* Attempt to fill the cell and return TRUE if successfull. */
int cell_fill(struct sdk *s, int row, int col)
{
	int av, n;
	av = cell_check(s, row, col);
	for (n = 0; n < SDK_W; n++) {
		/* Match. */
		if ((av & 1 << n) == (av | 1 << n)) {
			s->rows[row][col] = n + 1;
			return TRUE;
		}
	}
	return FALSE;

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
			if (s->rows[row][i] == n + 1) {
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
			if (s->rows[i][col] == n + 1)
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
	int grow, gcol;
	/* Specify group boundaries. */
	for (grow = row; grow % 3; grow--);
	for (gcol = col; gcol % 3; gcol--);
	int av, n, i, j;
	av = 0;
	for (n = 0; n < SDK_W; n++) {
		for (i = grow; i < grow + 3; i++) {
		for (j = gcol; j < gcol + 3; j++) {
			/* Skip current cell. */
			if (i == row && j == col)
				continue;
			/* exists. */
			if (s->rows[i][j] == n + 1)
				goto nloop;
		}
		}
		av |= 1 << n;
		nloop:
		continue;
	}
	return av;
}
