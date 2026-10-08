#include <stdio.h>

#include "common_structs.h"
#include "engine.h"
#include "graphic.h"

static int game_init();
static void game_destroy();
static void game_handle_resize();
static void game_reload_image();

int main(void)
{
    rectangle field, cup;
    u_seconds_t delay;
    game_init();

    game_destroy();
    return 0;
}

static int game_init()
{
    graphic_init();
    game_handle_resize();
}

static void game_destroy()
{
    graphic_end();
}

static void game_handle_resize()
{
}

static void game_reload_image()
{
}
