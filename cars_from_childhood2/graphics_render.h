#ifndef GRAPHICS_RENDER_H_SENTRY
#define GRAPHICS_RENDER_H_SENTRY

#include "common_structs.h"
#include "cars_settings.h"
#include <unistd.h>

#define MIN_SCR_HEIGHT  50
#define MIN_SCR_WIDTH   (3*3+15)

enum key_vals { key_left, key_right, key_pause, key_resize, key_exit, skip };

/* if 0 then error, 1 all right */
int  graphic_init(rectangle *field, rectangle *way, useconds_t *delay);
enum key_vals get_key(void);
void graphic_flush(void);
void graphic_sleep(useconds_t delay);
void graphic_decrease_time(useconds_t *delay);

/* draw function work only in buffer; call update_frame() after any draw() */
void draw_rectangle(const rectangle *rect, int ch);
void draw_rect_frame(const rectangle *frame, int ch);
void draw_own_car(const point *up_left);
void draw_enemy_car(const point *up_left);
void draw_hide_car(const point *up_left);
/* init position must be 0 or 1 */
void draw_road(char position, int x, int max_y);

/* no refrech in draws(), then after any draw call this update_frame() */
void update_frame(void);

void graphic_pause(void);
void graphic_end(void);

#endif