#include <stdio.h>
#include <stdlib.h>
#include "snake.h"

struct segment_of_snake_tag {
    point cur_p;
    struct segment_of_snake_tag *prev, *next;
};

struct snake_tag {
    segment_snake *head, *tail;
    unsigned int length;
    enum sides side;
};
 /* typedef defined in header file */

int snake_init(snake **s, point *game_field)
{
    segment_snake *head;
    *s = malloc(sizeof(**s));
    head = malloc(sizeof(*head));
    if (!*s || !head)
        return ERROR;

    (*s)->side = (rand() + 1) % 4;
    (*s)->length = 1;
    head->cur_p.x = game_field->x / 2;
    head->cur_p.y = game_field->y / 2;
    head->next = head->prev = (*s)->head = (*s)->tail = head;
    return 0;
}

int snake_move(snake *s) 
{
    s->tail->cur_p = s->head->cur_p;
    s->head = s->tail;
    s->tail = s->head->prev;

    switch(s->side) {
    case UP:    s->head->cur_p.y--; break;
    case DOWN:  s->head->cur_p.y++; break;
    case LEFT:  s->head->cur_p.x--; break;
    case RIGHT: s->head->cur_p.x++; break;
    default:
        return ERROR;
    }
    return 0;
}

int handle_resize(snake *s, point *game_field)
{
    if (snake_check_hit(game_field, s->head))
        return ERROR;
    else
        return 0;
}

int  snake_check_hit(const point *head, const segment_snake *next) 
{
    const segment_snake *anchor = next->prev; /* TRUE head */
    while (next != anchor) {
        if (head->x == next->cur_p.x && head->y == next->cur_p.y)
            return 1;
        next = next->next;
    } 
    return 0;
}

void snake_spawn_apple(const snake *s, point *app, point *game_field) 
{
    do {
        app->x = (rand() % (game_field->x-2)) + 1;
        app->y = (rand() % (game_field->y-2)) + 1;
    } while (snake_check_hit(app, s->head->next));
}

int snake_check_bounds(const snake *h, point *game_field) 
{
    return h->head->cur_p.x < 1 || h->head->cur_p.x >= game_field->x-1 ||
           h->head->cur_p.y < 1 || h->head->cur_p.y >= game_field->y-1;
}

int snake_change_side(snake *s, enum sides new_side)
{
    switch (new_side) {
    case UP:    if (s->side != DOWN)  s->side = UP;    break;
    case DOWN:  if (s->side != UP)    s->side = DOWN;  break;
    case LEFT:  if (s->side != RIGHT) s->side = LEFT;  break;
    case RIGHT: if (s->side != LEFT)  s->side = RIGHT; break;
    default:
        return ERROR;
    }
    return 0;
}

int snake_lengthen(snake *s, point *game_field)
{
    segment_snake *new_segm = malloc(sizeof(*new_segm));
    if (!new_segm)
        return ERROR;

    switch (s->side) {
    case UP:
        new_segm->cur_p.x = s->tail->cur_p.x;
        new_segm->cur_p.y = s->tail->cur_p.y + 1;
        break;
    case DOWN:
        new_segm->cur_p.x = s->tail->cur_p.x;
        new_segm->cur_p.y = s->tail->cur_p.y - 1;
        break;
    case LEFT:
        new_segm->cur_p.x = s->tail->cur_p.x + 1;
        new_segm->cur_p.y = s->tail->cur_p.y;
        break;
    case RIGHT:
        new_segm->cur_p.x = s->tail->cur_p.x - 1;
        new_segm->cur_p.y = s->tail->cur_p.y;
        break;
    default:
        return ERROR;
    }

    if (new_segm->cur_p.x < 0 || new_segm->cur_p.x >= game_field->x ||
        new_segm->cur_p.y < 0 || new_segm->cur_p.y >= game_field->y)
    {
        return ERROR;
    }
    
    s->length++;
    new_segm->next = s->head;
    new_segm->prev = s->tail;
    s->tail->next = new_segm;
    s->tail = new_segm;
    s->head->prev = s->tail;
    return 0;
}

int snake_is_win(const snake *s, point *game_field)
{
    return s->length >= (game_field->x-2) * (game_field->y-2);
}

point *get_head_point(const snake *s)
{
    return &s->head->cur_p;
}

point *get_tail_point(const snake *s)
{
    return &s->tail->cur_p;
}

segment_snake *get_head_segm(const snake *s)
{
    return s->head;
}

segment_snake *get_after_head_segm(const snake *s)
{
    return s->head->next;
}

int snake_get_size(const snake *s)
{
    return s->length;
}

void snake_destroy(snake **s) 
{
    (*s)->tail->next = NULL;
    while ((*s)->head != NULL) {
        segment_snake *cur = (*s)->head;
        (*s)->head = (*s)->head->next;
        free(cur);
    }
    free(*s);
    *s = NULL;
}
