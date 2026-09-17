#ifndef COMMON_STRUCTS_H_SENTRY
#define COMMON_STRUCTS_H_SENTRY

#include "cars_settings.h"

typedef struct {
    int x, y;
} point;

typedef struct {
    point up_left;
    char top[CAR_WIDTH];
    char mid[CAR_WIDTH];
    char bot[CAR_WIDTH];
} car_vec_chrs;

typedef struct {
    point up_left, down_right;
} rectangle;

#endif