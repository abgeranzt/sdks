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
	struct cell rows[9][9];
	struct cell (*group)[3][3];
};


/* io.c */
void parsesdk(struct sdk *s);
