#include <stdlib.h>

#include "sdks.h"

struct sdk *sdk_init(void)
{
	struct sdk *s = (struct sdk*) malloc(sizeof(struct sdk));
	for (int i = 0; i < SDK_W; i++)
		s->rows[i] = &s->cells[i * SDK_W];
	sdk_parsein(s);
	return s;
}

/* Check row for available numbers and return them in avail */
int sdk_checkrow(struct sdk *s, int avail, int row, int col)
{
	int n, i;
	for (n = 0; n < 9; n++) {
		/* Number already marked as available. */
		if (avail & 1 << n)
			continue;
		for (i = 0; i < 9; i++) {
			if (i == col)
				continue;
			/* Number exists. */
			if (s->rows[row][i].number == n + 1) {
				goto nloop;
			}
		}
		avail |= 1 << n;
		nloop:
		continue;
	}
	return avail;
}

/* Check column for available numbers and return them in avail.
 * Logically equivalent to checkrow().
 */
int sdk_checkcol(struct sdk *s, int avail, int row, int col)
{
	int n, i;
	for (n = 0; n < 9; n++) {
		if (avail & 1 << n)
			continue;
		for (i = 0; i < 9; i++) {
			if (i == row)
				continue;
			if (s->rows[i][col].number == n + 1)
				goto nloop;
		}
		avail |= 1 << n;
		nloop:
		continue;
	}
	return avail;
}

/* Check group of 3x3 cells for available numbers and return them in avail. */
int sdk_checkgrp(struct sdk *s, int avail, int row, int col)
{
	int grow, gcol;
	/* Specify group boundaries. */
	for (grow = row; grow % 3; grow--);
	for (gcol = col; gcol % 3; gcol--);
	int n, i, j;
	for (n = 0; n < 9; n++) {
		if (avail & 1 << n)
			continue;
		for (i = grow; i < grow + 3; i++) {
			for (j = gcol; j < gcol + 3; j++) {
				if (i == row && j == col)
					continue;
				if (s->rows[i][j].number == n + 1)
					goto nloop;
			}
		}
		avail |= 1 << n;
		nloop:
		continue;
	}
	return avail;
}

