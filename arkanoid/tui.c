#include "tui.h"
#include <stdlib.h>
#include <time.h>
#include <unistd.h> /* usleep(); timeout too bad for rendering frames */
#include <curses.h>

#ifdef KEY_ENTER /* In Debian, enter is 10 or '\n' */
#  undef KEY_ENTER
#  define KEY_ENTER '\n'
#endif

#define KEY_SPACE   ' '
#define KEY_ESCAPE  27

void sleep_frame(enum delays delay)
{
    usleep(delay);
}

void update_stats(unsigned int score)
{
    attrset(A_REVERSE);
    mvprintw(0, 0, "score:%u    ", score);
    attroff(A_REVERSE);
}

enum api_keys get_key(void)
{
    switch (getch()) {
    case KEY_LEFT:
    case 'a':
    case 'A':
        return to_left;

    case 'd':
    case 'D':
    case KEY_RIGHT:
        return to_right;

    case 'q':
    case 'Q':
    case KEY_ESCAPE:
        return quit;
    case 'p':
    case KEY_SPACE:
        return game_pause;

    case KEY_RESIZE:
        return resize;
    default:
        return no_key;
    }
}

void draw_bg(const rectangle *cup, enum game_colors_pair pair)
{
    rectangle target_rect = *cup;
    target_rect.up_left.x++;
    target_rect.down_right.x--;
    target_rect.down_right.y--;
    draw_rect(&target_rect, ' ', pair);
}

void draw_rect(const rectangle *r, int chr, enum game_colors_pair pair)
{
    int x, y;
        if (has_colors())
        attrset(COLOR_PAIR(pair));
    else
        attrset(A_BOLD);

    for (x = r->up_left.x; x <= r->down_right.x; x++)
        for(y = r->up_left.y; y <=r->down_right.y; y++)
            mvaddch(y, x, chr);
    refresh();
}

void set_pause_until_not_pressed(void)
{
    timeout(DELAY_STOP);
    getch();
    timeout(0);
}

void draw_contour(const rectangle *r, int chr, enum game_colors_pair pair)
{
    int x, y;
    if (has_colors())
        attrset(COLOR_PAIR(pair));
    else
        attrset(A_REVERSE | A_BOLD);

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

static void init_game_pairs(void) 
{
    init_pair(BALL_PAIR,   COLOR_RED,     COLOR_RED);
    init_pair(PADDLE_PAIR, COLOR_GREEN,   COLOR_GREEN);
    init_pair(BORDER_PAIR, COLOR_YELLOW,  COLOR_YELLOW);
    init_pair(BG_PAIR,     COLOR_CYAN,    COLOR_CYAN);
    init_pair(WIN_PAIR,    COLOR_BLACK,   COLOR_GREEN);
    init_pair(LOSE_PAIR,   COLOR_BLACK,     COLOR_RED);
    
    init_pair(BLOCK_PAIR_1,  COLOR_MAGENTA, COLOR_MAGENTA);
    init_pair(BLOCK_PAIR_2,  COLOR_GREEN, COLOR_GREEN);
    init_pair(BLOCK_PAIR_3,  COLOR_WHITE, COLOR_WHITE);
    init_pair(BLOCK_PAIR_4,  COLOR_BLUE, COLOR_BLUE);
}

int handle_resize(point *field, rectangle *cup)
{
    clear();
    getmaxyx(stdscr, field->y, field->x);
    if (field->x < MIN_TERM_SIZE || field->y < MIN_TERM_SIZE)
        return 0;

    cup->up_left.x = ((field->x-AREA_WIDTH) / 2)-1;
    cup->up_left.y = 0;
    cup->down_right.x = cup->up_left.x + AREA_WIDTH+1;
    cup->down_right.y = cup->up_left.y + AREA_HEIGHT+1;
}

int init_game(point *field, rectangle *cup, enum delays *delay)
{
    initscr();
    start_color();
    if (has_colors())
        init_game_pairs();
    cbreak();
    curs_set(0);
    noecho();
    keypad(stdscr, 1);
    *delay = DELAY_NORM; /* TODO: gived in tui */
    timeout(0);
    srand(time(NULL));
    if (!handle_resize(field, cup))
        return 0;

    /* calc cup */
    return 1;
}

void input_flush(void)
{
    while (getch() != ERR)
        ;
}

int end_game(enum win_state is_win, point *max_xy, unsigned int score)
{
    int key, x, y;
    rectangle pseudo_rect;
    pseudo_rect.up_left.x = 0;
    pseudo_rect.up_left.y = 0;
    pseudo_rect.down_right.x = max_xy->x;
    pseudo_rect.down_right.y = max_xy->y;
    x = (max_xy->x-AREA_WIDTH-2)/2;
    y = max_xy->y/2;
    clear();
    attrset(A_REVERSE | A_BOLD);
    if (is_win == WIN) {
        draw_rect(&pseudo_rect, ' ', WIN_PAIR);
        mvaddstr(y, x, "WOW!!!");
        mvaddstr(y + 1, x, "YOU WIN! :)");
    } else {
        draw_rect(&pseudo_rect, ' ', LOSE_PAIR);
        mvaddstr(y, x, "WE ARE SORRY.");
        mvaddstr(y + 1, x, "YOU LOSER :(");
    }
    mvprintw(y + 2, x, "YOUR SCORE: %u", score);
    attrset(A_BLINK | A_UNDERLINE | A_BOLD);
    mvaddstr(y + 3, x, "TYPE ENTER TO CONTINUE. . .");
    refresh();
    timeout(DELAY_STOP);
    key = ERR;
    while ((key = getch()) != KEY_ENTER)
        if (key == KEY_RESIZE) {
            getmaxyx(stdscr, max_xy->y, max_xy->x);
            end_game(is_win, max_xy, score);
            break;
        }
}

void terminate_game(void)
{
    endwin();
}
