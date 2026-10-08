#ifndef GRAPHIC_H_SENTRY
#define GRAPHIC_H_SENTRY

#include <unistd.h>
#include "common_structs.h"

enum keys { EXIT_KEY, LEFT, RIGHT, PAUSE };

enum keys get_key(void);
void graphic_init(void);
void graphic_resize(rectangle *field, rectangle *cup);
void graphic_end(void);
void update_screen(void);
void graphic_sleep(u_seconds_t delay);

#endif
