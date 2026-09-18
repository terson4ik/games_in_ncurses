#include <ncurses.h>
#include "graphics_render.h"

/* if 0 then error, 1 all right */
int  graphic_init(rectangle *field, rectangle *way, useconds_t *delay)
{
    return 1;
}

enum key_vals get_key(void)
{
    return 1;
}

void graphic_flush(void)
{
}

void graphic_sleep(useconds_t delay)
{
}

void graphic_decrease_time(useconds_t *delay)
{
}

/* draw function work only in buffer; call update_frame() after any draw() */
void draw_rectangle(const rectangle *rect)
{
}

void draw_rect_frame(const rectangle *frame)
{
}

void draw_own_car(const point *up_left)
{
}

void draw_enemy_car(const point *up_left)
{
}

void draw_hide_car(const point *up_left)
{
}

/* init position must be 0 or 1 */
void draw_road(char position)
{
}

/* no refrech in draws(), then after any draw call this update_frame() */
void update_frame(void)
{
}

void graphic_pause(void)
{
}

void graphic_end(void)
{
}