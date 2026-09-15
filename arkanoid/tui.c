#include "tui.h"
#include <curses.h>

#ifdef KEY_ENTER /* In Debian, enter is 10 or '\n' */
#  undef KEY_ENTER
#  define KEY_ENTER '\n'
#endif

#define KEY_SPACE   ' '
#define KEY_ESCAPE  27


enum api_keys get_key(void)
{
    switch (getch()) {
    case KEY_LEFT:
        return to_left;
    case KEY_RIGHT:
        return to_right;

    case 'q':
    case 'Q':
    case KEY_ESCAPE:
        return quit;
    case 'p':
    case KEY_SPACE:
        return pause;

    default:
        return no_key;
    }
}

void draw_rect(const rectangle *r, int chr)
{
    int x, y;
    for (x = r->up_left.x; x <= r->down_right.x; x++)
        for(y = r->up_left.y; y <=r->down_right.y; y++)
            mvaddch(y, x, chr);
    refresh();
}

void set_pause_until_not_pressed(enum delays delay)
{
    timeout(pause);
    getch();
    timeout(delay);
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
    timeout(*delay);
    getmaxyx(stdscr, field->y, field->x);
    cup->up_left.x = ((field->x-AREA_WIDTH) / 2)-1;
    cup->up_left.y = 0;
    cup->down_right.x = cup->up_left.x + AREA_WIDTH+1;
    cup->down_right.y = cup->up_left.y + AREA_HEIGHT+1;
    /* calc cup */
    return 1;
}

void terminate_game(void)
{
    endwin();
}
