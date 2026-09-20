#include <stdio.h> /* Save into file */
#include <ncurses.h>
#include "graphics_render.h"

#ifdef KEY_ENTER /* In Debian this value must be return '\n' */
#  undef KEY_ENTER
#  define KEY_ENTER '\n'
#endif

#define KEY_SPACE  ' '
#define KEY_ESCAPE 27

static void sec_to_hrs_mins_secs(unsigned int sec, unsigned int *hrs,
                                 unsigned int *mins, unsigned int *secs )
{
    *hrs  = sec/60 / 60;
    *mins = (sec/60) % 60;
    *secs = sec % 60;
}

static void init_my_pairs(void)
{
    init_pair(playr_pair, COLOR_CYAN,   COLOR_BLACK);
    init_pair(enem_pair,  COLOR_RED,    COLOR_BLACK);
    init_pair(brd_pair,   COLOR_YELLOW, COLOR_YELLOW);
    init_pair(lose_pair,  COLOR_BLACK,  COLOR_BLUE);
}

/* If 0 then error, 1 all right */
void graphic_init(rectangle *field, rectangle *way, useconds_t *delay)
{
    initscr();
    start_color();
    if (has_colors())
        init_my_pairs();
    /* timeout(0) handlided in resize func */
    keypad(stdscr, 1);

    curs_set(0);
    cbreak();
    noecho();

    graphic_resize(field, way);

    *delay = TIME_INIT;
}

void graphic_resize(rectangle *field, rectangle *way)
{
    int row, col;

    field->up_left.x = 0;
    field->up_left.y = 0;
    getmaxyx(stdscr, row, col);
    timeout(-1);
    while (row < MIN_SCR_HEIGHT || col < MIN_SCR_WIDTH) {
        mvaddstr(0, 0, "SMALL");
        mvaddstr(1, 0, "SCREEN");
        while (getch() != KEY_RESIZE) {
        }
        getmaxyx(stdscr, row, col);
    }
    timeout(0);

    field->down_right.x = col;
    field->down_right.y = row;
    
    way->up_left.x    = (field->down_right.x - STRIPS*CAR_WIDTH-1) / 2;
    way->down_right.x = way->up_left.x + STRIPS*CAR_WIDTH+1;

    way->up_left.y = 0;
    way->down_right.y = field->down_right.y;
}

void graphic_erase_screen(void)
{
    clear();
}

enum key_vals get_key(void)
{
    switch (getch()) {
    case 'a':
    case 'A':
    case 'j':
    case 'J':
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
    case KEY_RESIZE:
        return key_resize;
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

/* Draw function work only in buffer; call update_frame() after any draw() */
void draw_rectangle(const rectangle *rect, int ch, enum my_color_pair pair)
{
    int y, x;
    if (has_colors())
        attrset(COLOR_PAIR(pair));
    else
        attrset(COLOR_PAIR(common_pair));

    for (y = rect->up_left.y; y < rect->down_right.y; y++)
        for (x = rect->up_left.x; x < rect->down_right.x; x++)
            mvaddch(y, x, ch);
}

void draw_rect_vertical_frame(const rectangle *frame, int ch,
                              enum my_color_pair pair)
{
    const int right = frame->down_right.x;
    const int left = frame->up_left.x;
    int up;

    if (has_colors())
        attrset(COLOR_PAIR(pair));
    else
        attrset(A_REVERSE);

    for (up = frame->up_left.y; up <= frame->down_right.y; up++) {
        mvaddch(up, left, ch);
        mvaddch(up, right, ch);
    }
}

void draw_own_car(const point *up_left)
{
    if (has_colors())
        attrset(COLOR_PAIR(playr_pair));
    else
        attrset(A_BOLD);

    mvprintw(up_left->y, up_left->x, "%c%c%c",
             CHR_OWN_CAR_WHEELS, CHR_OWN_CAR_BAMPER, CHR_OWN_CAR_WHEELS);

    mvprintw(up_left->y+1, up_left->x, "%c%c%c",
             CHR_EMPTY, CHR_OWN_CAR_BODY, CHR_EMPTY);

    mvprintw(up_left->y+2, up_left->x, "%c%c%c",
             CHR_OWN_CAR_WHEELS, CHR_OWN_CAR_BAMPER, CHR_OWN_CAR_WHEELS);
}

void draw_enemy_car(const point *up_left)
{
    if (has_colors())
        attrset(COLOR_PAIR(enem_pair));
    else
        attrset(common_pair);

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

    attrset(COLOR_PAIR(common_pair));

    for (i = 3; i > 0; y++, i--)
        mvprintw(y, up_left->x, "%c%c%c", CHR_EMPTY, CHR_EMPTY, CHR_EMPTY);
}

/* Init position must be 0 or 1 */
void draw_road(char position, int x, int max_y)
{
    int y, empty;

    attrset(A_BOLD);
    empty = position % 2;
    for (y = 0; y < max_y; y++) {
        mvaddch(y, x, empty ? CHR_EMPTY : CHR_STRIP);
        empty = !empty;
    }
}

void draw_update_stats(size_t meters, unsigned int gear, unsigned int sec)
{
    unsigned int hours, minut, secs;

    attrset(A_REVERSE);
    mvprintw(0, 0, "METERS TRAVELED: %-6lu", meters);
    mvprintw(1, 0, "GEAR ENGAGED:    %-6u", gear);
    sec_to_hrs_mins_secs(sec, &hours, &minut, &secs);
    mvprintw(2, 0, "TIME HAS PASSED: h:%2d m:%2d s:%2d", hours, minut, secs);
    attroff(A_REVERSE);
}

/* No refrech in draws(), then after any draw call this update_frame() */
void update_frame(void)
{
    refresh();
}

static void write_new_rec(size_t meters, unsigned int sec)
{
    FILE *new_rec = fopen(RECORD_FILE_NAME, "w");
    if (!new_rec)
        return;

    fprintf(new_rec, "%ld %u", meters, sec);

    fclose(new_rec);
}

/* If new record then write 0 to parameters */
static void read_and_write_record(size_t *meters, unsigned int *sec)
{
    FILE *recptr = fopen(RECORD_FILE_NAME, "r");
    if (recptr) {
        size_t tmp_met;
        unsigned int tmp_sec;

        fscanf(recptr, "%lu %u", &tmp_met, &tmp_sec);
        fclose(recptr);

        if (tmp_met < *meters && tmp_sec < *sec)
            write_new_rec(*meters, *sec);
        else
            return;
    } else {
        write_new_rec(*meters, *sec);
    }

    *meters = *sec = 0; /* new rec! */
}

void graphic_show_lose_src(rectangle *fld, size_t meters, unsigned int sec)
{ /* If you redacting strings, please, set largest to this variable */
    const char enter_larg_str[] = "PRESS ENTER TO CONTINUE . . .";
    const int x = (fld->down_right.x - sizeof(enter_larg_str)) / 2;
    int y = fld->down_right.y/2 - 4; /* 4 strings is avarage value */
    
    unsigned int hrs, mins, secs, tmp_sec;
    size_t tmp_met;

    int key;


    /* Work also as clean() */
    draw_rectangle(fld, CHR_EMPTY, lose_pair);
    if (has_colors())
        attrset(COLOR_PAIR(lose_pair));
    else
        attrset(COLOR_PAIR(common_pair));

    mvaddstr(y++, x, "YOU SMASH A CAR :(");
    
    tmp_met = meters;
    tmp_sec = sec;
    read_and_write_record(&tmp_met, &tmp_sec);
    attrset(A_REVERSE);
    if (tmp_met == 0) { /* new rec! */
        mvaddstr(y++, x, "BUT!");
        mvaddstr(y++, x, "WOW!");
        mvaddstr(y++, x, "IT'S NEW RECORD!");
    } else {
        mvaddstr(y++, x, "YOUR OLD RECORD:");
        mvprintw(y++, x, "BEST TRAVEL: %ldm", tmp_met);
        sec_to_hrs_mins_secs(tmp_sec, &hrs, &mins, &secs);
        mvprintw(y++, x, "BEST TIME: h:%2u m:%2u s:%2u", hrs, mins, secs);
        attroff(A_REVERSE);
    }

    mvprintw(y++, x, "TRAVELED: %ldm", meters);
    sec_to_hrs_mins_secs(sec, &hrs, &mins, &secs);
    mvprintw(y++, x, "TIME SPENT: h:%2u m:%2u s:%2u", hrs, mins, secs);
    attrset(A_BLINK | A_REVERSE);
    mvaddstr(y, x, enter_larg_str);
    refresh();

    timeout(-1);
    while ((key = getch()) != KEY_ENTER)
        if (key == KEY_RESIZE) {
            getmaxyx(stdscr, fld->down_right.y, fld->down_right.x);
            graphic_show_lose_src(fld, meters, sec);
            break;
        }
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