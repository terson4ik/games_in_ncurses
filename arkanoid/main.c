#include <curses.h>
#include "default_defs.h"
#include "arkanoid.h"
#include "tui.h"

int main(void)
{
    cbreak();
    curs_set(0);
    keypad(stdscr, 1);
    timeout(DELAY_NORM);

    endwin();
    return 0;
}