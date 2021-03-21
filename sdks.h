#define SDK_W 9
#define SDK_SQ SDK_W * SDK_W

struct pos {
	int row;
	int col;
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
void sdk_print(struct sdk *s);

/* sdks.c */
struct sdk *sdk_init(void);
void cell_index(struct sdk *s, int row, int col);
void sdk_fill(struct sdk *s);

/* check.c */
int sdk_checkrow(struct sdk *s, int row, int col);
int sdk_checkcol(struct sdk *s, int row, int col);
int sdk_checkgrp(struct sdk *s, int row, int col);
