#include <stdio.h> /* error messages*/
#include <stdlib.h> /* init srand() */
#include <time.h>
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

static void move_enemys(enemy_car *enemys, const rectangle *way);
static void update_road_mark(char *one_bit);

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
    
    if (!game_start(&player, &enemys, &game_field, &game_way, &delay)) {
        graphic_end(); /* OS dispose cars automatically */
        fprintf(stderr, "Screen must be x=%d+, y=%d+\n",
                MIN_SCR_WIDTH, MIN_SCR_HEIGHT);   
        return 1;
    }

    status = playing;
    while ((key = get_key()) != key_exit && status == playing) {
        switch (key) {
        case key_right:  own_car_move(player, &game_way, to_right); break;
        case key_left:   own_car_move(player, &game_way, to_left);  break;
        case key_resize: handle_resize(player, enemys, &game_way);  break;
        case key_pause:  graphic_pause(); break;
        case key_exit:   /* handling in while headline */ break;
        case skip:       break;
        }
        graphic_flush();

        update_road_mark(&road_bit);
        draw_own_car(own_car_get_pos(player));
        move_enemys(enemys, &game_way);
        if (cars_is_hit(player, enemys))
            status = lose;

        update_frame();
        graphic_sleep(delay);
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

static void move_enemys(enemy_car *enemys, const rectangle *way)
{
}

static void update_road_mark(char *one_bit)
{
}

void handle_resize(own_car *player, enemy_car *enems, const rectangle *way)
{
}

static void game_end(own_car *player, enemy_car *enemys)
{
    graphic_end();
    cars_dispose(player, enemys);
}