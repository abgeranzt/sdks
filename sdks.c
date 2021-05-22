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
 * This file contains procedures to manipulate data in the sudoku structure.
 */

#include <stdlib.h>
#include <string.h>

#include "log.h"
#include "sdks.h"

static int sdks_stack_push(struct Sudoku *sdk);
static void sdks_stack_pop(struct Sudoku *sdk);

static struct Sudoku *sdkStack[SDK_STACK_SIZE];
static int sdkStackPos = 0;

/* Initialize Sudoku structure and return pointer to it.
 * Create and map arrays of pointers to provide a consistent abstracted interface
 * for rows, columns and groups.
 */
/* TODO TESTING */
struct Sudoku *sdks_init()
{
	int i, j, k, l;
	struct Sudoku *sdk = (struct Sudoku*) malloc(sizeof(struct Sudoku));
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


/* Attempt to fill sudoku and return the number of cells filled.
 * Iterate through the sudoku and attempt to fill all cells.
 * Return the number of cells filled
 */
int sdks_fill(struct Sudoku *sdk)
{
	#ifdef VERBOSE
		char *mod = "sdks_fill";
	#endif
	int i, num, nFilled;
	for (i = 0, nFilled = 0; i < SDK_CELLS; i++) {
		if (sdk->cells[i].num) {
			continue;
		}
		for (num = 1; num <= SDK_WIDTH; num++) {
			if (sdk->cells[i].avail & (1 << num)) {
				sdk->cells[i].num = num;
				sdk->freeCells--;
				nFilled++;
				LOG("%s: Filled cell %d with number %d\n", mod, i, num);
				break;
			}
		}
	}
	LOG("%s: Filled %d cells, %d remaining\n", mod, nFilled, sdk->freeCells);
	return nFilled;
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
	LOG("%s: Done\n", mod);
	return 1;
}

/* Copy sudoku structure onto the buffer stack.
 * Return 0 if the stack is already full, non-zero otherwise.
 */
static int sdks_stack_push(struct Sudoku *sdk)
{
	#ifdef VERBOSE
		char *mod = "sdks_stack_push";
	#endif
	if (sdkStackPos == SDK_STACK_SIZE - 1) {
		LOG("%s: Error: Stack already at maximum size of %d!\n", mod, SDK_STACK_SIZE);
		return 0;
	}
	LOG("%s: Copying sudoku to position %d of the buffer stack\n", mod, sdkStackPos);
	sdkStack[sdkStackPos] = (struct Sudoku*) malloc(sizeof(struct Sudoku));
	memcpy(sdkStack[sdkStackPos++], sdk, sizeof(struct Sudoku));
	LOG("%s: Done\n", mod);
	return 1;
}

/* Restore sudoku structure from the buffer stack copy and the memory used by the latter. */
static void sdks_stack_pop(struct Sudoku *sdk)
{
	#ifdef VERBOSE
		char *mod = "sdks_stack_push";
	#endif
	LOG("%s: Restoring sudoku from position %d of the buffer stack\n", mod, sdkStackPos);
	memcpy(sdk, sdkStack[sdkStackPos], sizeof(struct Sudoku));
	free(sdkStack[sdkStackPos--]);
	LOG("%s: Done\n", mod);
}
