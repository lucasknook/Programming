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
/* TODO: Fast drop. */
/* TODO: r. 131. */

void place_random_tetronimo(game_t *game) {
    int random = generate_random(0, 6);

    current_tetromino = create_tetromino(random, 3, 3, 0);
    place_tetromino(game, &current_tetromino);
}

void move_tetromino(game_t *game, g_int dx, g_int dy) {
    /* Create a new tetromino. */
    int name = current_tetromino.name;

    t_int x = current_tetromino.x;
    t_int y = current_tetromino.y;

    int rotation = current_tetromino.rotation;

    tetromino_t moved_tetromino = create_tetromino(name, x + dx, y - dy, rotation);

    /* Remove the old tetromino. */
    for (t_int i = 0; i < TETROMINO_GRID_WIDTH; i++) {
        for (t_int j = 0; j < TETROMINO_GRID_WIDTH; j++) {
            if (current_tetromino.squares[i][j]) {
                game->grid[x + i][y - j] = GREY;
            }
        }
    }

    /* Check if the movement is valid. */
    for (t_int i = 0; i < TETROMINO_GRID_WIDTH; i++) {
        for (t_int j = 0; j < TETROMINO_GRID_WIDTH; j++) {
            if (moved_tetromino.squares[i][j]) {
                if (compare_colors(game->grid[x + dx + i][y - dy - j], GREY)) {
                    /* Place the old tetromino back. */
                    place_tetromino(game, &current_tetromino);
                    return;
                }

                if (moved_tetromino.x + i < 0 || moved_tetromino.x + i > COLS) {
                    /* Place the old tetronimo back. */
                    place_tetromino(game, &current_tetromino);
                    return;
                }

                if (moved_tetromino.y - j >= ROWS) {
                    /* Place the old tetronimo back. */
                    place_tetromino(game, &current_tetromino);
                    return;
                }
            }
        }
    }

    /* Place the moved tetronimo and make it the current one. */
    place_tetromino(game, &moved_tetromino);
    current_tetromino = moved_tetromino;
}

void rotate_tetromino(game_t *game, int clockwise) {
    if (current_tetromino.name == O) {
        return;
    }

    /* Create a new tetromino. */
    int name = current_tetromino.name;

    t_int x = current_tetromino.x;
    t_int y = current_tetromino.y;

    int rotation = current_tetromino.rotation;
    if (clockwise) {
        if (rotation == 3) {
            rotation = 0;
        } else {
            rotation++;
        }
    } else {
        if (rotation == 0) {
            rotation = 3;
        } else {
            rotation--;
        }
    }

    tetromino_t rotated_tetromino = create_tetromino(name, x, y, rotation);

    /* Remove the old tetronimo. */
    for (t_int i = 0; i < TETROMINO_GRID_WIDTH; i++) {
        for (t_int j = 0; j < TETROMINO_GRID_WIDTH; j++) {
            if (current_tetromino.squares[i][j]) {
                game->grid[x + i][y - j] = GREY;
            }
        }
    }

    /* Check if the rotation is valid. */
    for (t_int i = 0; i < TETROMINO_GRID_WIDTH; i++) {
        for (t_int j = 0; j < TETROMINO_GRID_WIDTH; j++) {
            if (rotated_tetromino.squares[i][j]) {
                if (compare_colors(game->grid[x + i][y - j], GREY)) {
                    /* Place the old tetronimo back. */
                    place_tetromino(game, &current_tetromino);
                    return;
                }

                /* TODO: Make sure that you can still turn at the borders,
                 * by moving to the left or right. If successfull, try to rotate again.
                 */
                if (rotated_tetromino.x + i < 0 || rotated_tetromino.x + i > COLS) {
                    /* Place the old tetronimo back. */
                    place_tetromino(game, &current_tetromino);
                    return;
                }

                if (rotated_tetromino.y - j >= ROWS) {
                    /* Place the old tetronimo back. */
                    place_tetromino(game, &current_tetromino);
                    return;
                }
            }
        }
    }

    /* Place the rotated tetronimo and make it the current one. */
    place_tetromino(game, &rotated_tetromino);
    current_tetromino = rotated_tetromino;
}

/* TODO: Reset or decrement frames when succesfully moving left or right. */

void move_left(game_t *game) {
    move_tetromino(game, LEFT);
}

void move_right(game_t *game) {
    move_tetromino(game, RIGHT);
}

void move_down(game_t *game) {
    move_tetromino(game, DOWN);
}

void fast_drop(game_t *game, int *frame) {
    for (int i = 0; i < 20; i++) {
        move_tetromino(game, DOWN);
    }

    place_random_tetronimo(game);
    
    *frame = 0;
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