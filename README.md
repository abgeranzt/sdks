# sdks - Sudoku Solver

A simple program to solve sudokus.
Made in my spare time to practice working with C and its core features.

## Design Goals

For fun/learning purposes, a lot of time went into refining and refactoring the code over and over again. As a result, some parts of the program could be considered over-engineered for the task.
My personal motivations for the time spent working on this project include:

- Clearly defined code structure and architecture
- Concise and elegant C code
- Low CPU and memory footprint
- Operations on single bits to further reduce ressource usage and encourage/train thinking about actual memory
- Usage of pointers to create simple and efficient interfaces for allocated memory
- Preprocessor directives to provide conditional compilation of optional features like verbose logging and reduce overhead when no such funtionality is desired
- Adherence to the UNIX philosophy: The program is designed to only do one job and be used in conjunction with other programs; i.e. chained with pipes

At some point in the future this program will provide the underlying logic for a more 'complete' sudoku solving application.
Planned are text recognition on pictures of sudoku puzzles and filling in the puzzles on those images.

## Usage

sdks reads a formatted sudoku from stdin and writes the solution to stdout.

The format consists of a number from 0-9 for each cell with another character between each cell. The 0 represents an empty cell.

The character between each number is ignored and can be any delimiter like ',' and '\n' to increase human readability.

Since the program is intended be as part of a pipeline, no significant validation of the input string takes place.

#### Examples
```shell
./build/sdks < sudoku.txt > solved.txt
```
```shell
echo '0,0,0,0,0,6,8,5,0,0,0,0,0,0,0,0,0,9,0,0,5,8,0,0,0,2,4,9,0,6,0,0,8,0,4,5,7,0,0,0,1,0,0,0,2,2,5,0,6,0,0,3,0,8,4,7,0,0,0,3,5,0,0,6,0,0,0,0,0,0,0,0,0,9,8,2,0,0,0,0,0' | ./build/sdks > tee solved.txt
```

```shell
echo "\
0,0,0,0,0,6,8,5,0,\
0,0,0,0,0,0,0,0,9,\
0,0,5,8,0,0,0,2,4,\
9,0,6,0,0,8,0,4,5,\
7,0,0,0,1,0,0,0,2,\
2,5,0,6,0,0,3,0,8,\
4,7,0,0,0,3,5,0,0,\
6,0,0,0,0,0,0,0,0,\
0,9,8,2,0,0,0,0,0" | ./build/sdks > solved.txt
```

## Building

Building this program is relatively straight forward using your C Compiler of choice.

A makefile will be added in the future.

## Copyright

sdks - Sudoku Solver
Copyright (C) 2021 Marcel Engelke

Licensed under the GNU GPLv3
