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

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "sdks.h"

static uint16_t sdks_fill(struct Sudoku *sdk);
static uint16_t sdks_stack_push(struct Sudoku *sdk);
static void sdks_stack_pop(struct Sudoku *sdk);

static struct Sudoku *sdkStack[SDK_STACK_SIZE];
static uint16_t sdkStackPos = 0;

/* Initialize Sudoku structure and return pointer to it.
 * Create and map arrays of pointers to provide a consistent abstracted interface
 * for rows, columns and groups.
 */
struct Sudoku *sdks_init()
{
	uint16_t i, j, k, l;
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
		i = (
			(j / SDK_GRP_WIDTH) * SDK_WIDTH * SDK_GRP_WIDTH +
			(j % SDK_GRP_WIDTH) * SDK_GRP_WIDTH
		);
		for (k = 0; k < SDK_WIDTH; ) {
			for (l = 0; l < SDK_GRP_WIDTH; l++, k++, i++) {
				sdk->groups[j][k] = &(sdk->cells[i]);
			}
			i += SDK_WIDTH - SDK_GRP_WIDTH;
		}
	}
	return sdk;
}

/* Continously index and fill free sudoku cells until there are none left.
 * Recursively try out numbers in ambigious cells.
 * Return 0 if an error is encountered, non-zero otherwise.
 */
uint16_t sdks_solve(struct Sudoku *sdk)
{
	#ifdef DEBUG
		char *mod = "sdks_solve";
	#endif
	uint16_t i, num;
	while (sdk->freeCells) {
		LOG("%s: Indexing sudoku\n", mod);
		if (!idx_index_sdk(sdk)) {
			goto failure;
		}
		LOG("%s: Filling sudoku\n", mod);
		if (!sdks_fill(sdk)) {
			LOG("%s: Index inconclusive\n", mod);
			for (i = 0; i < SDK_CELLS; i++) {
				if (!(sdk->cells[i].num)) {
					break;
				}
			}
			for (num = 1; num <= SDK_WIDTH; num++) {
				if (!(sdk->cells[i].avail & (1 << num))) {
					continue;
				}
				if (!sdks_stack_push(sdk)) {
					exit(2);
				}
				LOG("%s: Trying number %d in cell %d\n", mod, num, i);
				sdk->cells[i].num = num;
				sdk->freeCells--;
				if (sdks_solve(sdk)) {
					goto success;
				}
				sdks_stack_pop(sdk);
			}
			goto failure;
		}
	}
	success:
		LOG("%s: Done\n", mod);
		return 1;
	failure:
		LOG("%s: Failed to solve sudoku!\n", mod);
		return 0;
}

/* Attempt to fill sudoku and return the number of cells filled.
 * Iterate through the sudoku and attempt to fill all cells.
 * Return the number of cells filled.
 */
static uint16_t sdks_fill(struct Sudoku *sdk)
{
	#ifdef DEBUG
		char *mod = "sdks_fill";
	#endif
	uint16_t num, nFilled;
	uint16_t i;
	for (i = 0, nFilled = 0; i < SDK_CELLS; i++) {
		if (sdk->cells[i].num) {
			continue;
		}
		for (num = 1; num <= SDK_WIDTH; num++) {
			if (sdk->cells[i].avail == (1 << num)) {
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

/* Copy sudoku structure onto the buffer stack.
 * Return 0 if the stack is already full, non-zero otherwise.
 */
static uint16_t sdks_stack_push(struct Sudoku *sdk)
{
	#ifdef DEBUG
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

/* Restore sudoku structure from the buffer stack copy and free the memory used by the latter. */
static void sdks_stack_pop(struct Sudoku *sdk)
{
	#ifdef DEBUG
		char *mod = "sdks_stack_push";
	#endif
	LOG("%s: Restoring sudoku from position %d of the buffer stack\n", mod, sdkStackPos);
	memcpy(sdk, sdkStack[--sdkStackPos], sizeof(struct Sudoku));
	free(sdkStack[sdkStackPos]);
	LOG("%s: Done\n", mod);
}
