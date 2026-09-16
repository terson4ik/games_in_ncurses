#ifndef TUI_CONF_SENTRY
#define TUI_CONF_SENTRY

#define CHR_BALL    '@'
#define CHR_PADDLE  '='
#define CHR_BLOCK   '*'
#define CHR_BOUNDS  '$'
#define CHR_EMPTY   ' '
#define SCORE_DESTROY_BLOCK 25
#define SCORE_NEW_LEVEL     100

enum game_colors_pair { /* colors in tui.c: static init_game_pairs() */
    COMMON_PAIR = 0, BALL_PAIR, PADDLE_PAIR, BORDER_PAIR,
    BLOCK_PAIR_1, BLOCK_PAIR_2, BLOCK_PAIR_3, BLOCK_PAIR_4,
    BG_PAIR, WIN_PAIR, LOSE_PAIR
};

#endif