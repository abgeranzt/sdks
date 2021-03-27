#include <stdio.h>
#include <stdlib.h>
#include "sdks.h"

int main()
{
	struct sdk *s = sdk_init();
	sdk_parsein(s);
	sdk_index_basic(s);
	printf("%d\n", s->nfree);
	int n = sdk_fill(s);
	printf("%d\n", s->nfree);
	sdk_print(s);
	//printf("\n%d", n);
	/*
	for (int i = 0; i < 2; i++) {
		sdk_fill(s);
		printf("\n");
		sdk_print(s);
	}
	*/
	return 0;
}
