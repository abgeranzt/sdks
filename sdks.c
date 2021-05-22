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

/* Initialize Sudoku structure and return pointer to it.
 * Create and map arrays of pointers to provide a consistent abstracted interface
 * for rows, columns and groups.
 */
/* TODO TESTING */
struct Sudoku *sdks_init()
{
	int i, j, k, l;
	struct Sudoku *sdk = malloc(sizeof(struct Sudoku));
	sdk->freeCells = 0;
	for (i = 0, j = 0; j < SDK_WIDTH; j++) {
		for (k = 0; k < SDK_WIDTH; k++, i++) {
			sdk->rows[j][k] = &(sdk->cells[i]);
		}
	}
	for (j = 0; j < SDK_WIDTH; j++) {
		for (i = j, k = 0; k < SDK_WIDTH; k++) {
			sdk->cols[j][k] = &(sdk->cells[i]);
			i += SDK_WIDTH;
		}
	}
	for (j = 0; j < SDK_WIDTH; j++) {
		i = (j / SDK_GRP_WIDTH) * SDK_WIDTH * SDK_GRP_WIDTH + (j % SDK_GRP_WIDTH) * SDK_GRP_WIDTH;
		for (k = 0; k < SDK_WIDTH; ) {
			for (l = 0; l < SDK_GRP_WIDTH; l++, k++, i++) {
				sdk->groups[j][k] = &(sdk->cells[i]);
			}
			i += SDK_WIDTH - SDK_GRP_WIDTH;
		}
	}
	return sdk;
}


/* Attempt to fill sudoku and return the number of cells filled. */
int sdks_fill(struct Sudoku *sdk)
{
	#ifdef VERBOSE
		char *mod = "idx_index_init";
	#endif
	int i, j;
	int n = 0;
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
