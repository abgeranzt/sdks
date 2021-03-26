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
 * Return the number of changed cells and perform a recursive call
 * to self if there are free cells left.
 */
int sdk_fill(struct sdk *s)
{
	int nc, n, i, j;
	sdk_index_basic(s);
	nc = 0;
	for (i = 0; i < SDK_W; i++) {
	for (j = 0; j < SDK_W; j++) {
		if ((n = cell_fill(s, i, j))) {
			nc++;
			s->nfree--;
			sdk_index_update(s, n, i, j);
		}
	}
	}
	/* TODO Advanced indexing */
	//if (!nc) ...
	return s->nfree ? n + sdk_fill(s) : n;
}

/* Iterate through the sudoku and index all available numbers for each
 * cell using simple checks. This should be sufficient most of the time and
 * sdk_index_adv() handles cases in which it is not.
 */
void sdk_index_basic(struct sdk *s)
{
	int i, j;
	for (i = 0; i < SDK_W; i++) {
	for (j = 0; j < SDK_W; j++) {
		/* Cell already filled. */
		if (s->rows[i][j].num)
			continue;
		cell_index_basic(s, i, j);
	}
	}
}

/* Iterate through rows/columns/groups of the sudoku and index available
 * numbers using more sophisticated methods than sdk_index_basic().
 */
void sdk_index_adv(struct sdk *s)
{
	/* TODO continue here. */
}

/* Update number index by removing n from the available numbers of
 * adjacent rows, columns and groups.
 */
void sdk_index_update(struct sdk *s, int n, int row, int col)
{
	int i, j, gr, gc;
	/* Update row. */
	for (i = 0; i < SDK_W; i++) {
		/* Cell already filled. */
		if (s->rows[row][i].num)
			continue;
		s->rows[row][i].num &= ~(1 << --n);
	}
	/* Update column. */
	for (i = 0; i < SDK_W; i++) {
		/* Cell already filled. */
		if (s->rows[i][col].num)
			continue;
		s->rows[i][col].num &= ~(1 << --n);
	}
	/* Specify group boundaries. */
	gr = row - row % 3;
	gc = col - col % 3;
	/* Update group. */
	for (i = gr; i < gr + 3; i++) {
	for (j = gc; j < gc + 3; j++) {
		/* Cell already filled. */
		if (s->rows[i][j].num)
			continue;
		s->rows[i][j].num &= ~(1 << --n);
	}
	}
}
