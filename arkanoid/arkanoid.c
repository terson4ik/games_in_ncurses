#include <stdlib.h> /* malloc & free */
#include <time.h> /* use in init ball */
#include "arkanoid.h"

#define DOWN +1
#define UP   -1
#define LEFT -1
#define RIGHT 1

struct paddle_tab {
    rectangle cur_r;
};

struct ball_tag {
    point pos, vector;
};

struct block_tag {
    rectangle rect;
    char is_die;
};
/* typedefs in header file */

int objects_init(paddle **p_paddle, ball **p_ball, block **p_blocks,
                                            const rectangle *cup)
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
    
    (*p_ball)->pos.x = (*p_paddle)->cur_r.up_left.x + 2;
    (*p_ball)->pos.y = cup->down_right.y / 2;
    (*p_ball)->vector.x = (time(NULL) % 2 == 0) ? LEFT : RIGHT;
    (*p_ball)->vector.y = DOWN;

    return 1;
}

rectangle *paddle_get_ptr_rect(paddle *p)
{
    return &(p->cur_r);
}

rectangle *ball_get_ptr_rect(ball *b)
{
    static rectangle pseudo_rectangle_entry; /* wrapper for one pixel objects*/

    pseudo_rectangle_entry.down_right.x
    = pseudo_rectangle_entry.up_left.x = b->pos.x;

    pseudo_rectangle_entry.down_right.y
    = pseudo_rectangle_entry.up_left.y = b->pos.y;

    return &pseudo_rectangle_entry;
}

void paddle_move(paddle *pad, const rectangle *cup, int dx)
{
    if (pad->cur_r.up_left.x + dx > cup->up_left.x
        &&  pad->cur_r.down_right.x + dx < cup->down_right.x) {
        pad->cur_r.up_left.x += dx;
        pad->cur_r.down_right.x += dx;
    }
}

enum ball_act
ball_move(ball *b, const paddle *p, block *blks, const rectangle *cup)
{
    /* touch walls? */
    if (b->pos.x + b->vector.x <= cup->up_left.x
       ||  b->pos.x + b->vector.x >= cup->down_right.x)
        b->vector.x *= -1;

    /* touch floor? */
    if (b->pos.y + b->vector.y < 0)
        b->vector.y = DOWN;

    /* touch lave? */
    if (b->pos.y + b->vector.y >= cup->down_right.y)
        return 0;
        
    /* touche paddle? */
    if (b->pos.y + b->vector.y >= cup->down_right.y - 1)
        if (b->pos.x >= p->cur_r.up_left.x
        && b->pos.x <= p->cur_r.down_right.x)
            b->vector.y = UP;

    b->pos.x += b->vector.x;
    b->pos.y += b->vector.y;

    return 1;
}

void objects_erase(paddle *p_paddle, ball *p_ball, block *p_blocks)
{
    free(p_ball);
    free(p_paddle);
    free(p_blocks);
}