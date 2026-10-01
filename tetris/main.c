#include <stdio.h>

#include "common_structs.h"
#include "engine.h"
#include "graphic.h"

static int game_init();
static void game_handle_resize();
static void game_destroy();
static void game_reload_image();
int main(void)
{
    game_init();

    game_destroy();
    return 0;
}
