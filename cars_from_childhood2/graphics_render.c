#include <ncurses.h>
#include "graphics_render.h"

#ifdef KEY_ENTER /* in Debian this value must be return '\n' */
#  undef KEY_ENTER
#  define KEY_ENTER '\n'
#endif

#define KEY_SPACE  ' '
#define KEY_ESCAPE 27

enum time_values {
    FULL_SEC  = 1000000,
    TIME_INIT = FULL_SEC / 10,
    TIME_STEP = 10
};

/* if 0 then error, 1 all right */
int  graphic_init(rectangle *field, rectangle *way, useconds_t *delay)
{
    initscr();
    start_color();
    timeout(0); /* no timeout; usleep used */
    keypad(stdscr, 1);

    curs_set(0);
    cbreak();
    noecho();

    getmaxyx(stdscr, field->down_right.y, field->down_right.x);
    if (0)
        return 0;

    field->up_left.x = 0;
    field->up_left.y = 0;

    way->up_left.x    = (field->down_right.x - STRIPS*CAR_WIDTH-1) / 2;
    way->down_right.x = way->up_left.x + STRIPS*CAR_WIDTH+1;

    way->up_left.y = 0;
    way->down_right.y = field->down_right.y;

    *delay = TIME_INIT;
    
    return 1;
}

enum key_vals get_key(void)
{
    switch (getch()) {
    case 'a':
    case 'A':
    case 'h':
    case 'H':
    case KEY_LEFT:
        return key_left;

    case 'd':
    case 'D':
    case 'k':
    case 'K':
    case KEY_RIGHT:
        return key_right;

    case KEY_SPACE:
    case KEY_ENTER:
    case 'p':
    case 'P':
        return key_pause;

    case 'q':
    case 'Q':
    case KEY_ESCAPE:
        return key_exit;

    default: return skip;
    }
}

void graphic_key_flush(void)
{
    while (getch() != ERR) {
    }
}

void graphic_sleep(useconds_t delay)
{
    usleep(delay);
}

void graphic_decrease_time(useconds_t *delay)
{
    *delay -= TIME_STEP;
}

/* draw function work only in buffer; call update_frame() after any draw() */
void draw_rectangle(const rectangle *rect, int ch)
{
    int y, x;
    for (y = rect->up_left.y; y < rect->down_right.y; y++)
        for (x = rect->up_left.x; x < rect->down_right.x; x++)
            mvaddch(y, x, ch);
}

void draw_rect_vertical_frame(const rectangle *frame, int ch)
{
    const int right = frame->down_right.x;
    const int left = frame->up_left.x;
    int up;
    
    for (up = frame->up_left.y; up <= frame->down_right.y; up++) {
        mvaddch(up, left, ch);
        mvaddch(up, right, ch);
    }
}

void draw_own_car(const point *up_left)
{
    mvprintw(up_left->y, up_left->x, "%c%c%c",
             CHR_OWN_CAR_WHEELS, CHR_OWN_CAR_BAMPER, CHR_OWN_CAR_WHEELS);

    mvprintw(up_left->y+1, up_left->x, "%c%c%c",
             CHR_EMPTY, CHR_OWN_CAR_BODY, CHR_EMPTY);

    mvprintw(up_left->y+2, up_left->x, "%c%c%c",
             CHR_OWN_CAR_WHEELS, CHR_OWN_CAR_BAMPER, CHR_OWN_CAR_WHEELS);
}

void draw_enemy_car(const point *up_left)
{
    mvprintw(up_left->y, up_left->x, "%c%c%c",
             CHR_ENEMY_CAR_WHEELS, CHR_ENEMY_CAR_BAMPER, CHR_ENEMY_CAR_WHEELS);

    mvprintw(up_left->y+1, up_left->x, "%c%c%c",
             CHR_EMPTY, CHR_ENEMY_CAR_BODY, CHR_EMPTY);

    mvprintw(up_left->y+2, up_left->x, "%c%c%c",
             CHR_ENEMY_CAR_WHEELS, CHR_ENEMY_CAR_BAMPER, CHR_ENEMY_CAR_WHEELS); 
}

void draw_hide_car(const point *up_left)
{
    int i;
    int y = up_left->y;
    for (i = 3; i > 0; y++, i--)
        mvprintw(y, up_left->x, "%c%c%c", CHR_EMPTY, CHR_EMPTY, CHR_EMPTY);
}

/* init position must be 0 or 1 */
void draw_road(char position, int x, int max_y)
{
    int y, empty;

    empty = position % 2;
    for (y = 0; y < max_y; y++) {
        mvaddch(y, x, empty ? CHR_EMPTY : CHR_STRIP);
        empty = !empty;
    }
}

/* no refrech in draws(), then after any draw call this update_frame() */
void update_frame(void)
{
    refresh();
}

void graphic_pause(void)
{
    timeout(-1);
    getch();
    timeout(0);
}

void graphic_end(void)
{
    endwin();
}