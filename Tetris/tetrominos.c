#include <SDL2/SDL.h>

#include "game.h"
#include "rendering.h"
#include "logic.h"

#include "tetrominos.h"

tetromino_t create_tetromino(int name, g_int x, g_int y, int rotation) {
    tetromino_t tetromino;

    tetromino.name = name;

    tetromino.rotation = rotation;

    tetromino.x = x;
    tetromino.y = y;

    for (t_int i = 0; i < TETROMINO_GRID_WIDTH; i++) {
        for (t_int j = 0; j < TETROMINO_GRID_WIDTH; j++) {
            tetromino.squares[i][j] = 0;
        }
    }
    
    switch (name) {
        case I:
            tetromino.color = LIGHT_BLUE;
            switch (rotation) {
                case 0:
                    tetromino.squares[0][2] = 1;
                    tetromino.squares[1][2] = 1;
                    tetromino.squares[2][2] = 1;
                    tetromino.squares[3][2] = 1;
                    break;
                case 1:
                    tetromino.squares[2][0] = 1;
                    tetromino.squares[2][1] = 1;
                    tetromino.squares[2][2] = 1;
                    tetromino.squares[2][3] = 1;
                    break;
                case 2:
                    tetromino.squares[0][1] = 1;
                    tetromino.squares[1][1] = 1;
                    tetromino.squares[2][1] = 1;
                    tetromino.squares[3][1] = 1;
                    break;
                case 3:
                    tetromino.squares[1][0] = 1;
                    tetromino.squares[1][1] = 1;
                    tetromino.squares[1][2] = 1;
                    tetromino.squares[1][3] = 1;
                    break;
            }
            break;
        case J:
            tetromino.color = BLUE;
            switch (rotation) {
                case 0:
                    tetromino.squares[0][1] = 1;
                    tetromino.squares[0][2] = 1;
                    tetromino.squares[1][1] = 1;
                    tetromino.squares[2][1] = 1;
                    break;
                case 1:
                    tetromino.squares[1][0] = 1;
                    tetromino.squares[1][1] = 1;
                    tetromino.squares[1][2] = 1;
                    tetromino.squares[2][2] = 1;
                    break;
                case 2:
                    tetromino.squares[0][1] = 1;
                    tetromino.squares[1][1] = 1;
                    tetromino.squares[2][1] = 1;
                    tetromino.squares[2][0] = 1;
                    break;
                case 3:
                    tetromino.squares[0][0] = 1;
                    tetromino.squares[1][0] = 1;
                    tetromino.squares[1][1] = 1;
                    tetromino.squares[1][2] = 1;
                    break;
            }
            break;
        case L:
            tetromino.color = ORANGE;
            switch (rotation) {
                case 0:
                    tetromino.squares[0][1] = 1;
                    tetromino.squares[1][1] = 1;
                    tetromino.squares[2][1] = 1;
                    tetromino.squares[2][2] = 1;
                    break;
                case 1:
                    tetromino.squares[1][0] = 1;
                    tetromino.squares[2][0] = 1;
                    tetromino.squares[1][1] = 1;
                    tetromino.squares[1][2] = 1;
                    break;
                case 2:
                    tetromino.squares[0][0] = 1;
                    tetromino.squares[0][1] = 1;
                    tetromino.squares[1][1] = 1;
                    tetromino.squares[2][1] = 1;
                    break;
                case 3:
                    tetromino.squares[1][0] = 1;
                    tetromino.squares[1][1] = 1;
                    tetromino.squares[1][2] = 1;
                    tetromino.squares[0][2] = 1;
                    break;
            }
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
            switch (rotation) {
                case 0:
                    tetromino.squares[0][1] = 1;
                    tetromino.squares[1][1] = 1;
                    tetromino.squares[1][2] = 1;
                    tetromino.squares[2][2] = 1;
                    break;
                case 1:
                    tetromino.squares[1][1] = 1;
                    tetromino.squares[1][2] = 1;
                    tetromino.squares[2][1] = 1;
                    tetromino.squares[2][0] = 1;
                    break;
                case 2:
                    tetromino.squares[1][1] = 1;
                    tetromino.squares[0][0] = 1;
                    tetromino.squares[1][0] = 1;
                    tetromino.squares[2][1] = 1;
                    break;
                case 3:
                    tetromino.squares[1][1] = 1;
                    tetromino.squares[0][2] = 1;
                    tetromino.squares[0][1] = 1;
                    tetromino.squares[1][0] = 1;
                    break;
            }
            break;
        case T:
            tetromino.color = PURPLE;
            switch (rotation) {
                case 0:
                    tetromino.squares[0][1] = 1;
                    tetromino.squares[1][1] = 1;
                    tetromino.squares[1][2] = 1;
                    tetromino.squares[2][1] = 1;
                    break;
                case 1:
                    tetromino.squares[1][1] = 1;
                    tetromino.squares[1][0] = 1;
                    tetromino.squares[1][2] = 1;
                    tetromino.squares[2][1] = 1;
                    break;
                case 2:
                    tetromino.squares[1][1] = 1;
                    tetromino.squares[0][1] = 1;
                    tetromino.squares[1][0] = 1;
                    tetromino.squares[2][1] = 1;
                    break;
                case 3:
                    tetromino.squares[1][1] = 1;
                    tetromino.squares[0][1] = 1;
                    tetromino.squares[1][0] = 1;
                    tetromino.squares[1][2] = 1;
                    break;
            }
            break;
        case Z:
            tetromino.color = RED;
            switch (rotation) {
                case 0:
                    tetromino.squares[0][2] = 1;
                    tetromino.squares[1][1] = 1;
                    tetromino.squares[1][2] = 1;
                    tetromino.squares[2][1] = 1;
                    break;
                case 1:
                    tetromino.squares[1][1] = 1;
                    tetromino.squares[1][0] = 1;
                    tetromino.squares[2][1] = 1;
                    tetromino.squares[2][2] = 1;
                    break;
                case 2:
                    tetromino.squares[1][1] = 1;
                    tetromino.squares[0][1] = 1;
                    tetromino.squares[1][0] = 1;
                    tetromino.squares[2][0] = 1;
                    break;
                case 3:
                    tetromino.squares[1][1] = 1;
                    tetromino.squares[0][0] = 1;
                    tetromino.squares[0][1] = 1;
                    tetromino.squares[1][2] = 1;
                    break;
            }
            break;
    }

    return tetromino;
}