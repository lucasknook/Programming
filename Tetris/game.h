#ifndef _GAME_H
#define _GAME_H

/* Grid size. */
#define COLS 10
#define ROWS (2 * COLS)

/* Game states. */
#define ONGOING 0
#define GAME_WON 1
#define GAME_OVER 2
#define ENDED 3

/* Differentiate between pixel and grid positions. */
typedef int p_int;
typedef int g_int;

/* Game struct. */
typedef struct {
    SDL_Color grid[COLS][ROWS];
    int status;
} game_t;

#endif