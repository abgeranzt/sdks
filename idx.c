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
#include "log.h"

static void idx_index_row(struct Cell *cells, int row);
static int idx_index_row_adv(struct Cell *cells, int row);
static void idx_index_col(struct Cell *cells, int col);
static int idx_index_col_adv(struct Cell *cells, int col);
static void idx_index_grp(struct Cell *cells, int row, int col);
static int idx_index_grp_adv(struct Cell *cells, int row, int col);

/* Initialize the index for unfilled cells. */
void idx_index_init(struct Sudoku *sdk)
{
	int i;
	#ifdef VERBOSE
		char *mod = "idx_index_init";
	#endif
	LOG("%s: Initializing index\n", mod);
	for (i = 0; i < SDK_CELLS; i++) {
		if (sdk->cells[i].num) {
			LOG("%s: Skipped cell %d\n", mod, i);
			continue;
		}
		sdk->cells[i].avail = SDK_AVAIL_DEF;
		LOG("%s: Indexed cell %d\n", mod, i);
	}
	LOG("%s: Done\n", mod);
}

/* TODO TESTING */
/* Remove available numbers for each cell based on rules for existing numbers. */
void idx_index(struct Cell *cells)
{
	int i, j;
	#ifdef VERBOSE
		char *mod = "idx_index";
	#endif
	LOG("%s: Indexing rows\n", mod);
	for (i = 0; i < SDK_ROWS; i++) {
		idx_index_row(cells, i);
	}
	for (i = 0; i < SDK_COLS; i++) {
		idx_index_col(cells, i);
	}
	for (i = 0; i < SDK_ROWS; i+= sqrt(SDK_ROWS)) {
		for (j = 0; j < SDK_COLS; j += sqrt(SDK_COLS)) {
			idx_index_grp(cells, i, j);
		}
	}
}

/* Perform advanced indexing using more sophisticated rules.
 * Return non-zero upon success and 0 if a logical error is encountered.
 * I.e. sudoku is unsolvable.
 */
int idx_index_adv(struct Cell *cells)
{
	int i, j;
	#ifdef VERBOSE
		char *mod = "idx_index_adv";
	#endif
	LOG("%s: Indexing rows\n", mod);
	for (i = 0; i < SDK_ROWS; i++) {
		if (!idx_index_row_adv(cells, i)) {
			goto failed;
		}
	}
	LOG("%s: Indexing columns\n", mod);
	for (i = 0; i < SDK_COLS; i++) {
		if (! idx_index_col_adv(cells, i)) {
			goto failed;
		}
	}
	LOG("%s: Indexing groups\n", mod);
	for (i = 0; i < SDK_ROWS; i+= sqrt(SDK_ROWS)) {
		for (j = 0; j < SDK_COLS; j += sqrt(SDK_COLS)) {
			if (!idx_index_grp_adv(cells, i, j)) {
				goto failed;
			}
		}
	}
	LOG("%s: Done\n", mod);
	return 1;
	failed:
		LOG("%s: Error while indexing!\n", mod);
		return 0;
}

/* Find existing numbers in a row and remove them from each cell's available numbers. */
static void idx_index_row(struct Cell *cells, int row)
{
	int i;
	int n = 0;
	#ifdef VERBOSE
		char *mod = "idx_index_row";
	#endif
	LOG("%s: Indexing row %d\n", mod, row);
	for (i = row * SDK_COLS; i < (row + 1) * SDK_COLS; i++) {
		if (cells[i].num) {
			n |= 1 << cells[i].num;
			LOG("%s: Found existing number %d\n", mod, cells[i].num);
		}
	}
	LOG("%s: Available numbers: %x\n", mod, SDK_AVAIL_DEF ^ n);
	LOG("%s: Updating cells\n", mod);
	for (i = row * SDK_COLS; i < (row + 1) * SDK_COLS; i++) {
		if (cells[i].num) {
			continue;
		}
		cells[i].avail &= ~n;
		LOG("%s: Cell %d - avail: %x\n", mod, i, cells[i].avail);
	}
	LOG("%s: Done\n", mod);
}

/* Count the possible fields in a row for each number
 * and trim availble numbers accordingly.
 * Return zero if a number cannot be filled in.
 */
static int idx_index_row_adv(struct Cell *cells, int row)
{
	int i, n, pos, posAvail;
	int avail = SDK_AVAIL_DEF;
	#ifdef VERBOSE
		char *mod = "idx_index_row_adv";
	#endif
	LOG("%s: Indexing row %d\n", mod, row);
	for (i = row * SDK_COLS; i < (row + 1) * SDK_COLS; i++) {
		if (cells[i].num) {
			avail &= ~(1 << cells[i].num);
		}
	}
	for (n = 1; n < SDK_COLS + 1; n++) {
		/* Skip if the number is already filled in. */
		if (!(avail & (1 << n))) {
			LOG("%s: Skipped existing number %d\n", mod, n);
			continue;
		}
		posAvail = 0;
		for (i = row * SDK_COLS; i < (row + 1) * SDK_COLS; i++) {
			if (cells[i].num) {
				continue;
			}
			if (cells[i].avail & 1 << n) {
				posAvail++;
				pos = i;
			}
		}
		switch (n) {
		case 0:
			LOG("%s: No field in row available for number %d\n", mod, row, n);
			return 0;
		case 1:
			cells[pos].avail &= 1 << n;
			LOG("%s: Cell %d - avail: %x\n", mod, pos, n);
			break;
		}
	}
	LOG("%s: Done\n", mod);
	return 1;
}

/* Find existing numbers in a column and remove them from each cell's available numbers. */
static void idx_index_col(struct Cell *cells, int col)
{
	int i;
	int n = 0;
	#ifdef VERBOSE
		char *mod = "idx_index_col";
	#endif
	LOG("%s: Indexing column %d\n", mod, col);
	for (i = col; i < SDK_CELLS; i += SDK_COLS) {
		if (cells[i].num) {
			n |= 1 << cells[i].num;
			LOG("%s: Found existing number %d\n", mod, cells[i].num);
		}
	}
	LOG("%s: Available numbers: %x\n", mod, SDK_AVAIL_DEF ^ n);
	LOG("%s: Updating cells\n", mod);
	for (i = col; i < SDK_CELLS; i += SDK_COLS) {
		if (cells[i].num) {
			continue;
		}
		cells[i].avail &= ~n;
		LOG("%s: Cell %d - avail: %x\n", mod, i, cells[i].avail);
	}
	LOG("%s: Done\n", mod);
}

/* Count the possible fields in a column for each number
 * and trim availble numbers accordingly.
 * Return zero if a number cannot be filled in.
 */
static int idx_index_col_adv(struct Cell *cells, int col)
{
	int i, j, pos;
	int n = 0;
	#ifdef VERBOSE
		char *mod = "idx_index_col_adv";
	#endif
	LOG("%s: Indexing column %d\n", mod, col);
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
		switch (n) {
		case 0:
			LOG("%s: No field in row available for number %d\n", mod, row, i);
			return 0;
		case 1:
			cells[pos].avail &= 1 << i;
			break;
		}
		filled:
			continue;
	}
	LOG("%s: Done\n", mod);
	return 1;
}

/* Find existing numbers in a group and remove them from each cell's available numbers. */
static void idx_index_grp(struct Cell *cells, int row, int col)
{
	int i, j;
	int n = 0;
	#ifdef VERBOSE
		char *mod = "idx_index_grp";
	#endif
	LOG("%s: Indexing group %d:%d", mod, row , col);
	for (
		i = row * SDK_COLS + col;
		i < (row + sqrt(SDK_ROWS)) * SDK_COLS + col;
		i += SDK_COLS
	) {
		for (j = i; j < i + sqrt(SDK_COLS); j++) {
			if (cells[j].num) {
				n |= 1 << cells[j].num;
				LOG("%s: Found existing number %d\n", mod, cells[i].num);
			}
		}
	}
	LOG("%s: Available numbers: %x\n", mod, SDK_AVAIL_DEF ^ n);
	LOG("%s: Updating cells\n", mod);
	for (
		i = row * SDK_COLS + col;
		i < (row + sqrt(SDK_ROWS)) * SDK_COLS + col;
		i += SDK_COLS
	) {
		for (j = i; j < i + sqrt(SDK_COLS); j++) {
			if (cells[j].num) {
				LOG("Skipped cell %d\n", j);
				continue;
			}
			cells[j].avail &= ~n;
			LOG("%s: Cell %d - avail: %x\n", mod, i, cells[i].avail);
		}
	}
	LOG("%s: Done\n", mod);
}

/* Count the possible fields in a group for each number
 * and trim availble numbers accordingly.
 * Return zero if a number cannot be filled in.
 */
static int idx_index_grp_adv(struct Cell *cells, int row, int col)
{
	int i, j, k, pos;
	int n = 0;
	#ifdef VERBOSE
		char *mod = "idx_index_grp_adv";
	#endif
	LOG("%s: Indexing group %d:%d", mod, row , col);
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
		switch (n) {
		case 0:
			LOG("%s: No field in row available for number %d\n", mod, row, i);
			return 0;
		case 1:
			cells[pos].avail &= 1 << i;
			break;
		}
		filled:
			continue;
	}
	LOG("%s: Done\n", mod);
	return 1;
}
