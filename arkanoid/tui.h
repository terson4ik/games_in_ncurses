#ifndef TUI_H_SENTRY
#define TUI_H_SENTRY

#include "default_structs.h"
#include "tui_conf.h"
#include "arkanoid_conf.h"

#define RECORD_FILE_NAME ".ARKANOID_biggest_record.txt"
#define BLOK_PAIRS_COUNT 4
enum api_keys { quit = 1, to_left, to_right, game_pause, resize, no_key };
enum win_state { WIN = 1, LOSE = -1, UNKOWN = 0 };
enum delays { 
    SECOND = 1000000, DELAY_NORM = SECOND/10,
    DELAY_STOP = -1, NEW_LVL = 7
};

int init_game(point *field, rectangle *cup, unsigned long *delay,
              int *is_rebuild);
void terminate_game(void);
enum api_keys get_key(void);
void draw_rect(const rectangle *r, int chr, enum game_colors_pair pair);
void draw_contour(const rectangle *r, int chr, enum game_colors_pair pair);
void draw_bg(const rectangle *cup, enum game_colors_pair pair);
void set_pause_until_not_pressed(void);
void input_flush(void);
void sleep_frame(int delay);
int handle_resize(point *field, rectangle *cup);
void update_stats(unsigned long score, unsigned long lvl);
void end_game(enum win_state is_win, point *max_xy,
              unsigned long score, unsigned long lvl);

#endif
