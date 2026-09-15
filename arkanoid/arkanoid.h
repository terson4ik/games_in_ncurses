#ifndef ARKANOID_H_SENTRY
#define ARKANOID_H_SENTRY

#include "default_structs.h"
#include "arkanoid_conf.h"

typedef struct paddle_tab paddle;
typedef struct ball_tag ball;
typedef struct block_tag block;

void objects_init(paddle **p_paddle, ball **p_ball, block **p_blocks,
                                            rectangle *cup);
void objects_erase(paddle *p_paddle, ball *p_ball, block *p_blocks);

rectangle *paddle_get_ptr_rect(paddle *p);

#endif
