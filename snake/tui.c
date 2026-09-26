#include <stdio.h> /* new records */
#include <curses.h>
#include "tui.h" /* common types also included here */

#ifdef KEY_ENTER
#  undef  KEY_ENTER
#  define KEY_ENTER '\n'
#else
#  define KEY_ENTER '\n'
#endif

#define KEY_ESCAPE 27
#define KEY_SPACE  ' '

#define DELAY_TIME_EASY 130
#define DELAY_TIME_NORM 50
#define DELAY_TIME_HARD 30

enum levels { easy = 0, normal, hard };

static enum levels menu(const rectangle *fld);
static void init_my_pairs(void);
#define NEW_RECORD 0


void graphics_init(rectangle *fld, int *delay, int size)
{
    initscr();
    start_color();
    if (has_colors())
        init_my_pairs();
    cbreak();
    noecho();
    curs_set(0);
    keypad(stdscr, 1);
    
    fld->up_left.x = 0;
    fld->up_left.y = 0;
    graphics_handle_resize(fld, 1); /* 1 is wrapper */

    switch (menu(fld)) {
    case easy:   *delay = DELAY_TIME_EASY; break;
    default:
    case normal: *delay = DELAY_TIME_NORM; break;
    case hard:   *delay = DELAY_TIME_HARD; break;
    }
    timeout(*delay);
    rebuild_game(fld, size+1); /* + head */
}

void graphics_handle_resize(rectangle *fld, int delay)
{
    int row, col;
    attrset(COLOR_PAIR(COMMON_PAIR));
    clear();
    mvaddstr(0, 0, "SMALL TERM!");
    while (1) { /* infinity loop, break places down */
        getmaxyx(stdscr, row, col);
        if (row < MIN_SIZE_WINDOW || col < MIN_SIZE_WINDOW) {
            timeout(-1);
            clear();
            mvaddstr(0, 0, "SMALL TERM!");
            while (getch() != KEY_RESIZE) {
            }
        } else {
           break;
        }
    }
    timeout(delay);
    
    fld->down_right.x = col-1; /* 1 is border in right side */
    fld->down_right.y = row-1;
}

void graphics_pause(int delay)
{
    timeout(-1);
    getch();
    timeout(delay);
}

void print_text(const char *str)
{
    mvaddstr(0, 0, str);
}

void graphics_end(void)
{
    endwin();
}

enum keys get_key(void)
{
    switch (getch()) {
        case 'w':
        case 'W':
        case KEY_UP:    return up;
        case 's':
        case 'S':
        case KEY_DOWN:  return down;
        case 'a':
        case 'A':
        case KEY_LEFT:  return left;
        case 'd':
        case 'D':
        case KEY_RIGHT: return right;

        case KEY_ENTER:
        case KEY_SPACE:
        case 'p':
        case 'P':        return pause;
        case KEY_RESIZE: return resize;
        default:         return skip;
    }
}

static void init_my_pairs()
{
    init_pair(BORDER_PAIR, COLOR_BLACK,   COLOR_RED);
    init_pair(BG_PAIR,     COLOR_CYAN,    COLOR_CYAN);
    init_pair(HEAD_PAIR,   COLOR_MAGENTA, COLOR_MAGENTA);
    init_pair(BODY_PAIR,   COLOR_GREEN,   COLOR_GREEN);
    init_pair(APPLE_PAIR,  COLOR_RED,     COLOR_RED);
    init_pair(WIN_PAIR,    COLOR_BLACK,   COLOR_GREEN);
    init_pair(LOSE_PAIR,   COLOR_BLACK,   COLOR_RED);
}

void draw_char(const point *p, char c, int pair)
{
    if (has_colors())
        attrset(COLOR_PAIR(pair));
    else
        attrset(COLOR_PAIR(COMMON_PAIR));
        
    mvaddch(p->y, p->x, c);
}

void draw_rectangle(const rectangle *r, char c, int pair)
{
    int x, y;

    if (has_colors())
        attrset(COLOR_PAIR(pair));
    else
        attrset(COLOR_PAIR(COMMON_PAIR));

    for (y = r->up_left.y; y < r->down_right.y; y++)
        for (x = r->up_left.x; x < r->down_right.x; x++)
            mvaddch(y, x, c);
}

void draw_borders(const rectangle *r, char vert, char horiz, int pair)
{
    int x, y;

    if (has_colors())
        attrset(COLOR_PAIR(pair));
    else
        attrset(A_REVERSE);

    for (y = r->up_left.y; y <= r->down_right.y; y++) {
        mvaddch(y, r->down_right.x, vert);
        mvaddch(y, r->up_left.x,    vert);
    }

    for (x = r->up_left.x; x <= r->down_right.x; x++) {
        mvaddch(r->down_right.y, x, horiz);
        mvaddch(r->up_left.y, x,    horiz);
    }
}

void rebuild_game(const rectangle *fld, int size)
{
    rectangle game_rect;
    clear();

    draw_borders(fld, CHR_VERTICAL, CHR_HORIZONTAL, BORDER_PAIR);
    
    game_rect = *fld;
    game_rect.up_left.x++;
    game_rect.up_left.y++;


    draw_rectangle(&game_rect, CHR_EMPTY, BG_PAIR);

    update_stats(size);
}

static void write_new_rec(int new_rec)
{
    FILE *fp = fopen(RECORD_FILE_NAME, "w");
    if (!fp)
        return;
    fprintf(fp, "%u", new_rec);
    fclose(fp);
}

/* 0 is new record */
static int check_record(int cur_size)
{
    int score;
    FILE *records = fopen(RECORD_FILE_NAME, "r");
    if (records) {
        fscanf(records, "%d", &score);
        fclose(records);
        if (cur_size > score) {
            write_new_rec(cur_size);
            return NEW_RECORD;
        } else {
            return score;
        }
    } else { /* not exist -- automaticaly new rec */
        write_new_rec(cur_size);
        return NEW_RECORD;
    }
}

void end_game(rectangle *fld, int size, int is_win)
{
    const int medianX = fld->down_right.x / 2 - 7; /* 7 centring X */
    const int medianY = fld->down_right.y / 2;
    int key, shift, high_score;

    clear();
    if(is_win) {
        if (has_colors())
            draw_rectangle(fld, CHR_EMPTY, WIN_PAIR);

        attrset(A_REVERSE);
        mvaddstr(medianY, medianX, "WTF!?");
        mvaddstr(medianY+1, medianX, "WOW!!!");
        mvaddstr(medianY+2, medianX, "YOU WIN!!!");
        shift = 3;
    } else {
        if (has_colors())
            draw_rectangle(fld, CHR_EMPTY, LOSE_PAIR);
        attrset(COLOR_PAIR(LOSE_PAIR));
        mvaddstr(medianY, medianX, "YOU DIE :(");
        mvaddstr(medianY+1, medianX, "BUT!!!");

        high_score = check_record(size);
        if (high_score == NEW_RECORD) {
            shift = 3;
            attrset(A_BOLD | A_UNDERLINE);
            mvprintw(medianY+2, medianX, "HOLY SHIT! IT'S NEW RECORD: %dcm!", size);
        } else {
            shift = 4;
            mvprintw(medianY+2, medianX, "YOUR LENGTH BEFORE DIE %dcm;", size);
            mvprintw(medianY+3, medianX, "OLD LARGEST SNAKE HAVE %dcm;", high_score);
        }
    }
    timeout(-1);
    attrset(A_BLINK | A_BOLD | A_UNDERLINE);
    mvaddstr(medianY+shift, medianX, "PRESS ENTER TO CONTINUE...");
    refresh();
    while ((key = getch()) != KEY_ENTER)
        if (key == KEY_RESIZE) {
            getmaxyx(stdscr, fld->down_right.y, fld->down_right.x);
            end_game(fld, size, is_win);
            break;
        }
}

static enum levels menu(const rectangle *fld)
{
    enum levels lvl = normal;
    const int medianX = fld->down_right.x / 2 - 4;
    const int medianY = fld->down_right.y / 2;
    int key;

    clear();
    attrset(A_UNDERLINE | A_BLINK | A_DIM);

    mvaddstr(medianY-1, medianX, "CHOOSE LEVEL(TYPE ENTER)");
    attrset(A_BOLD);
    mvaddstr(medianY,   medianX, "EASY :)))");
    mvaddstr(medianY+1, medianX, "NORMAL");
    mvaddstr(medianY+2, medianX, "HARD :(");
    attron(A_REVERSE);
    mvaddstr(medianY+1, medianX, "NORMAL");
    refresh();

    timeout(-1);
    while ((key = getch()) != KEY_ENTER) {
        attrset(A_BOLD);
        switch (key) {
        case KEY_UP:
            if (lvl != easy) {
                mvaddstr(medianY+lvl, medianX, (lvl == hard) ? "HARD :(" : "NORMAL");                
                lvl--;
                attron(A_REVERSE);
                mvaddstr(medianY+lvl, medianX, (lvl == easy) ? "EASY :)))" : "NORMAL");    
            }
            break;
        case KEY_DOWN:
            if (lvl != hard) {
                mvaddstr(medianY+lvl, medianX, (lvl == easy) ? "EASY :)))" : "NORMAL");         
                lvl++;
                attron(A_REVERSE);
                mvaddstr(medianY+lvl, medianX, (lvl == hard) ? "HARD :(" : "NORMAL");     
            }
            break;
        case KEY_RESIZE:
            return menu(fld);
        }
        refresh();
    }
    return lvl;
}

void update_stats(int size)
{
    attrset(A_REVERSE);
    mvprintw(0, 0, "length:%-5d", size);
}

void update_frame(void)
{
    move(0, 0);
    refresh();
}