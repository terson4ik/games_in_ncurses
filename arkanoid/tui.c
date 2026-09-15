#include "tui.h"
#include <curses.h>

enum delays { DELAY_EASY = 10, DELAY_NORM = 100, DELAY_HARD = 1000 };

int get_key(void)
{
    return getch();
}

void draw_rect(const rectangle *r, int chr)
{
    int x, y;
    for (x = r->up_left.x; x <= r->down_right.x; x++)
        for(y = r->up_left.y; y <=r->down_right.y; y++)
            mvaddch(y, x, chr);
    refresh();
}

void draw_contour(const rectangle *r, int chr)
{
    int x, y;
    /* walls */
    for (y = r->up_left.y; y <= r->down_right.y; y++) {
        mvaddch(y, r->up_left.x, chr);
        mvaddch(y, r->down_right.x, chr);
    }

    /* floor */
    for (x = r->up_left.x; x <= r->down_right.x; x++)
        mvaddch(r->down_right.y, x, chr);
/* mvaddch(r->down_right.y, x, chr); */
    refresh();
}

int init_game(point *field, rectangle *cup, int *delay)
{
    initscr();
    start_color();
    cbreak();
    curs_set(0);
    noecho();
    keypad(stdscr, 1);
    *delay = DELAY_NORM; /* TODO: gived in tui */
    timeout(delay);
    getmaxyx(stdscr, field->y, field->x);
    cup->up_left.x = ((field->x-AREA_WIDTH) / 2)-1;
    cup->up_left.y = 0;
    cup->down_right.x = cup->up_left.x + AREA_WIDTH+1;
    cup->down_right.y = cup->up_left.y + AREA_HEIGHT+1;
    /* calc cup */
}

void terminate_game(void)
{
    endwin();
}
