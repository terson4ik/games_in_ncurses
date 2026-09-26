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

int snake_init(snake **s, const rectangle *fld)
{
    segment_snake *head;
    *s = malloc(sizeof(**s));
    head = malloc(sizeof(*head));
    if (!*s || !head)
        return 0;

    (*s)->side = (rand() + 1) % 4;
    (*s)->length = 1;
    head->cur_p.x = fld->down_right.x / 2;
    head->cur_p.y = fld->down_right.y / 2;
    head->next = head->prev = (*s)->head = (*s)->tail = head;
    return 1;
}

void snake_move(snake *s) 
{
    s->tail->cur_p = s->head->cur_p;
    s->head = s->tail;
    s->tail = s->head->prev;

    switch(s->side) {
    case UP:    s->head->cur_p.y--; break;
    case DOWN:  s->head->cur_p.y++; break;
    case LEFT:  s->head->cur_p.x--; break;
    case RIGHT: s->head->cur_p.x++; break;
    }
}

int snake_handle_resize(snake *s, const rectangle *fld)
{
    return snake_check_hit(&fld->down_right, s->head);
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

void snake_spawn_apple(const snake *s, point *app, const rectangle *fld)
{
    enum { lft_x = 1, up_y = 1 };
    const int rgt_x = fld->down_right.x-1;
    const int dwn_y = fld->down_right.y-1;

    do {
        app->x = (rand() % rgt_x) + lft_x;
        app->y = (rand() % dwn_y) + up_y;
    } while (snake_check_hit(app, s->head->next) ||
             (app->x == s->head->cur_p.x && app->y == s->head->cur_p.y));
}

int snake_check_bounds(const snake *h, const rectangle *fld) 
{
    return h->head->cur_p.x < 1 || h->head->cur_p.x >= fld->down_right.x ||
           h->head->cur_p.y < 1 || h->head->cur_p.y >= fld->down_right.y;
}

void snake_change_side(snake *s, enum sides new_side)
{
    switch (new_side) {
    case UP:    if (s->side != DOWN)  s->side = UP;    break;
    case DOWN:  if (s->side != UP)    s->side = DOWN;  break;
    case LEFT:  if (s->side != RIGHT) s->side = LEFT;  break;
    case RIGHT: if (s->side != LEFT)  s->side = RIGHT; break;
    }
}

void snake_lengthen(snake *s)
{
    segment_snake *new_segm = malloc(sizeof(*new_segm));
    if (!new_segm)
        return;

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
    }

    s->length++;
    new_segm->next = s->head;
    new_segm->prev = s->tail;
    s->tail->next = new_segm;
    s->tail = new_segm;
    s->head->prev = s->tail;
}

int snake_is_win(const snake *s, const rectangle *fld)
{
    /* 2 is size of border in eash borders */
    return s->length >= (fld->down_right.x-1) * (fld->down_right.y-1) - 1;
}

const point *get_head_point(const snake *s)
{
    return &s->head->cur_p;
}

const point *get_tail_point(const snake *s)
{
    return &s->tail->cur_p;
}

const point *get_segment_point(const segment_snake *segm)
{
    return &segm->cur_p;
}

const segment_snake *get_head_segm(const snake *s)
{
    return s->head;
}

const segment_snake *get_next_segm(const segment_snake *segm)
{
    return segm->next;
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
