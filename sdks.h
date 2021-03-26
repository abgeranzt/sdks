/* TODO formatting for functions */

#define FALSE 0
#define TRUE 1

#define SDK_GW 3
#define SDK_W SDK_GW * SDK_GW
#define SDK_SQ SDK_W * SDK_W

struct cell {
	int num;
	int av;
};

struct sdk {
	struct cell cells[SDK_SQ];
	struct cell *rows[SDK_W];
	int nfree;
};


/* io.c */
void sdk_parsein(struct sdk *s);
void sdk_print(struct sdk *s);

/* sdk.c */
struct sdk *sdk_init(void);
int sdk_fill(struct sdk *s);
void sdk_index_basic(struct sdk *s);
void sdk_index_update(struct sdk *s, int n, int row, int col);

/* cell.c */
int cell_index_basic(struct sdk *s, int row, int col);
int cell_fill(struct sdk *s, int row, int col);
int cell_checkrow(struct sdk *s, int row, int col);
int cell_checkcol(struct sdk *s, int row, int col);
int cell_checkgrp(struct sdk *s, int row, int col);
