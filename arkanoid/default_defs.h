#ifndef DEFAULT_DEFS_H_SENTRY
#define DEFAULT_DEFS_H_SENTRY

typedef struct {
    int x, y;
} point;

typedef struct {
    point up_left, down_right;
} rectangle;

enum delays { DELAY_EASY = 10, DELAY_NORM = 100, DELAY_HARD = 1000 };

#ifdef KEY_ENTER /* in Debian, enter is 10 or '\n' */
#  undef KEY_ENTER
#  define KEY_ENTER '\n'
#endif

#define KEY_SPACE   ' '
#define KEY_ESCAPE  27

#define CHR_BALL    '@'
#define CHR_PADDLE  '='
#define CHR_BLOCK   '*'
#define BLOCK_SIZE  3
#define BLOCK_COLS  4
#define BLOCK_ROWS  3
#define AREA_WIDTH  (BLOCK_SIZE * BLOCK_COLS)
#define PADDLE_LEN  4

#endif
