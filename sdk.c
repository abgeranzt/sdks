#include <stdlib.h>

#include "sdks.h"

/* sdks.c
 * This file contains procedures that operate on the sudoku structure.
 */

/* Initialize sudoku structure and return a pointer to it. */
struct sdk *sdk_init(void)
{
	struct sdk *s = (struct sdk*) malloc(sizeof(struct sdk));
	for (int i = 0; i < SDK_W; i++)
		s->rows[i] = &s->cells[i * SDK_W];
	return s;
}

/* Iterate through the sudoku and attempt to fill every cell.
 * Return the of unchanged cells.
 */
int sdk_fill(struct sdk *s)
{
	int n, i, j;
	n = 0;
	for (i = 0; i < SDK_W; i++) {
	for (j = 0; j < SDK_W; j++) {
		if (s->rows[i][j])
			continue;
		if (!cell_fill(s, i, j)) {
			/* TODO FIXME
			 * there is a false positive upon the checks which causes
			 * invalid numbers to be filled in.
			 */
			n++;
		}
	}
	}
	return n;
}
