all: sdks
files =	sdks.h \
	sdks.c \
	idx.c \
	io.c \
	main.c
out_dir:
	mkdir -p bin
sdks: out_dir
	gcc -o bin/sdks ${files}
debug: out_dir
	gcc -D DEBUG -o bin/sdks ${files}
