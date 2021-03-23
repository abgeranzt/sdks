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
	for (int i = 0; i < SDK_SQ; i++) {
		s->cells[i] = getchar() - '0';
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
		printf("%d%c", s->cells[i], c);
	}
}
