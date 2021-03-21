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

	for (i = 0; i < SDK_W; i++) {
		}
	}
}
