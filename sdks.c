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

#include "sdks.h"


/* Initialize Sudoku structure and return pointer to it. */
struct Sudoku *sdks_init()
{
	return malloc(sizeof(struct Sudoku));
}


/* Attempt to fill in cells and return the number of filled cells */
int sdks_fill(struct Sudoku *sdk)
{
	int i, j;
	int n = 0;
	for (i = 0; i < SDK_CELLS; i++) {
		if (sdk->cells[i].num) {
			continue;
		}
		for (j = 1; j < SDK_ROWS + 1; j++) {
			if (sdk->cells[i].avail == 1 << j) {
				sdk->cells[i].num = j;
				sdk->freeCells--;
				n++;
				break;
			}
		}
	}
	return n;
}
