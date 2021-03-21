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

/* Check possible numbers for a cell through multiple tests. */
void cell_index(struct sdk *s, int row, int col)
{
	s->rows[row][col].avail =
		sdk_checkrow(s, row, col) &
		sdk_checkcol(s, row, col) &
		sdk_checkgrp(s, row, col);
}

/* Iterate through every cell and fill in numbers where possible. */
void sdk_fill(struct sdk *s)
{
	int n, i, j;
	for (i = 0; i < SDK_W; i++) {
	for (j = 0; j < SDK_W; j++) {
		cell_index(s, i, j);
		for (n = 0; n < SDK_W; n++) {
			if (s->rows[i][j].avail & 1 << n) {
				s->rows[i][j].number = n + 1;
				break;
			}
		}
	}
	}
}
