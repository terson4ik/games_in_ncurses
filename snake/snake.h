#ifndef SNAKE_SENTRY_HEADER
#define SNAKE_SENTRY_HEADER

#include "common_types.h"

enum sides { UP = 0, DOWN, LEFT, RIGHT };

typedef struct snake_tag snake;
typedef struct segment_of_snake_tag segment_snake;

int snake_init(snake **s, const point *game_field);
int snake_move(snake *s);
int snake_change_side(snake *s, enum sides new_side);
void snake_destroy(snake **s);
int handle_resize(snake *s, const point *game_field);
point *get_head_point(const snake *s);
point *get_tail_point(const snake *s);
segment_snake *get_head_segm(const snake *s);
segment_snake *get_after_head_segm(const snake *s);
int snake_get_size(const snake *s);
 /* use also to spawn apple */
int snake_check_hit(const point *head, const segment_snake *next);

int snake_check_bounds(const snake *s, const point *game_field) ;
int snake_lengthen(snake *s, const point *game_field);
void snake_spawn_apple(const snake *s, point *apple, const point *game_field);

int snake_is_win(const snake *s, const point *game_field);

#endif
