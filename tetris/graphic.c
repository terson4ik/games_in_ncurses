#include <curses.h>
#include <unistd.h>

#include "graphic.h"

#ifdef KEY_ENTER
#  undef KEY_ENTER /* by default curses use 10('\n') */
#endif

#define KEY_ENTER '\n'
#define KEY_SPACE ' '

static void pair_init(void);

enum keys get_key(void)
{

}

void graphic_init(void)
{
    initscr();
    start_color();
    if (has_colors())
        pair_init();

    cbreak();
    curs_set(0);
    keypad(stdscr, 1);
    noecho();
    timeout(0); /* No time to wait */

}

void graphic_resize(rectangle *field, rectangle *cup)
{
}

void graphic_end(void)
{
    endwin();
}

void update_screen(void)
{
    refresh();
}

void graphic_sleep(u_seconds_t delay)
{
    usleep(delay);
}

static void pair_init(void)
{
}
