#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>

#include "sdks.h"

/* Parse stdin and populate sudoku structure.
 */
void parsesdk(struct sdk *s)
{
	for (int i = 0; i < SDK_W; i++) {
		for (int j = 0; j < SDK_W; j++) {
			s->rows[i][j].number = getchar() - '0';
			/* Next char is a delimiter. */
			getchar();
		}
	}
}
