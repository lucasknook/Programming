#ifndef _GAME_H
#define _GAME_H

/* Game states. */
#define ONGOING 0
#define GAME_WON 1
#define GAME_OVER 2
#define ENDED 3

typedef struct {
    int status;
} game_t;

void game_setup(game_t *game);

#endif