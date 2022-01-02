#include <SDL2/SDL.h>

#include "game.h"
#include "rendering.h"
#include "tetrominos.h"

#include "logic.h"

static tetromino_t current_tetromino;

void place_tetromino(game_t *game, tetromino_t *tetromino) {
    for (t_int i = 0; i < TETROMINO_GRID_WIDTH; i++) {
        for (t_int j = 0; j < TETROMINO_GRID_WIDTH; j ++) {
            if (tetromino->squares[i][j]) {
                game->grid[tetromino->x + i][tetromino->y - j] = tetromino->color;
            }
        }
    }
}

void move_tetromino(game_t *game, tetromino_t *tetromino, g_int x, g_int y) {
    /* Check if the movement is valid. */
    for (t_int i = 0; i < TETROMINO_GRID_WIDTH; i++) {
        for (t_int j = 0; j < TETROMINO_GRID_WIDTH; j++) {
            if (tetromino->squares[i][j]) {
                /* If the new square is not GREY. */
                if (compare_colors(game->grid[tetromino->x + i + x][tetromino->y - j - y], GREY)) {
                    /* If the new square is not part of the old tetromino. */
                    if (i + x < 0 || i + x > TETROMINO_GRID_WIDTH) {
                        return;
                    }

                    if (j + y < 0 || j + y > TETROMINO_GRID_WIDTH) {
                        return;
                    }

                    if (!tetromino->squares[i + x][j + y]) {
                        return;
                    }
                }
            }

            if (tetromino->x + i + x < 0 || tetromino->x + i + x > COLS) {
                return;
            }

            if (tetromino->y - j - y > ROWS) {
                return;
            }
        }
    }

    /* Remove the old tetromino. */
    for (t_int i = 0; i < TETROMINO_GRID_WIDTH; i++) {
        for (t_int j = 0; j < TETROMINO_GRID_WIDTH; j++) {
            if (tetromino->squares[i][j]) {
                game->grid[tetromino->x + i][tetromino->y - j] = GREY;
            }
        }
    }

    tetromino->x += x;
    tetromino->y -= y;

    place_tetromino(game, tetromino);
}

void move_left(game_t *game) {
    move_tetromino(game, &current_tetromino, LEFT);
}

void move_right(game_t *game) {
    move_tetromino(game, &current_tetromino, RIGHT);
}

void move_down(game_t *game) {
    move_tetromino(game, &current_tetromino, DOWN);
}

/* Testing purposes. */
void move_up(game_t *game) {
    move_tetromino(game, &current_tetromino, 0, 1);
}

void game_setup(game_t *game) {
    game->status = ONGOING;

    /* Set grid. */
    for (g_int i = 0; i < COLS; i++) {
        for (g_int j = 0; j < ROWS; j++) {
            game->grid[i][j] = GREY;
        }
    }

    /* Testing. */
    tetromino_t tetromino_I = create_tetromino(I, 3, 21);
    place_tetromino(game, &tetromino_I);

    tetromino_t tetromino_Z = create_tetromino(Z, 3, 19);
    place_tetromino(game, &tetromino_Z);

    tetromino_t tetromino_T = create_tetromino(T, 3, 17);
    place_tetromino(game, &tetromino_T);

    tetromino_t tetromino_O = create_tetromino(O, 3, 15);
    place_tetromino(game, &tetromino_O);

    tetromino_t tetromino_L = create_tetromino(L, 3, 13);
    place_tetromino(game, &tetromino_L);

    tetromino_t tetromino_S = create_tetromino(S, 3, 11);
    place_tetromino(game, &tetromino_S);

    tetromino_t tetromino_J = create_tetromino(J, 3, 9);
    place_tetromino(game, &tetromino_J);

    current_tetromino = tetromino_L;
}