#ifndef _LOGIC_H
#define _LOGIC_H

#define LEFT -1, 0
#define RIGHT 1, 0
#define DOWN 0, -1

#define CLOCKWISE 0
#define COUNTER_CLOCKWISE 1

void game_setup(game_t *game);

void game_update(game_t *game, int *frame);

void move_left(game_t *game);
void move_right(game_t *game);
void move_down(game_t *game);

void rotate_tetromino(game_t *game, int clockwise);

void fast_drop(game_t *game, int *frame);

#endif