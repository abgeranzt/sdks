/* TODO formatting for functions */

#define FALSE 0
#define TRUE 1

#define SDK_W 9
#define SDK_SQ SDK_W * SDK_W

struct sdk {
	int cells[SDK_SQ];
	int *rows[SDK_W];
};


/* io.c */
void sdk_parsein(struct sdk *s);
void sdk_print(struct sdk *s);

/* sdk.c */
struct sdk *sdk_init(void);
int sdk_fill(struct sdk *s);

/* cell.c */
int cell_check(struct sdk *s, int row, int col);
int cell_fill(struct sdk *s, int row, int col);
int cell_checkrow(struct sdk *s, int row, int col);
int cell_checkcol(struct sdk *s, int row, int col);
int cell_checkgrp(struct sdk *s, int row, int col);
