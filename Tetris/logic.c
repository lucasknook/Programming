#include <SDL2/SDL.h>
#include <time.h>

#include "game.h"
#include "rendering.h"
#include "tetrominos.h"

#include "logic.h"

/* TODO: Remove magic numbers.*/
/* TODO: Reset or decrement frames when succesfully moving left or right. */

static tetromino_t current_tetromino;

void check_and_clear_lines(game_t *game) {
    for (g_int j = ROWS - 1; j > 0; j--) {
        int amount = 0;

        for (g_int i = 0; i < COLS; i++) {
            if (compare_colors(game->grid[i][j], GREY)) {
                amount++;
            } else break;
        }

        if (amount == 10) {
            /* Clear the line. */
            for (g_int i = 0; i < COLS; i++) {
                game->grid[i][j] = GREY;
            }

            /* Move everything above the line down. */
            for (g_int y = j; y > 0; y--) {
                for (g_int x = 0; x < COLS; x++) {
                    game->grid[x][y] = game->grid[x][y - 1];
                }
            }

            /* Check the removed line again, since there could be
             * more than one line that has to be cleared.
             */
            j++;
        }
    }
}

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

void place_random_tetronimo(game_t *game) {
    /* Clear lines. */
    check_and_clear_lines(game);

    /* Place a new random tetronimo. */
    int random = generate_random(0, 6);

    current_tetromino = create_tetromino(random, 3, 3, 0);
    place_tetromino(game, &current_tetromino);
}

void remove_tetromino(game_t *game) {
    g_int x = current_tetromino.x;
    g_int y = current_tetromino.y;

    for (t_int i = 0; i < TETROMINO_GRID_WIDTH; i++) {
        for (t_int j = 0; j < TETROMINO_GRID_WIDTH; j++) {
            if (current_tetromino.squares[i][j]) {
                game->grid[x + i][y - j] = GREY;
            }
        }
    }
}

/* Check if a tetromino is in a valid position. 
 * If it is, return 1, otherise, return 0.
 */
int tetromino_invalid_position(game_t *game, tetromino_t *tetromino) {
    g_int x = tetromino->x;
    g_int y = tetromino->y;

    for (t_int i = 0; i < TETROMINO_GRID_WIDTH; i++) {
        for (t_int j = 0; j < TETROMINO_GRID_WIDTH; j++) {
            if (tetromino->squares[i][j]) {
                if (compare_colors(game->grid[x + i][y - j], GREY)) {
                    return 1;
                }

                if (tetromino->x + i < 0 || tetromino->x + i > COLS) {
                    return 1;
                }

                if (tetromino->y - j >= ROWS) {
                    return 1;
                }
            }
        }
    }

    return 0;
}

void move_tetromino(game_t *game, g_int dx, g_int dy, int check_valid) {
    /* Create a new tetromino. */
    int name = current_tetromino.name;

    t_int x = current_tetromino.x;
    t_int y = current_tetromino.y;

    int rotation = current_tetromino.rotation;

    tetromino_t moved_tetromino = create_tetromino(name, x + dx, y - dy, rotation);

    /* Remove the old tetromino. */
    remove_tetromino(game);

    /* Check if the movement is valid. */
    if (check_valid && tetromino_invalid_position(game, &moved_tetromino)) {
        place_tetromino(game, &current_tetromino);
        return;
    }

    /* Place the moved tetronimo and make it the current one. */
    place_tetromino(game, &moved_tetromino);
    current_tetromino = moved_tetromino;
}

int new_rotation(int rotation, int direction) {
    if (direction) {
        if (rotation == 3) {
            return 0;
        }

        return rotation + 1;
    }

    if (rotation == 0) {
        return 3;
    }

    return rotation - 1;
}

void rotate_tetromino(game_t *game, int direction) {
    if (current_tetromino.name == O) {
        return;
    }

    /* Create a new tetromino. */
    int name = current_tetromino.name;

    t_int x = current_tetromino.x;
    t_int y = current_tetromino.y;

    int rotation = current_tetromino.rotation;
    rotation = new_rotation(rotation, direction);

    tetromino_t rotated_tetromino = create_tetromino(name, x, y, rotation);

    /* Remove the old tetronimo. */
    remove_tetromino(game);

    /* Check if the rotation is valid. */
    if (tetromino_invalid_position(game, &rotated_tetromino)) {
        place_tetromino(game, &current_tetromino);
        return;
    }

    /* Place the rotated tetronimo and make it the current one. */
    place_tetromino(game, &rotated_tetromino);
    current_tetromino = rotated_tetromino;
}

void fast_drop(game_t *game, int *frame) {
    for (int i = 0; i < 20; i++) {
        move_tetromino(game, DOWN, 1);
    }

    place_random_tetronimo(game);
    
    *frame = 0;
}

void game_update(game_t *game, int *frame) {
    if (*frame > SPEED) {
        g_int x = current_tetromino.x;
        g_int y = current_tetromino.y;

        move_tetromino(game, DOWN, 1);

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