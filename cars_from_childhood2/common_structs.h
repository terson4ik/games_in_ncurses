#ifndef COMMON_STRUCTS_H_SENTRY
#define COMMON_STRUCTS_H_SENTRY

#include "cars_settings.h"

typedef struct {
    int x, y;
} point;

typedef struct {
    point up_left, down_right;
} rectangle;

#endif