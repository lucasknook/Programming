#ifndef _LOGIC_H
#define _LOGIC_H

#define LEFT -1, 0
#define RIGHT 1, 0
#define DOWN 0, -1

#define CLOCKWISE 0
#define COUNTER_CLOCKWISE 1

#define BAG_SIZE 7

void game_setup(game_t *game);

void game_update(game_t *game, int *frame);

void game_end(SDL_Renderer *renderer, game_t *game);

void move_tetromino(game_t *game, int *frame, int dx, int dy);

void rotate_tetromino(game_t *game, int *frame, int direction);

void fast_drop(game_t *game, int *frame);

void hold_tetromino(game_t *game, int *frame);

#endif