#ifndef _TETROMINOS_H
#define _TETROMINOS_H

#define I 0
#define J 1
#define L 2
#define O 3
#define S 4
#define T 5
#define Z 6

/* The tetromino grid is 4x4 to create all tetromino's in any rotation. */
#define TETROMINO_GRID_WIDTH 4

typedef struct {
    int name;
    int rotation;
    g_int x, y;
    SDL_Color color;
    t_int squares[TETROMINO_GRID_WIDTH][TETROMINO_GRID_WIDTH];
} tetromino_t;

tetromino_t create_tetromino(int name, g_int x, g_int y, int rotation);

#endif