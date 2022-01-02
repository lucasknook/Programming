#include <SDL2/SDL.h>

#include "game.h"
#include "rendering.h"

#include "tetrominos.h"

tetromino_t create_tetromino(int name, g_int x, g_int y) {
    tetromino_t tetromino;

    tetromino.x = x;
    tetromino.y = y;

    for (t_int i = 0; i < TETROMINO_GRID_WIDTH; i++) {
        for (t_int j = 0; j < TETROMINO_GRID_WIDTH; j++) {
            tetromino.squares[i][j] = 0;
        }
    }
    
    switch(name) {
        case I:
            tetromino.color = LIGHT_BLUE;
            tetromino.squares[0][2] = 1;
            tetromino.squares[1][2] = 1;
            tetromino.squares[2][2] = 1;
            tetromino.squares[3][2] = 1;
            break;
        case J:
            tetromino.color = BLUE;
            tetromino.squares[0][1] = 1;
            tetromino.squares[0][2] = 1;
            tetromino.squares[1][1] = 1;
            tetromino.squares[2][1] = 1;
            break;
        case L:
            tetromino.color = ORANGE;
            tetromino.squares[0][1] = 1;
            tetromino.squares[1][1] = 1;
            tetromino.squares[2][1] = 1;
            tetromino.squares[2][2] = 1;
            break;
        case O:
            tetromino.color = YELLOW;
            tetromino.squares[1][1] = 1;
            tetromino.squares[1][2] = 1;
            tetromino.squares[2][1] = 1;
            tetromino.squares[2][2] = 1;
            break;
        case S:
            tetromino.color = GREEN;
            tetromino.squares[0][1] = 1;
            tetromino.squares[1][1] = 1;
            tetromino.squares[1][2] = 1;
            tetromino.squares[2][2] = 1;
            break;
        case T:
            tetromino.color = PURPLE;
            tetromino.squares[0][1] = 1;
            tetromino.squares[1][1] = 1;
            tetromino.squares[1][2] = 1;
            tetromino.squares[2][1] = 1;
            break;
        case Z:
            tetromino.squares[0][2] = 1;
            tetromino.squares[1][1] = 1;
            tetromino.squares[1][2] = 1;
            tetromino.squares[2][1] = 1;
            tetromino.color = RED;
            break;
    }

    return tetromino;
}