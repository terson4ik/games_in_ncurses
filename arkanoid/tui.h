#ifndef TUI_H_SENTRY
#define TUI_H_SENTRY

#include "default_structs.h"
#include "tui_conf.h"
#include "arkanoid_conf.h"

enum api_keys { quit, to_left, to_right, game_pause, no_key };
enum delays { SECOND = 1000000, 
              DELAY_EASY = SECOND/10, DELAY_NORM = SECOND/10, 
              DELAY_HARD = SECOND/1000, DELAY_STOP = -1 
};

int init_game(point *field, rectangle *cup, enum delays *delay);
void terminate_game(void);
enum api_keys get_key(void);
void draw_rect(const rectangle *r, int chr);
void draw_contour(const rectangle *r, int chr);
void set_pause_until_not_pressed(void);
void input_flush(void);
void sleep_frame(enum delays delay);

#endif
