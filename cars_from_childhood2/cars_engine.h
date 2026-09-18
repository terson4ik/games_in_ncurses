#ifndef CARS_ENGINE_H_SENTRY
#define CARS_ENGINE_H_SENTRY

#include "cars_settings.h"
#include "common_structs.h"

enum enemy_index { FIRST = 0, SECOND, THIRD, FOURTH, ENEMYS_COUNT };

typedef struct enemys_tag enemy_car;
typedef struct car_tag own_car;

/* NULLs not needed; 0 is error, 1 is ok */
int  cars_init(own_car **p_own_c, enemy_car **p_enm_crs, const rectangle *way);
int
cars_handling_resize(own_car *player, enemy_car *enems, const rectangle *way);

/* only move; check hit in cars_is_hit() */
void own_car_move(own_car *own_c, const rectangle *way, int shift);
void enemy_car_update(enemy_car *enms, const rectangle *way);

int  cars_is_hit(own_car *own_c, enemy_car *enm_cars);

/* get up_left position to draw cars;*/
const point *enemy_car_get_pos(const enemy_car *enms, enum enemy_index car);
const point *own_car_get_pos(const own_car *own_c);

void cars_dispose(own_car *p_own_c, enemy_car *p_enm_cars);

#endif