#ifndef _LOGIC_H
#define _LOGIC_H

#define LEFT -1, 0
#define RIGHT 1, 0
#define DOWN 0, -1

void game_setup(game_t *game);

void move_left(game_t *game);
void move_right(game_t *game);
void move_down(game_t *game);

/* Testing purposes. */
void move_up(game_t *game);

#endif