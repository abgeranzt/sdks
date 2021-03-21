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

struct sdk {
	int rows[9][9];
	int (*groups)[3][9];
};

int parsesdk(struct sdk *s);
