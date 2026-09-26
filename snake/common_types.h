#ifndef COMMON_TYPES_SENTRY_HEADER
#define COMMON_TYPES_SENTRY_HEADER

typedef struct {
    int x, y;
} point;

typedef struct {
    point up_left, down_right;
} rectangle;

#endif