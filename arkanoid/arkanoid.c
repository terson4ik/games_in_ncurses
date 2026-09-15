#include <stdlib.h> /* malloc & free */
#include <time.h> /* use in init ball */
#include "arkanoid.h"

struct paddle_tab {
    rectangle cur_r;
};

struct ball_tag {
    rectangle cur_r, vector;
};

struct block_tag {
    rectangle rect;
    char is_die;
};
/* typedefs in header file */

int objects_init(paddle **p_paddle, ball **p_ball, block **p_blocks,
                                                rectangle *cup)
{
    int row, col;

    *p_paddle = malloc(sizeof(paddle));
    *p_ball   = malloc(sizeof(ball));
    *p_blocks = malloc(sizeof(block) * BLOCK_COLS * BLOCK_ROWS);
    if (!*p_ball || !*p_paddle || !*p_blocks)
        return 0;

    (*p_paddle)->cur_r.up_left.x = ((cup->up_left.x + cup->down_right.x) / 2) - 1;
    (*p_paddle)->cur_r.down_right.x = (*p_paddle)->cur_r.up_left.x + PADDLE_LEN;
    (*p_paddle)->cur_r.up_left.y = (*p_paddle)->cur_r.down_right.y = cup->down_right.y - 1;
    
    (*p_ball)->cur_r.up_left.x = (*p_paddle)->cur_r.up_left.x + 2;
    (*p_ball)->cur_r.down_right.x = (*p_ball)->cur_r.up_left.x;
    (*p_ball)->cur_r.up_left.y = cup->down_right.y / 2;
    (*p_ball)->cur_r.down_right.y = (*p_ball)->cur_r.up_left.y;

    return 1;
}

rectangle *paddle_get_ptr_rect(paddle *p)
{
    return &(p->cur_r);
}

rectangle *ball_get_ptr_rect(ball *b)
{
    return &(b->cur_r);
}

void paddle_move(paddle *pad, const rectangle *cup, int dx)
{
    if (pad->cur_r.up_left.x + dx > cup->up_left.x
        &&  pad->cur_r.down_right.x + dx < cup->down_right.x) {
        pad->cur_r.up_left.x += dx;
        pad->cur_r.down_right.x += dx;
    }
}

void objects_erase(paddle *p_paddle, ball *p_ball, block *p_blocks)
{
    free(p_ball);
    free(p_paddle);
    free(p_blocks);
}