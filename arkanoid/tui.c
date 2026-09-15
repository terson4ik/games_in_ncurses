#include "tui.h"
#include <curses.h>

enum delays { DELAY_EASY = 10, DELAY_NORM = 100, DELAY_HARD = 1000 };

int get_key(void)
{
    return getch();
}

int init_game(point *field, rectangle *cup, int *delay)
{
    initscr();
    start_color();
    cbreak();
    curs_set(0);
    keypad(stdscr, 1);
    *delay = DELAY_NORM; /* TODO: gived in tui */
    timeout(delay);
    getmaxyx(stdscr, field->y, field->x);
    /* calc cup */
}

void terminate_game(void)
{
    endwin();
}
