#include <stdlib.h> /* all objects a dynamic */
#include "common_structs.h"
#include "cars_engine.h"

#define ENEMY_STEP 1

/* typedef enemy and own cars in header file */
struct car_tag {
    point up_left;
};

struct enemys_tag {
    struct car_tag cars[ENEMYS_COUNT];
};

static void enemy_cars_set_x(struct car_tag *enem1, struct car_tag *enem2,
                             const rectangle *way)
{
    do {
        enem1->up_left.x = (rand() % STRIPS )*3 + 1 + way->up_left.x;
        enem2->up_left.x = (rand() % STRIPS )*3 + 1 + way->up_left.x;
    } while (enem1->up_left.x == enem2->up_left.x);
}

int  cars_init(own_car **p_own_c, enemy_car **p_enm_crs, const rectangle *way)
{
    *p_own_c   = malloc(sizeof **p_own_c);
    *p_enm_crs = malloc(sizeof **p_enm_crs);
    if (!*p_own_c || !*p_enm_crs)
        return 0;

   cars_handling_resize(*p_own_c, *p_enm_crs, way);
   return 1;
}

void
cars_handling_resize(own_car *player, enemy_car *enems, const rectangle *way)
{
    player->up_left.x = way->up_left.x + 1 + CAR_WIDTH;
    player->up_left.y = way->down_right.y / 2;

    enems->cars[FIRST].up_left.y  = CAR_HEIGHT;
    enems->cars[SECOND].up_left.y = enems->cars[FIRST].up_left.y;

    enems->cars[THIRD].up_left.y  = player->up_left.y + CAR_HEIGHT;
    enems->cars[FOURTH].up_left.y = enems->cars[THIRD].up_left.y;

    enemy_cars_set_x(&enems->cars[FIRST], &enems->cars[SECOND], way);
    enemy_cars_set_x(&enems->cars[THIRD], &enems->cars[FOURTH], way);
}

void own_car_move(own_car *own_c, const rectangle *way, int shift)
{
    if (own_c->up_left.x + shift > way->up_left.x &&
        own_c->up_left.x + shift < way->down_right.x)
    {
        own_c->up_left.x += shift;
    }
}

void enemy_car_update(enemy_car *enms, const rectangle *way)
{
    enum enemy_index ind;
    for (ind = FIRST; ind < ENEMYS_COUNT; ind += 2) {
        if (enms->cars[ind].up_left.y + ENEMY_STEP >= way->down_right.y) {
            enms->cars[ind].up_left.y = -CAR_HEIGHT;
            enms->cars[ind+1].up_left.y = enms->cars[ind].up_left.y;
            enemy_cars_set_x(&enms->cars[ind], &enms->cars[ind+1], way);
        }
    }

    for (ind = FIRST; ind < ENEMYS_COUNT; ind++)
        enms->cars[ind].up_left.y += ENEMY_STEP;
}

int  cars_is_hit(own_car *own_c, enemy_car *enm_cars)
{
    enum enemy_index ind;
    const int play_x = own_c->up_left.x;
    const int play_y = own_c->up_left.y;

    for (ind = FIRST; ind < ENEMYS_COUNT; ind++) {
        const int enem_x = enm_cars->cars[ind].up_left.x;
        const int enem_y = enm_cars->cars[ind].up_left.y;
        if (play_x == enem_x &&
            ((play_y >= enem_y && play_y <= enem_y + CAR_HEIGHT-1) ||
            (play_y+CAR_HEIGHT-1 > enem_y &&
            play_y+CAR_HEIGHT-1 < enem_y+CAR_HEIGHT-1)))
        {
            return 1;
        }
    }
    
    return 0;
}

const point *enemy_car_get_pos(const enemy_car *enms,
                               enum enemy_index car)
{
    return &enms->cars[car].up_left;
}

const point *own_car_get_pos(const own_car *own_c)
{
    return &own_c->up_left;
}

void cars_dispose(own_car *p_own_c, enemy_car *p_enm_cars)
{
    free(p_enm_cars);
    free(p_own_c);
}