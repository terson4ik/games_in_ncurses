#include <stdio.h>  /* error messages*/
#include <stdlib.h> /* init srand() */
#include <time.h>   /* timer & correct init srand() */
#include "common_structs.h"
#include "graphics_render.h"
#include "cars_settings.h"
#include "cars_engine.h"

enum move_vec {
    to_left  = -CAR_WIDTH,
    to_right = +CAR_WIDTH
};

enum game_state { lose, playing };

/* 0 is error, 1 is ok */
static int game_start(own_car **player, enemy_car **enemys,
                      rectangle *field, rectangle *way, useconds_t *delay);

static void move_player(own_car *player, const rectangle *way, int shift);
static void move_cars_and_strip(own_car *player, enemy_car *enemys,
                                const rectangle *way, char *road_bit);

static void update_road_mark(char *one_bit, int x, int max_y);

void handle_resize(own_car *player, enemy_car *enems, const rectangle *way);
static void game_end(own_car *player, enemy_car *enemys);

int main(void)
{
    rectangle game_field, game_way;
    enemy_car *enemys; /* no NULL needed */
    own_car *player; /* no NULL needed */
    enum game_state status;
    enum key_vals key;
    useconds_t delay;
    char road_bit;
    size_t meters;
    unsigned int gear, seconds; /* 64000 seconds */
    time_t cur_time, era_time;
    
    if (!game_start(&player, &enemys, &game_field, &game_way, &delay)) {
        graphic_end(); /* OS dispose cars automatically */
        fprintf(stderr, "Screen must be x=%d+, y=%d+\n",
                MIN_SCR_WIDTH, MIN_SCR_HEIGHT);   
        return 1;
    }

    draw_rect_vertical_frame(&game_way, CHR_BICH);

    seconds  = 0;
    meters   = 0;
    gear     = 1;
    cur_time = time(NULL);
    era_time = cur_time;
    status   = playing;
    while ((key = get_key()) != key_exit && status == playing) {
        time_t tmp_time;
        switch (key) {
        case key_right:  move_player(player, &game_way, to_right); break;
        case key_left:   move_player(player, &game_way, to_left);  break;
        case key_resize: handle_resize(player, enemys, &game_way); break;
        case key_pause:  graphic_pause(); break;
        case key_exit:   /* handling in while headline */ break;
        case skip:       break;
        }
        if ((key==key_left || key==key_right) && cars_is_hit(player, enemys))
            status = lose;

        graphic_key_flush();
        tmp_time =  time(NULL);
        if (cur_time < tmp_time) {
            seconds++;
            cur_time = tmp_time;
            if (cur_time - era_time >= TIME_1_ERA) {
                era_time = cur_time;
                gear++;
                graphic_decrease_time(&delay);
            }
        }

        move_cars_and_strip(player, enemys, &game_way, &road_bit);
        meters++;
        if (cars_is_hit(player, enemys))
            status = lose;

        draw_update_stats(meters, gear, seconds);
        update_frame();
        graphic_sleep(delay);
    }

    if (status != playing) {
        /* lose screen*/
    }

    game_end(player, enemys);
    return 0;
}

static int game_start(own_car **player, enemy_car **enemys,
                      rectangle *field, rectangle *way, useconds_t *delay)
{
    if (!graphic_init(field, way, delay))
        return 0;

    if (!cars_init(player, enemys, way))
        return 0;

    srand(time(NULL));
    return 1;
}

static void move_player(own_car *player, const rectangle *way, int shift)
{
    draw_hide_car(own_car_get_pos(player));
    own_car_move(player, way, shift);
}

static void move_cars_and_strip(own_car *player, enemy_car *enemys,
                                const rectangle *way, char *road_bit)
{
    enum enemy_index ind;
    for (ind = FIRST; ind < ENEMYS_COUNT; ind++)
        draw_hide_car(enemy_car_get_pos(enemys, ind));

    enemy_car_update(enemys, way);

    update_road_mark(road_bit, way->up_left.x + CAR_WIDTH+2, /* mid */
                     way->down_right.y);

    draw_own_car(own_car_get_pos(player));
    for (ind = FIRST; ind < ENEMYS_COUNT; ind++)
        draw_enemy_car(enemy_car_get_pos(enemys, ind));
}

static void update_road_mark(char *one_bit, int x, int max_y)
{
    draw_road(*one_bit, x, max_y);
    *one_bit = !*one_bit;
}

void handle_resize(own_car *player, enemy_car *enems, const rectangle *way)
{
}

static void game_end(own_car *player, enemy_car *enemys)
{
    graphic_end();
    cars_dispose(player, enemys);
}