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

#define SDK_CELLS 81
#define SDK_WIDTH 9
#define SDK_GRP_WIDTH 3
#define SDK_AVAIL_DEF 0x3fe
#define SDK_STACK_SIZE 256

#ifdef DEBUG
	#include <stdio.h>
	#define LOG(...) fprintf(stderr, __VA_ARGS__)
	#define VERBOSE 1
#else
	#define LOG(...) (void)0
#endif

/* Available numbers are stored as bit flags:
 * One as 1 << 1 (...010), two as 1 << 2 (...100) etc.
 * The rightmost bit is ignored.
 */
struct Cell {
	int avail;
	int num;
};

struct Sudoku {
	struct Cell cells[SDK_CELLS];
	int freeCells;
	struct Cell *rows[SDK_WIDTH][SDK_WIDTH];
	struct Cell *cols[SDK_WIDTH][SDK_WIDTH];
	struct Cell *groups[SDK_WIDTH][SDK_WIDTH];
};

/* sdks.c */
struct Sudoku *sdks_init();
int sdks_solve(struct Sudoku *sdk);

/* idx.c */
void idx_index_init(struct Sudoku *sdk);
int idx_index_sdk(struct Sudoku *sdk);

/* io.c */
void io_parse(struct Sudoku *sdk);
void io_print(struct Sudoku *sdk);
void io_print_stack(struct Sudoku *sdk);
