#include <stdio.h>
#include <stdlib.h>

#include "sdks.h"

int main()
{
	struct sdk *s = sdk_init();
	sdk_parsein(s);
	int n;
	/*
	do {
		n = sdk_fill(s);
		printf("n: %d\n", n);
	} while (n != 0);
	*/

	sdk_print(s);
	for (int i = 0; i < 1; i++) {
		sdk_fill(s);
		sdk_print(s);
	}
	return 0;
}
