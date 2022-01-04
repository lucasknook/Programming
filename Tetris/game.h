#ifndef _GAME_H
#define _GAME_H

/* Extra rows where the blocks spawn. */
#define EXTRA_ROWS 4

/* Grid size. */
#define COLS 10
#define ROWS (2 * COLS + EXTRA_ROWS)

/* Game settings. */
#define SPEED 60

/* Game states. */
#define ONGOING 0
#define GAME_WON 1
#define GAME_OVER 2
#define ENDED 3

/* Differentiate between pixel, game grid and tetromino grid positions. */
typedef int p_int;
typedef int g_int;
typedef int t_int;

/* Game struct. */
typedef struct {
    SDL_Color grid[COLS][ROWS + 2];
    int status;
} game_t;

#endif