#ifndef TUI_H_SENTRY
#define TUI_H_SENTRY

#include "default_structs.h"
#include "tui_conf.h"
#include "arkanoid_conf.h"

#ifdef KEY_ENTER /* In Debian, enter is 10 or '\n' */
#  undef KEY_ENTER
#  define KEY_ENTER '\n'
#endif

#define KEY_SPACE   ' '
#define KEY_ESCAPE  27

int init_game(point *field, rectangle *cup, int *delay);
void terminate_game(void);
int get_key(void);
void draw_rect(const rectangle *r, int chr);
void draw_contour(const rectangle *r, int chr);

#endif
