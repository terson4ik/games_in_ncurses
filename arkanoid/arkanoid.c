#include <stdlib.h> /* malloc & free; use rand() in init ball */
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
void rebuild_entries(paddle *pad, ball *ba, block *blocks,
                     const rectangle *cup)
{
    int col, row;
    pad->cur_r.up_left.x = ((cup->up_left.x + cup->down_right.x) / 2) - 1;
    pad->cur_r.down_right.x = pad->cur_r.up_left.x + PADDLE_LEN;
    pad->cur_r.up_left.y = pad->cur_r.down_right.y = cup->down_right.y - 1;
    
    ba->pos.x = pad->cur_r.up_left.x + 2;
    ba->pos.y = cup->down_right.y / 2;
    ba->vector.x = (rand() % 2 == 0) ? LEFT : RIGHT;
    ba->vector.y = DOWN;
    for (row = 0; row < BLOCK_ROWS; row++)
    for (col = 0; col < BLOCK_COLS; col++) {
        int ind = (row * BLOCK_COLS + col);
        blocks[ind].rect.up_left.x =cup->up_left.x+(col*BLOCK_WIDTH)+1;
        blocks[ind].rect.up_left.y = cup->up_left.y+(row*BLOCK_HEIGHT);
        blocks[ind].rect.down_right.x = blocks[ind].rect.up_left.x + BLOCK_WIDTH-1;
        blocks[ind].rect.down_right.y = blocks[ind].rect.up_left.y + BLOCK_HEIGHT-1;            
    }
}

int objects_init(paddle **p_paddle, ball **p_ball, block **p_blocks,
                 const rectangle *cup)
{
    int row, col;

    *p_paddle = malloc(sizeof(paddle));
    *p_ball   = malloc(sizeof(ball));
    *p_blocks = malloc(sizeof(block) * BLOCK_COLS * BLOCK_ROWS);
    if (!*p_ball || !*p_paddle || !*p_blocks)
        return 0;
    rebuild_entries(*p_paddle, *p_ball, *p_blocks, cup);

    for (row = 0; row < BLOCK_ROWS; row++)
        for (col = 0; col < BLOCK_COLS; col++)
            (*p_blocks)[row * BLOCK_COLS + col].is_die = 0;
    return 1;
}

const rectangle *paddle_get_ptr_rect(const paddle *p)
{
    return &(p->cur_r);
}

const rectangle *ball_get_ptr_rect(const ball *b)
{
    static rectangle pseudo_rectangle_entry; /* wrapper for one pixel objects*/

    pseudo_rectangle_entry.down_right.x
    = pseudo_rectangle_entry.up_left.x = b->pos.x;

    pseudo_rectangle_entry.down_right.y
    = pseudo_rectangle_entry.up_left.y = b->pos.y;

    return &pseudo_rectangle_entry;
}

const rectangle *blocks_get_ptr_rect(const block *blocks, int row, int col)
{
    return &blocks[row * BLOCK_COLS + col].rect;
}

void paddle_move(paddle *pad, const rectangle *cup, int dx)
{
    if (pad->cur_r.up_left.x + dx > cup->up_left.x
        &&  pad->cur_r.down_right.x + dx < cup->down_right.x) {
        pad->cur_r.up_left.x += dx;
        pad->cur_r.down_right.x += dx;
    }
}

static rectangle *block_check_hit(block *blks, const ball *b)
{
    int col, row;
    for (row = 0; row < BLOCK_ROWS; row++)
        for (col = 0; col < BLOCK_COLS; col++) {
            int ind = (row * BLOCK_COLS + col);
            if (blks[ind].is_die == 0
              && b->pos.x >= blks[ind].rect.up_left.x
              && b->pos.x <= blks[ind].rect.down_right.x
              && b->pos.y >= blks[ind].rect.up_left.y
              && b->pos.y <= blks[ind].rect.down_right.y) {
                blks[ind].is_die = 1;
                return &blks[ind].rect;
              }
        }
    return NULL;
}

static int ball_is_touch_wall(const ball *b, const rectangle *cup)
{
    return b->pos.x + b->vector.x <= cup->up_left.x
        || b->pos.x + b->vector.x >= cup->down_right.x;
}

enum ball_act
ball_move(ball *b, const paddle *p, block *blks, const rectangle *cup, 
          rectangle **callback_rect_block)
{
    /* touch walls? */
    if (ball_is_touch_wall(b, cup))
        b->vector.x *= -1;

    /* touch floor? */
    if (b->pos.y + b->vector.y < 0)
        b->vector.y = DOWN;

    /* touche paddle? */
    if (b->pos.y + b->vector.y >= cup->down_right.y - 1)
        if (b->pos.x >= p->cur_r.up_left.x
        && b->pos.x <= p->cur_r.down_right.x) {
            b->vector.y = UP;
            b->vector.x = (rand() % 2 == 0) ? LEFT : RIGHT;
            if (ball_is_touch_wall(b, cup))
                b->vector.x *= -1;
        }

    /* touch lava? */
    if (b->pos.y + b->vector.y >= cup->down_right.y)
        return lose;
            

    if ((*callback_rect_block = block_check_hit(blks, b))) {
        b->vector.y *= -1;
        return hit;
    }

    b->pos.x += b->vector.x;
    b->pos.y += b->vector.y;

    return nothing;
}

int block_all_destroyed(block *blks)
{
    int col, row;
    for (row = 0; row < BLOCK_ROWS; row++)
        for (col = 0; col < BLOCK_COLS; col++)
            if(block_is_live(blks, row, col))
                return 0;
    return 1;
}

int block_is_live(const block *blocks, int row, int col)
{
    return blocks[row * BLOCK_COLS + col].is_die == 0;
}

void objects_erase(paddle *p_paddle, ball *p_ball, block *p_blocks)
{
    free(p_ball);
    free(p_paddle);
    free(p_blocks);
}