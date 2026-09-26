/* snake: support modern and legacy terminals */
#include <stdio.h> /* prints stats */

#include <stdlib.h> /* inits rand() */
#include <time.h>   /* ------------ */

#include "common_types.h"
#include "snake.h"
#include "tui.h"

static int  start_game(rectangle *field, snake **head, int *delay, int size);
static void make_snake(snake *head, int size);
static void spawn_apple(point *apple, const snake *head, const rectangle *fld);
static void handle_resize(rectangle *fld, snake *head, point *apple, int delay);
static void destroy_game(snake *head);

int main(void)
{
    snake *head; /* AUTO_INIT, NULL not needed */
    rectangle field;
    point apple;
    enum keys key;
    int delay;

    start_game(&field, &head, &delay, INIT_SIZE);

    make_snake(head, INIT_SIZE);
    spawn_apple(&apple, head, &field);
    update_frame();

        /* get_key() contains sleep() */
    while ((key = get_key()) != quit) {
        point *head_p;
        switch (key) {
        case up:     snake_change_side(head, UP);    break;
        case down:   snake_change_side(head, DOWN);  break;
        case left:   snake_change_side(head, LEFT);  break;
        case right:  snake_change_side(head, RIGHT); break;
        case pause:  graphics_pause(delay);          break;
        case resize: handle_resize(&field, head, &apple, delay); break;
        case quit:   break;
        case skip:   break;
        }

        draw_char(get_head_point(head), CHR_SN_BODY, BODY_PAIR);
        draw_char(get_tail_point(head), CHR_EMPTY, BG_PAIR);

        snake_move(head);
        head_p = (point *)get_head_point(head);
        draw_char(head_p, CHR_SN_HEAD, HEAD_PAIR);

        if (apple.x == head_p->x && apple.y == head_p->y) {
            snake_lengthen(head);
            draw_char(get_head_point(head), CHR_SN_BODY, BODY_PAIR);
            spawn_apple(&apple, head, &field);
            update_stats(snake_get_size(head));
        } else 
        if (snake_check_hit(head_p, get_next_segm(get_head_segm(head))) ||
            snake_check_bounds(head, &field))
        {
            end_game(&field, snake_get_size(head), 0);
            break;
        } else
        if (snake_is_win(head, &field)) {
            end_game(&field, snake_get_size(head), 1);
            break;
        }

        update_frame();
    }

    destroy_game(head);
    return 0;
}

static int start_game(rectangle *field, snake **head, int *delay, int size)
{
    int i;
    srand(time(NULL));
    graphics_init(field, delay, size);
    if (!snake_init(head, field))
        return 0;
        
    for (i = size; i > 0; i--)
        snake_lengthen(*head);

    return 1;
}

static void spawn_apple(point *apple, const snake *head, const rectangle *fld)
{
    snake_spawn_apple(head, apple, fld);
    draw_char(apple, CHR_APPLE, APPLE_PAIR);
}

static void make_snake(snake *head, int size)
{
    int i;
    segment_snake *cur_segm = (segment_snake *) get_head_segm(head);
    draw_char(get_segment_point(cur_segm), CHR_SN_HEAD, HEAD_PAIR);

    for (i = size - 1; i > 0; i--) {
        draw_char(get_segment_point(cur_segm), CHR_SN_BODY, BODY_PAIR);
        cur_segm = (segment_snake *)get_next_segm(cur_segm);
    }
}

static void handle_resize(rectangle *fld, snake *head, point *apple, int delay)
{
    do {
        graphics_handle_resize(fld, delay);
        update_frame();
    } while (snake_check_bounds(head, fld));
    
    rebuild_game(fld, snake_get_size(head));
    spawn_apple(apple, head, fld);
    make_snake(head, snake_get_size(head));
    update_frame();
}

static void destroy_game(snake *head)
{
    graphics_end();
    snake_destroy(&head);
}