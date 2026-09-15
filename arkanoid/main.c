#include "default_structs.h"
#include "arkanoid.h"
#include "tui.h"

int main(void)
{
    point game_size;
    rectangle cup;
    block *blocks;
    paddle *pad;
    ball *pill;
    int key, delay;

    init_game(&game_size, &cup, &delay);
    objects_init(&pad, &pill, &blocks, &cup);
    draw_contour(&cup, CHR_BOUNDS);
    draw_rect(paddle_get_ptr_rect(pad), CHR_PADDLE);

    while ((key = get_key()) != KEY_ESCAPE && key != 'q' && key != 'Q'){
        /* game loop */
    }

    terminate_game();
    objects_erase(pad, pill, blocks);
    return 0;
}