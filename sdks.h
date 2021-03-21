#define SDK_W 9

enum status { SUCCESS, FAILURE };
enum numbers {
	ONE = 1,
	TWO = 2,
	THREE = 4,
	FOUR = 8,
	FIVE = 16,
	SIX = 32,
	SEVEN = 64,
	EIGHT = 128,
	NINE = 256
};

struct cell {
	int number;
	int avail;
};

struct sdk {
	struct cell cells[SDK_SQ];
	struct cell *rows[SDK_W];
};


/* io.c */
void sdk_parsein(struct sdk *s);

/* sdks.c */
struct sdk *sdk_init(void);
void sdk_index(struct sdk *s);
int sdk_checkrow(struct sdk *s, int row, int col);
int sdk_checkcol(struct sdk *s, int row, int col);
int sdk_checkgrp(struct sdk *s, int row, int col);
