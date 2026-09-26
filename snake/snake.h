#ifndef SNAKE_SENTRY_HEADER
#define SNAKE_SENTRY_HEADER

#include "common_types.h"

#define INIT_SIZE 3 /* body */

enum sides { UP = 0, DOWN, LEFT, RIGHT };

typedef struct snake_tag snake;
typedef struct segment_of_snake_tag segment_snake;

int snake_init(snake **s, const rectangle *field);
void snake_move(snake *s);
void snake_change_side(snake *s, enum sides new_side);
void snake_destroy(snake **s);
int snake_handle_resize(snake *s, const rectangle *field);
const point *get_head_point(const snake *s);
const point *get_tail_point(const snake *s);
const point *get_segment_point(const segment_snake *segm);
const segment_snake *get_head_segm(const snake *s);
const segment_snake *get_next_segm(const segment_snake *segm);

int snake_get_size(const snake *s);
 /* use also to spawn apple */
int snake_check_hit(const point *head, const segment_snake *next);

int snake_check_bounds(const snake *s, const rectangle *field) ;
void snake_lengthen(snake *s);
void snake_spawn_apple(const snake *s, point *apple, const rectangle *field);

int snake_is_win(const snake *s, const rectangle *field);

#endif
