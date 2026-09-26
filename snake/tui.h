#ifndef TUI_SENTRY_HEADER
#define TUI_SENTRY_HEADER

#include "common_types.h"

#define RECORD_FILE_NAME ".SNAKE_RECORD.txt"
#define MIN_SIZE_WINDOW 10
#define CHR_HORIZONTAL  '$'
#define CHR_VERTICAL    '|'
#define CHR_SN_HEAD     '@'
#define CHR_SN_BODY     'o'
#define CHR_APPLE       '&'
#define CHR_EMPTY       ' '

enum pairs { 
    COMMON_PAIR = 0, BORDER_PAIR, BG_PAIR,
    HEAD_PAIR, BODY_PAIR, APPLE_PAIR,
    WIN_PAIR, LOSE_PAIR
};

enum keys { left, right, up, down, pause, resize, quit, skip };

void graphics_init(rectangle *field, int *delay, int size);
void graphics_handle_resize(rectangle *field, int delay);
void graphics_pause(int delay);
void print_text(const char *str);
void graphics_end(void);
void end_game(rectangle *game_field, int size, int is_win);
void rebuild_game(const rectangle *field, int size);
enum keys get_key(void);

/* all simple draw functions needs update_frame() */
void update_stats(int size);
void draw_char(const point *pnt, char c, int pair);
void draw_rectangle(const rectangle *r, char c, int pair);
void draw_borders(const rectangle *r, char vert, char horiz, int pair);
void update_frame(void);

#endif
