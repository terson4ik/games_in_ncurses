#ifndef TUI_H_SENTRY
#define TUI_H_SENTRY

#include "default_structs.h"
#include "tui_conf.h"
#include "arkanoid_conf.h"

#define BLOK_PAIRS_COUNT 4
enum api_keys { quit, to_left, to_right, game_pause, resize, no_key };
enum win_state { WIN = 1, LOSE = -1, UNKOWN = 0, EXIT = 100 };
enum delays { SECOND = 1000000, 
              DELAY_EASY = SECOND/1, DELAY_NORM = SECOND/10, 
              DELAY_HARD = SECOND/100, DELAY_STOP = -1 
};

int init_game(point *field, rectangle *cup, enum delays *delay);
void terminate_game(void);
enum api_keys get_key(void);
void draw_rect(const rectangle *r, int chr, enum game_colors_pair pair);
void draw_contour(const rectangle *r, int chr, enum game_colors_pair pair);
void draw_bg(const rectangle *cup, enum game_colors_pair pair);
void set_pause_until_not_pressed(void);
void input_flush(void);
void sleep_frame(enum delays delay);
int handle_resize(point *field, rectangle *cup);
void update_stats(unsigned int score);
int end_game(enum win_state is_win, point *max_xy, unsigned int score);

#endif
