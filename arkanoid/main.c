#include <stdio.h> /* print error messages */
#include "default_structs.h"
#include "arkanoid.h"
#include "tui.h"

enum sides { LEFT = -1, RIGHT = +1 };

static void move_paddle(paddle *pad, const rectangle *cup, enum sides side)
{
    draw_rect(paddle_get_ptr_rect(pad), CHR_EMPTY);
    paddle_move(pad, cup, side);
    draw_rect(paddle_get_ptr_rect(pad), CHR_PADDLE);
}

int main(void)
{
    point game_size;
    rectangle cup;
    block *blocks;
    paddle *pad;
    ball *pill;
    enum delays delay;
    enum api_keys key;

    if(!init_game(&game_size, &cup, &delay)) {
        terminate_game();
        fprintf(stderr, "Size must be greater then %d\n", MIN_TERM_SIZE);
        return 1;
    }
    if (!objects_init(&pad, &pill, &blocks, &cup)) {
        terminate_game();
        fputs("RAM is too small. Clear you RAM\n", stderr);
        return 2;
    }
    draw_contour(&cup, CHR_BOUNDS);
    draw_rect(paddle_get_ptr_rect(pad), CHR_PADDLE);
    draw_rect(ball_get_ptr_rect(pill), CHR_BALL);

    while ((key = get_key()) != quit) {
        switch (key) {
        case to_left:
            move_paddle(pad, &cup, LEFT);
            break;
        case to_right:
            move_paddle(pad, &cup, RIGHT);
            break;
        case pause:
            set_pause_until_not_pressed(delay);
            break;
        }

        draw_rect(ball_get_ptr_rect(pill), CHR_EMPTY);
        if (!ball_move(pill, pad, blocks, &cup))
            break;
        draw_rect(ball_get_ptr_rect(pill), CHR_BALL);

    }

    terminate_game();
    objects_erase(pad, pill, blocks);
    return 0;
}