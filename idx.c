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
 * idx.c
 * This file contains procedures for indexing the fields of sudoku cells
 */

#include <math.h>

#include "sdks.h"

static void idx_index_row(struct Cell *row);
static void idx_index_row_adv(struct Cell *row);
static void idx_index_col(struct Cell *cells, int col);
static void idx_index_col_adv(struct Cell *cells, int col);
static void idx_index_grp(struct Cell *cells, int row, int col);
static void idx_index_grp_adv(struct Cell *cells, int row, int col);

/* Index Sudoku for the first time.
 * This is a seperate function because a lot of what it does is only required once.
 */
void idx_index_init(struct Sudoku *sdk)
{
	int i;
	int n = 0;
	/* Set bit flags for all possible numbers. */
	for (i = 0; i < SDK_COLS; i++) {
		n |= 1 << i;
	}
	for (i = 0; i < SDK_CELLS; i++) {
		if (sdk->cells[i].num) {
			continue;
		}
		sdk->cells[i].avail = n;
		sdk->freeCells++;
	}
}

/* TODO TESTING */
/* Remove available numbers for each cell based on rules for existing numbers. */
void idx_index(struct Cell *cells)
{
	int i, j;
	for (i = 0; i < SDK_ROWS; i += SDK_COLS) {
		idx_index_row(&cells[i]);
	}
	for (i = 0; i < SDK_COLS; i++) {
		idx_index_col(cells, i);
	}
	for (i = 0; i < SDK_ROWS; i += SDK_COLS) {
		for (j = 0; j < SDK_COLS; j += sqrt(SDK_COLS)) {
			idx_index_grp(cells, i, j);
		}
	}
}

void idx_index_adv(struct Cell *cells)
{
	int i, j;
	for (i = 0; i < SDK_ROWS; i += SDK_COLS) {
		idx_index_row_adv(&cells[i]);
	}
	for (i = 0; i < SDK_COLS; i++) {
		idx_index_col_adv(cells, i);
	}
	for (i = 0; i < SDK_ROWS; i += SDK_COLS) {
		for (j = 0; j < SDK_COLS; j += sqrt(SDK_COLS)) {
			idx_index_grp_adv(cells, i, j);
		}
	}
}

/* Find existing numbers in a row and remove them from each cell's available numbers. */
static void idx_index_row(struct Cell *row)
{
	int i;
	int n = 0;
	for (i = 0; i < SDK_COLS; i++) {
		if (row[i].num) {
			n |= 1 << row[i].num;
		}
	}
	for (i = 0; i < SDK_COLS; i++) {
		if (row[i].num) {
			continue;
		}
		row[i].avail &= ~n;
	}
}

/* Count the possible fields in a row for each number
 * and trim availble numbers accordingly.
 */
static void idx_index_row_adv(struct Cell *row)
{
	int i, j, pos;
	int n = 0;
	for (i = 1; i < SDK_COLS + 1; i++) {
		/* Skip if number already filled in */
		for (j = 0; j < SDK_COLS; j++) {
			if (row[j].num == i) {
				goto filled;
			} else if (row[j].num) {
				continue;
			}
			if (row[j].avail & 1 << i) {
				n++;
				pos = j;
			}
		}
		if (n == 1) {
			row[pos].avail &= 1 << i;
		}
		filled:
			continue;
	}
}

/* Find existing numbers in a column and remove them from each cell's available numbers. */
static void idx_index_col(struct Cell *cells, int col)
{
	int i;
	int n = 0;
	for (i = col; i < SDK_CELLS; i += SDK_COLS) {
		if (cells[i].num) {
			n |= 1 << cells[i].num;
		}
	}
	for (i = col; i < SDK_CELLS; i += SDK_COLS) {
		if (cells[i].num) {
		cells[i].avail &= ~n;
		}
	}
}

/* Count the possible fields in a column for each number
 * and trim availble numbers accordingly.
 */
static void idx_index_col_adv(struct Cell *cells, int col)
{
	int i, j, pos;
	int n = 0;
	for (i = 1; i < SDK_COLS + 1; i++) {
		for (j = col; j < SDK_CELLS; j += SDK_COLS) {
			if (cells[j].num == i) {
				goto filled;
			} else if (cells[j].num) {
				continue;
			}
			if (cells[j].avail & 1 << i) {
				n++;
				pos = j;
			}
		}
		if (n == 1) {
			cells[pos].avail &= 1 << i;
		}
		filled:
			continue;
	}

}

/* Find existing numbers in a group and remove them from each cell's available numbers. */
static void idx_index_grp(struct Cell *cells, int row, int col)
{
	int i, j;
	int n = 0;
	for (
		i = row * SDK_COLS + col;
		i < (row + sqrt(SDK_ROWS)) * SDK_COLS + col;
		i += SDK_COLS
	) {
		for (j = i; j < i + sqrt(SDK_COLS); j++) {
			if (cells[j].num) {
				n |= 1 << cells[j].num;
			}
		}
	}
	for (
		i = row * SDK_COLS + col;
		i < (row + sqrt(SDK_ROWS)) * SDK_COLS + col;
		i += SDK_COLS
	) {
		for (j = i; j < i + sqrt(SDK_COLS); j++) {
			if (cells[j].num) {
				cells[j].avail &= ~n;
			}
		}
	}
}

static void idx_index_grp_adv(struct Cell *cells, int row, int col)
{
	int i, j, k, pos;
	int n = 0;
	for (i = 1; i < SDK_COLS + 1; i++) {
		for (
			j = row * SDK_COLS + col;
			j < (row + sqrt(SDK_ROWS)) * SDK_COLS + col;
			j += SDK_COLS
		) {
			for (k = j; k < j + sqrt(SDK_COLS); k++) {
				if (cells[k].num == i) {
					goto filled;
				} else if (cells[k].num) {
					continue;
				}
				if (cells[k].avail & 1 << i) {
					n++;
					pos = k;
				}
			}
		}
		if (n == 1) {
			cells[pos].avail &= 1 << i;
		}
		filled:
			continue;
	}
}
