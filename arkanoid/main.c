#include <stdio.h> /* print error messages */
#include "default_structs.h"
#include "arkanoid.h"
#include "tui.h"

enum sides { LEFT = -1, RIGHT = +1 };
enum win_state { WIN = 1, LOSE = -1, UNKOWN = 0 };

static void move_paddle(paddle *pad, const rectangle *cup, enum sides side)
{
    draw_rect(paddle_get_ptr_rect(pad), CHR_EMPTY);
    paddle_move(pad, cup, side);
    draw_rect(paddle_get_ptr_rect(pad), CHR_PADDLE);
}
static void spawn_blocks(block *blocks)
{
    int col, row;
    for (row = 0; row < BLOCK_ROWS; row++)
        for (col = 0; col < BLOCK_COLS; col++)
            draw_rect(blocks_get_ptr_rect(blocks, row, col), CHR_BLOCK);
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
    draw_contour(&cup, CHR_BOUNDS);
    draw_rect(paddle_get_ptr_rect(pad), CHR_PADDLE);
    draw_rect(ball_get_ptr_rect(pill), CHR_BALL);
    spawn_blocks(blocks);

    is_win = UNKOWN;
    while ((key = get_key()) != quit && is_win == UNKOWN) {
        switch (key) {
        case to_left:
            move_paddle(pad, &cup, LEFT);
            break;
        case to_right:
            move_paddle(pad, &cup, RIGHT);
            break;
        case game_pause:
            set_pause_until_not_pressed();
            break;
        }
        input_flush();
        sleep_frame(delay);

        draw_rect(ball_get_ptr_rect(pill), CHR_EMPTY);
        switch (ball_move(pill, pad, blocks, &cup, &callback_rect_block)) {
        case hit:
            draw_rect(callback_rect_block, CHR_EMPTY);
            if (block_all_destroyed(blocks))
                is_win = WIN;
            break;
        case lose:
            is_win = LOSE;
            break;
        case nothing:
        }

        draw_rect(ball_get_ptr_rect(pill), CHR_BALL);
    }

    terminate_game();
    objects_erase(pad, pill, blocks);
    return 0;
}