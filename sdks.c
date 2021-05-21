/*
 * sdks - Sudoku Solver
 * Copyright (C) 2021 Marcel Engelke
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

/*
 * sdks.c
 * This file contains procedures to manipulate data in the sudoku strcture.
 */

#include <stdlib.h>

#include "log.h"
#include "sdks.h"

/* Initialize Sudoku structure and return pointer to it. */
struct Sudoku *sdks_init()
{
	struct Sudoku *sdk = malloc(sizeof(struct Sudoku));
	sdk->freeCells = 0;
	return sdk;
}


/* Attempt to fill sudoku and return the number of cells filled. */
int sdks_fill(struct Sudoku *sdk)
{
	int i, j;
	int n = 0;
	#ifdef VERBOSE
		char *mod = "idx_index_init";
	#endif
	for (i = 0; i < SDK_CELLS; i++) {
		if (sdk->cells[i].num) {
			LOG("%s: Skipped filled cell %d\n", mod, i);
			continue;
		}
		LOG("%s: Filling cell %d", mod, i);
		for (j = 1; j < SDK_WIDTH + 1; j++) {
			if (sdk->cells[i].avail == 1 << j) {
				sdk->cells[i].num = j;
				sdk->freeCells--;
				n++;
				LOG("%d\n");
				goto filled;
			}
		}
		LOG("%s: Failed to fill cell %d\n", mod, i);
		filled:
		continue;
	}
	return n;
}

/* Attempt to solve sudoku using indexing techniques.
 * Return -1 i
 * TODO
 */
int sdks_solve(struct Sudoku *sdk)
{
	int n = 0;
	int tmp;
	while (sdk->freeCells) {
		idx_index(sdk->cells);
		if ((tmp = sdks_fill(sdk))) {
			n += tmp;
			continue;
		}
		idx_index_adv(sdk->cells);
		if ((tmp = sdks_fill(sdk))) {
			n += tmp;
			continue;
		}
	}
	return n;
}
