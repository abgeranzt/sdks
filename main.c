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
 * main.c
 * This file contains the main operating procedure of the program.
 */

#include <stdio.h>

#include "sdks.h"

/* TODO args */
/* TODO testing */
int main()
{
	struct Sudoku *sdk = sdks_init();
	io_parse(sdk);
	idx_index_init(sdk);
	if (!sdks_solve(sdk)) {
		io_print(sdk);
		return 1;
	}
	io_print(sdk);
	return 0;
}
