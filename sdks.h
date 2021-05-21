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
#define SDK_ROWS 9
#define SDK_COLS 9
#define SDK_AVAIL_DEF 0x3fe

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
};

/* sdks.c */
struct Sudoku *sdks_init();
int sdks_fill(struct Sudoku *sdk);
int sdks_solve(struct Sudoku *sdk);

/* idx.c */
void idx_index_init(struct Sudoku *sdk);
void idx_index(struct Cell *cells);
void idx_index_adv(struct Cell *cells);

/* io.c */
void io_parse(struct Sudoku *sdk);
void io_print(struct Sudoku *sdk);
void io_print_stack(struct Sudoku *sdk);
