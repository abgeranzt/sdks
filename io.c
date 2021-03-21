#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>

#include "sdks.h"

/* Parse stdin and populate sudoku structure and return SUCCESS.
 * Return FAILURE when invalid input is encountered.
 */

int parsesdk(struct sdk *s)
{
	char c;
	int i, j, n;
	for (i = 0; i < SDK_W; i++) {
	for (j = 0; j < SDK_W; j++) {
		c = getchar();
		if (isdigit(c)) {
			n = c - '0';
			/* Invalid number. */
			if (n < 0 || n > 9) {
				fprintf(stderr, "Invalid number '%d\n'", n);
				return FAILURE;
			}
			s->rows[i][j] = n;
			/* Next char is a delimiter. */
			getchar();
			continue;
		}
		switch (c) {
		case ' ':
			getchar();
		case ',':
			s->rows[i][j] = 0;
			continue;
		default:
			fprintf(stderr, "Invalid character '%c'\n", c);
			return FAILURE;
		}
	}
	}
	return SUCCESS;
}
