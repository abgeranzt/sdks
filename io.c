#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>

#include "sdks.h"

/* io.c
 * This file contains procedures to read from stdin and write to stdout.
 */

/* Parse stdin and populate sudoku structure.
 */
void sdk_parsein(struct sdk *s)
{
	int n, i;
	for (i = 0; i < SDK_SQ; i++) {
		n = getchar() - '0';
		s->cells[i].num = n;
		if (!n)
			s->nfree++;
		/* Next char is a delimiter. */
		getchar();
	}
}


/* Print sudoku cells in machine readable form.
 */
void sdk_print(struct sdk *s)
{
	char c;
	for (int i = 0; i < SDK_SQ; i++) {
		c = ((i + 1) % 9) ? ',' : '\n';
		printf("%d%c", s->cells[i].num, c);
	}
}
