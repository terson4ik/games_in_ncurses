#include "common_structs.h"
#include "cars_engine.h"

/* typedef enemy and own cars in header file */
struct car_tag {
    rectangle gabarites;
};

struct enemys_tag {
    struct car_tag cars[ENEMYS_COUNT];
};

int  cars_init(own_car **p_own_c, enemy_car **p_enm_crs, const rectangle *way)
{
    return 1;
}

void own_car_move(own_car *own_c, const rectangle *way, int shift)
{

}

void enemy_car_update(enemy_car *enms, const rectangle *way)
{

}

int  cars_is_hit(own_car *own_c, enemy_car *enm_cars)
{
    return 1;
}

const point *enemy_car_get_pos(const enemy_car *enms,
                               enum enemy_index car)
{
    return 0;
}

const point *own_car_get_pos(const own_car *own_c)
{
    return 0;
}

void cars_dispose(own_car *p_own_c, enemy_car *p_enm_cars)
{

}