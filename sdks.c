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

/* Iterate through every cell and check available numbers */
void sdk_index(struct sdk *s)
{
	int av, i, j;
	for (i = 0; i < SDK_W; i++) {
		for (j = 0; j < SDK_W; j++) {
			av = sdk_checkrow(s, i, j);
			av &= sdk_checkcol(s, i, j);
			av &= sdk_checkgrp(s, i, j);
			s->rows[i][j].avail = av;
		}
	}
}

/* Check row for available numbers and return them in av */
int sdk_checkrow(struct sdk *s, int row, int col)
{
	int av, n, i;
	av = 0;
	for (n = 0; n < 9; n++) {
		/* Number already marked as available. */
		for (i = 0; i < 9; i++) {
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
	for (n = 0; n < 9; n++) {
		for (i = 0; i < 9; i++) {
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
	for (n = 0; n < 9; n++) {
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

