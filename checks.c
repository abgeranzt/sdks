#include "sdks.h"

/* Check row for available numbers and return them in av */
int sdk_checkrow(struct sdk *s, int row, int col)
{
	int av, n, i;
	av = 0;
	for (n = 0; n < SDK_W; n++) {
		/* Number already marked as available. */
		for (i = 0; i < SDK_W; i++) {
			if (i == col)
				continue;
			/* Number exists. */
			if (s->rows[row][i].number == n + 1) {
				goto nloop;
			}
		}
		av |= 1 << n;
		nloop:
		continue;
	}
	return av;
}

/* Check column for available numbers and return them in av.
 * Logically equivalent to checkrow().
 */
int sdk_checkcol(struct sdk *s, int row, int col)
{
	int av, n, i;
	av = 0;
	for (n = 0; n < SDK_W; n++) {
		for (i = 0; i < SDK_W; i++) {
			if (i == row)
				continue;
			if (s->rows[i][col].number == n + 1)
				goto nloop;
		}
		av |= 1 << n;
		nloop:
		continue;
	}
	return av;
}

/* Check group of 3x3 cells for available numbers and return them in av. */
int sdk_checkgrp(struct sdk *s, int row, int col)
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
				if (i == row && j == col)
					continue;
				if (s->rows[i][j].number == n + 1)
					goto nloop;
			}
		}
		av |= 1 << n;
		nloop:
		continue;
	}
	return av;
}
