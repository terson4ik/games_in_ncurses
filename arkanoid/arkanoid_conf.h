#ifndef ARKA_CONF_SENTRY
#define ARKA_CONF_SENTRY

/* PLEASE, DON'T TOUCH IT */
#define BLOCK_WIDTH     3
#define BLOCK_HEIGHT    2

/* set prefered value in Makefile */
#ifndef BLOCK_ROWS
#  define BLOCK_ROWS      3
#endif

#ifndef BLOCK_COLS 
#  define BLOCK_COLS      4
#endif

#define AREA_WIDTH      (BLOCK_WIDTH * BLOCK_COLS)
#define AREA_HEIGHT     (BLOCK_ROWS * 6)
#define MIN_TERM_SIZE   (AREA_WIDTH + 10)
#define PADDLE_LEN      4

#endif