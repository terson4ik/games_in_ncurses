#ifndef ARKANOID_H_SENTRY
#define ARKANOID_H_SENTRY

#include "default_structs.h"
#include "arkanoid_conf.h"

typedef struct paddle_tab paddle;
typedef struct ball_tag ball;
typedef struct block_tag block;

enum ball_act { hit, lose, nothing };

int objects_init(paddle **p_paddle, ball **p_ball, block **p_blocks,
                                        const rectangle *cup);
void objects_erase(paddle *p_paddle, ball *p_ball, block *p_blocks);

void paddle_move(paddle *pad, const rectangle *cup, int dx);

enum ball_act
ball_move(ball *b, const paddle *p, block *blks, const rectangle *cup);

rectangle *paddle_get_ptr_rect(paddle *p);
rectangle *ball_get_ptr_rect(ball *b);
#endif
