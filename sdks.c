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
