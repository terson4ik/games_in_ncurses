#ifndef CARS_ENGINE_H_SENTRY
#define CARS_ENGINE_H_SENTRY

#include "cars_settings.h"
#include "common_structs.h"

enum enemy_cars_ind {FIRST = 0, SECOND, THIRD, FOURTH, ENEMYS_COUNT };

typedef struct enemys_tag enemy_car;
typedef struct car_tag own_car;

int  cars_init(own_car **p_own_c, enemy_car **p_enm_crs, const rectangle *way);
void own_car_move(own_car *own_c, int shift, const rectangle *way);
void enemy_car_update(enemy_car *enms, const rectangle *way);
int  cars_is_hit(own_car *own_c, enemy_car *enm_cars);
const car_vec_chrs *enemy_car_get_pos(const enemy_car *enms,
                                      enum enemy_cars_ind car);
const car_vec_chrs *own_car_get_pos(const own_car *own_c);
void cars_dispose(own_car *p_own_c, enemy_car *p_enm_cars);

#endif