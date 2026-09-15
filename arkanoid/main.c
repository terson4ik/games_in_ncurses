#include <stdio.h> /* print error messages */
#include <stdlib.h>
#include "default_structs.h"
#include "arkanoid.h"
#include "tui.h"

enum sides { LEFT = -1, RIGHT = +1 };


static void move_paddle(paddle *pad, const rectangle *cup, enum sides side);
static void spawn_blocks(block *blocks);
static int safety_resize(paddle *pad, ball *ba, block *blocks,
                                point *field, rectangle *cup);

#define SAFE_RESIZE_MACRO() \
    if (!safety_resize(pad, pill, blocks, &game_size, &cup)){ \
        objects_erase(pad, pill, blocks); \
        terminate_game(); \
        fprintf(stderr, "You broke this game. min size %d\n", MIN_TERM_SIZE); \
        return 3; \
    }
int main(void)
{
    point game_size;
    rectangle cup, *callback_rect_block;
    block *blocks;
    paddle *pad;
    ball *pill;
    enum delays delay;
    enum api_keys key;
    enum win_state is_win;
    unsigned int score = 0;

    if(!init_game(&game_size, &cup, &delay)) {
        terminate_game();
        fprintf(stderr, "Size must be greater then %d\n", MIN_TERM_SIZE);
        return 1;
    }
    if (!objects_init(&pad, &pill, &blocks, &cup)) {
        objects_erase(pad, pill, blocks);
        terminate_game();
        fputs("RAM is too small. Clear you RAM\n", stderr);
        return 2;
    }
    SAFE_RESIZE_MACRO();

    is_win = UNKOWN;
    update_stats(score);
    while ((key = get_key()) != quit) {
        switch (key) {
        case to_left:    move_paddle(pad, &cup, LEFT);  break;
        case to_right:   move_paddle(pad, &cup, RIGHT); break;
        case game_pause:
            input_flush();
            set_pause_until_not_pressed();
            break;
        case quit:       break;
        case resize: 
            while ((key = get_key()) == resize)
                ; /* get final size and call resize */
            SAFE_RESIZE_MACRO();
        case no_key: break;
        }

        input_flush();
        sleep_frame(delay);

        draw_rect(ball_get_ptr_rect(pill), CHR_EMPTY, BG_PAIR);
        switch (ball_move(pill, pad, blocks, &cup, &callback_rect_block)) {
        case hit:
            draw_rect(callback_rect_block, CHR_EMPTY, BG_PAIR);
            if (block_all_destroyed(blocks)) {
                is_win = WIN;
                end_game(is_win, &game_size, score);
            }
            score += 16;
            update_stats(score);
            break;
        case lose:
            is_win = LOSE;
            end_game(is_win, &game_size, score);
            break;
        case nothing: break;
        }
        if (is_win == UNKOWN)
            draw_rect(ball_get_ptr_rect(pill), CHR_BALL, BALL_PAIR);
        else
            break;
    }

    terminate_game();
    objects_erase(pad, pill, blocks);
    return 0;
}

static void move_paddle(paddle *pad, const rectangle *cup, enum sides side)
{
    draw_rect(paddle_get_ptr_rect(pad), CHR_EMPTY, BG_PAIR);
    paddle_move(pad, cup, side);
    draw_rect(paddle_get_ptr_rect(pad), CHR_PADDLE, PADDLE_PAIR);
}

static void spawn_blocks(block *blocks)
{
    int col, row, bg;
    for (row = 0; row < BLOCK_ROWS; row++)
        for (col = 0; col < BLOCK_COLS; col++)
            if (block_is_live(blocks, row, col)) {
                bg = BLOCK_PAIR_1 + (rand() % BLOK_PAIRS_COUNT);
                draw_rect(blocks_get_ptr_rect(blocks, row, col), CHR_BLOCK, bg);
            }
}

static int safety_resize(paddle *pad, ball *ba, block *blocks,
                                point *field, rectangle *cup)
{
    if (!handle_resize(field, cup))
        return 0;
    rebuild_entries(pad, ba, blocks, cup);
    draw_contour(cup, CHR_BOUNDS, BORDER_PAIR);
    draw_bg(cup, BG_PAIR);
    draw_rect(paddle_get_ptr_rect(pad), CHR_PADDLE, PADDLE_PAIR);
    draw_rect(ball_get_ptr_rect(ba), CHR_BALL, BALL_PAIR);
    spawn_blocks(blocks);

    return 1;
}