#include <SDL2/SDL.h>
#include <time.h>

#include "game.h"
#include "rendering.h"
#include "tetrominos.h"

#include "logic.h"

static tetromino_t current_tetromino;

int generate_random(int l, int r) {
    srand(time(0));
    return (int) ((rand() % (r - l + 1)) + l);
}

void place_tetromino(game_t *game, tetromino_t *tetromino) {
    for (t_int i = 0; i < TETROMINO_GRID_WIDTH; i++) {
        for (t_int j = 0; j < TETROMINO_GRID_WIDTH; j ++) {
            if (tetromino->squares[i][j]) {
                game->grid[tetromino->x + i][tetromino->y - j] = tetromino->color;
            }
        }
    }
}

/* TODO: Remove magic numbers. */
void place_random_tetronimo(game_t *game) {
    int random = generate_random(0, 6);

    current_tetromino = create_tetromino(random, 3, 3);
    place_tetromino(game, &current_tetromino);
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
                if (tetromino->x + i + x < 0 || tetromino->x + i + x > COLS) {
                    return;
                }

                if (tetromino->y - j - y >= ROWS) {
                    return;
                }
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

/* TODO: Reset or decrement frames when succesfully moving left or right. */

void move_left(game_t *game) {
    move_tetromino(game, &current_tetromino, LEFT);
}

void move_right(game_t *game) {
    move_tetromino(game, &current_tetromino, RIGHT);
}

void move_down(game_t *game) {
    move_tetromino(game, &current_tetromino, DOWN);
}

void game_update(game_t *game, int *frame) {
    if (*frame > SPEED) {
        g_int x = current_tetromino.x;
        g_int y = current_tetromino.y;

        move_down(game);

        if (current_tetromino.x == x && current_tetromino.y == y) {
            place_random_tetronimo(game);
        }

        *frame = 0;
    } else {
        *frame = *frame + 1;
    }
}

void game_setup(game_t *game) {
    game->status = ONGOING;

    /* Set grid. */
    for (g_int i = 0; i < COLS; i++) {
        for (g_int j = 0; j < ROWS; j++) {
            game->grid[i][j] = GREY;
        }
    }

    place_random_tetronimo(game);
}