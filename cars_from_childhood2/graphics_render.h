#ifndef GRAPHICS_RENDER_H_SENTRY
#define GRAPHICS_RENDER_H_SENTRY

#include "common_structs.h"
#include "cars_settings.h"
#include <unistd.h>

#define RECORD_FILE_NAME ".cars_RECORD.txt"
#define MIN_SCR_HEIGHT  CAR_HEIGHT * 7
#define MIN_SCR_WIDTH   (3*3+15)

enum time_values {
    FULL_SEC   = 1000000,
    TIME_INIT  = FULL_SEC / 7,
    TIME_STEP  = 10000,
    TIME_1_ERA = 10,
};
enum my_color_pair { common_pair, playr_pair, enem_pair, brd_pair, lose_pair };
enum key_vals { key_left, key_right, key_pause, key_resize, key_exit, skip };

/* If 0 then error, 1 all right */
void graphic_init(rectangle *field, rectangle *way, useconds_t *delay);
void graphic_resize(rectangle *field, rectangle *way);
void graphic_erase_screen(void); 
enum key_vals get_key(void);
void graphic_key_flush(void);
void graphic_sleep(useconds_t delay);
void graphic_decrease_time(useconds_t *delay);

/* Draw function work only in buffer; call update_frame() after any draw() */
void draw_rectangle(const rectangle *rect, int ch, enum my_color_pair pair);
void draw_rect_vertical_frame(const rectangle *frame, int ch,
                              enum my_color_pair pair);

void draw_own_car(const point *up_left);
void draw_enemy_car(const point *up_left);
void draw_hide_car(const point *up_left);
/* Init position must be 0 or 1 */
void draw_road(char position, int x, int max_y);
void draw_update_stats(size_t meters, unsigned int gear, unsigned int sec);

/* No refrech in draws(), then after any draw call this update_frame() */
void update_frame(void);

void graphic_show_lose_src(rectangle *fld, size_t meters, unsigned int sec);
void graphic_pause(void);
void graphic_end(void);

#endif