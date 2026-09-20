#include <stdio.h>  /* Error messages*/
#include <stdlib.h> /* Init srand() */
#include <time.h>   /* Timer & correct init srand() */
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

void handle_resize(own_car *player, enemy_car *enems,
                   rectangle *field, rectangle *way);
                   
static void game_end(own_car *player, enemy_car *enemys);

int main(void)
{
    rectangle field, way;
    enemy_car *enemys; /* No NULL needed */
    own_car *player; /* No NULL needed */
    enum game_state status;
    enum key_vals key;
    useconds_t delay;
    char road_bit;
    size_t meters;
    unsigned int gear, seconds; /* 64000 seconds */
    time_t cur_time, era_time;
    
    if (!game_start(&player, &enemys, &field, &way, &delay)) {
        graphic_end(); /* OS dispose cars automatically */
        fputs("FREE YOUR RAM", stderr);
        return 1;
    }

    draw_rect_vertical_frame(&way, CHR_BICH, brd_pair);

    seconds  = 0;
    meters   = 0;
    gear     = 1;
    cur_time = time(NULL);
    era_time = cur_time;
    status   = playing;
    while ((key = get_key()) != key_exit && status == playing) {
        time_t tmp_time;
        switch (key) {
        case key_right:  move_player(player, &way, to_right); break;
        case key_left:   move_player(player, &way, to_left);  break;
        case key_resize: handle_resize(player, enemys, &field, &way); break;
        case key_pause:  graphic_pause(); break;
        case key_exit:   /* Handling in while headline */ break;
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

        move_cars_and_strip(player, enemys, &way, &road_bit);
        meters++;

        draw_update_stats(meters, gear, seconds);
        update_frame();
        if (cars_is_hit(player, enemys))
            status = lose;

        graphic_sleep(delay);
    }

    if (status != playing) {
        graphic_sleep(FULL_SEC);
        graphic_show_lose_src(&field, meters, seconds);
    }

    game_end(player, enemys);
    fprintf(stderr, "Game cars:\n"
                    "Total meters:%ld\nTotal sec:%u\n", meters, seconds);
    return 0;
}

static int game_start(own_car **player, enemy_car **enemys,
                      rectangle *field, rectangle *way, useconds_t *delay)
{
    graphic_init(field, way, delay);

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

void handle_resize(own_car *player, enemy_car *enems,
                   rectangle *field, rectangle *way)
{
    char tmp = 0; /* wrapper */
    graphic_erase_screen();
    graphic_resize(field, way);
    cars_handling_resize(player, enems, way);
    draw_rect_vertical_frame(way, CHR_BICH, brd_pair);
    move_cars_and_strip(player, enems, way, &tmp);
    update_frame();
}

static void game_end(own_car *player, enemy_car *enemys)
{
    graphic_end();
    cars_dispose(player, enemys);
}