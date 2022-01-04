#include <SDL2/SDL.h>
#include <time.h>

#include "game.h"
#include "rendering.h"
#include "tetrominos.h"

#include "logic.h"

/* TODO: Remove magic numbers.*/
/* TODO: Reset or decrement frames when succesfully moving left or right. */

static tetromino_t current_tetromino;

/* bag[BAG_SIZE + 1] keeps track of the amount of tetromino's in the bag. */
int bag[BAG_SIZE + 1];

/* Returns 1 on game over, 0 otherwise. */
int check_game_over(game_t *game) {
    /* Check for top out / tetris death. */
    int oob_count = 0;
    if (current_tetromino.y <= EXTRA_ROWS + 2) {
        for (t_int i = 0; i < TETROMINO_GRID_WIDTH; i++) {
            for (t_int j = 0; j < TETROMINO_GRID_WIDTH; j++) {
                if (current_tetromino.squares[i][j] && current_tetromino.y - j < EXTRA_ROWS) {
                    oob_count++;
                }
            }
        }
    }
    if (oob_count == 4) {
        game->status = ENDED;
        return 1;
    } else {
        return 0;
    }
}

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

void place_tetromino(game_t *game, tetromino_t *tetromino) {
    for (t_int i = 0; i < TETROMINO_GRID_WIDTH; i++) {
        for (t_int j = 0; j < TETROMINO_GRID_WIDTH; j ++) {
            if (tetromino->squares[i][j]) {
                game->grid[tetromino->x + i][tetromino->y - j] = tetromino->color;
            }
        }
    }
}

int generate_random(int l, int r) {
    srand(time(0));
    return ((rand() % (r - l + 1)) + l);
}

void generate_bag(void) {
    // bag[0] = I;
    // bag[1] = J;
    // bag[2] = L;
    // bag[3] = O;
    // bag[4] = S;
    // bag[5] = T;
    // bag[6] = Z;
    bag[0] = I;
    bag[1] = I;
    bag[2] = I;
    bag[3] = I;
    bag[4] = I;
    bag[5] = I;
    bag[6] = I;

    /* Keeps track of the amount of tetromino's in the bag. */
    bag[7] = BAG_SIZE;
}

int generate_random_from_bag(void) {
    if (bag[BAG_SIZE] == 0) {
        generate_bag();
    }

    /* Generate random number from 0 to amount of pieces in the bag. */
    int random = generate_random(0, bag[BAG_SIZE] - 1);

    int random_from_bag = bag[random];

    for (int i = random; i < bag[BAG_SIZE]; i++) {
        bag[i] = bag[i + 1];
    }

    bag[BAG_SIZE] = bag[BAG_SIZE] - 1;

    return random_from_bag;
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

void place_random_tetronimo(game_t *game) {
    /* Clear lines. */
    check_and_clear_lines(game);

    if (check_game_over(game)) {
        return;
    }

    /* Place a new random tetronimo. */
    int random = generate_random_from_bag();

    if (random == I) {
        current_tetromino = create_tetromino(random, 3, EXTRA_ROWS + 1, 0);
    } else {
        current_tetromino = create_tetromino(random, 3, EXTRA_ROWS, 0);
    }

    while (tetromino_invalid_position(game, &current_tetromino)) {
        current_tetromino = create_tetromino(random, 3, current_tetromino.y - 1, 0);
    }
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

void move_tetromino(game_t *game, g_int dx, g_int dy) {
    /* Create a new tetromino. */
    int name = current_tetromino.name;

    t_int x = current_tetromino.x;
    t_int y = current_tetromino.y;

    int rotation = current_tetromino.rotation;

    tetromino_t moved_tetromino = create_tetromino(name, x + dx, y - dy, rotation);

    /* Remove the old tetromino. */
    remove_tetromino(game);

    /* Check if the movement is valid. */
    if (tetromino_invalid_position(game, &moved_tetromino)) {
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

    /* Check basic rotation. */
    if (!tetromino_invalid_position(game, &rotated_tetromino)) {

        /* Place the rotated tetronimo and make it the current one. */
        place_tetromino(game, &rotated_tetromino);
        current_tetromino = rotated_tetromino;

        return;
    }

    /* Wall kick system using the data table on the wiki. */
    int index;

    if (direction == 1) {
        switch (rotation) {
            case 0: 
                index = 6;
                break;
            case 1:
                index = 0;
                break;
            case 2:
                index = 2;
                break;
            case 3:
                index = 4;
                break;
        }
    }

    if (direction == 0) {
        switch (rotation) {
            case 0: 
                index = 1;
                break;
            case 1:
                index = 3;
                break;
            case 2:
                index = 5;
                break;
            case 3:
                index = 7;
                break;
        }
    }

    /* J, L, T, S, Z Wall Kicks. */
    if (name != I) {
        switch (index) {
            case 0:
                /* Test 2. */
                rotated_tetromino = create_tetromino(name, x - 1, y, rotation);
                if (tetromino_invalid_position(game, &rotated_tetromino)) {
                    /* Test 3. */
                    rotated_tetromino = create_tetromino(name, x - 1, y - 1, rotation);
                    if (tetromino_invalid_position(game, &rotated_tetromino)) {
                        /* Test 4. */
                        rotated_tetromino = create_tetromino(name, x, y + 2, rotation);
                        if (tetromino_invalid_position(game, &rotated_tetromino)) {
                            /* Test 5. */
                            rotated_tetromino = create_tetromino(name, x - 1, y + 2, rotation);
                            if (tetromino_invalid_position(game, &rotated_tetromino)) {
                                break;
                            }
                        }
                    }
                }
                place_tetromino(game, &rotated_tetromino);
                current_tetromino = rotated_tetromino;
                return;
            case 1:
                /* Test 2. */
                rotated_tetromino = create_tetromino(name, x + 1, y, rotation);
                if (tetromino_invalid_position(game, &rotated_tetromino)) {
                    /* Test 3. */
                    rotated_tetromino = create_tetromino(name, x + 1, y + 1, rotation);
                    if (tetromino_invalid_position(game, &rotated_tetromino)) {
                        /* Test 4. */
                        rotated_tetromino = create_tetromino(name, x, y - 2, rotation);
                        if (tetromino_invalid_position(game, &rotated_tetromino)) {
                            /* Test 5. */
                            rotated_tetromino = create_tetromino(name, x + 1, y - 2, rotation);
                            if (tetromino_invalid_position(game, &rotated_tetromino)) {
                                break;
                            }
                        }
                    }
                }
                place_tetromino(game, &rotated_tetromino);
                current_tetromino = rotated_tetromino;
                return;
            case 2:
                /* Test 2. */
                rotated_tetromino = create_tetromino(name, x + 1, y, rotation);
                if (tetromino_invalid_position(game, &rotated_tetromino)) {
                    /* Test 3. */
                    rotated_tetromino = create_tetromino(name, x + 1, y + 1, rotation);
                    if (tetromino_invalid_position(game, &rotated_tetromino)) {
                        /* Test 4. */
                        rotated_tetromino = create_tetromino(name, x, y - 2, rotation);
                        if (tetromino_invalid_position(game, &rotated_tetromino)) {
                            /* Test 5. */
                            rotated_tetromino = create_tetromino(name, x + 1, y - 2, rotation);
                            if (tetromino_invalid_position(game, &rotated_tetromino)) {
                                break;
                            }
                        }
                    }
                }
                place_tetromino(game, &rotated_tetromino);
                current_tetromino = rotated_tetromino;
                return;
            case 3:
                /* Test 2. */
                rotated_tetromino = create_tetromino(name, x - 1, y, rotation);
                if (tetromino_invalid_position(game, &rotated_tetromino)) {
                    /* Test 3. */
                    rotated_tetromino = create_tetromino(name, x - 1, y - 1, rotation);
                    if (tetromino_invalid_position(game, &rotated_tetromino)) {
                        /* Test 4. */
                        rotated_tetromino = create_tetromino(name, x, y + 2, rotation);
                        if (tetromino_invalid_position(game, &rotated_tetromino)) {
                            /* Test 5. */
                            rotated_tetromino = create_tetromino(name, x - 1, y + 2, rotation);
                            if (tetromino_invalid_position(game, &rotated_tetromino)) {
                                break;
                            }
                        }
                    }
                }
                place_tetromino(game, &rotated_tetromino);
                current_tetromino = rotated_tetromino;
                return;
            case 4:
                /* Test 2. */
                rotated_tetromino = create_tetromino(name, x + 1, y, rotation);
                if (tetromino_invalid_position(game, &rotated_tetromino)) {
                    /* Test 3. */
                    rotated_tetromino = create_tetromino(name, x + 1, y - 1, rotation);
                    if (tetromino_invalid_position(game, &rotated_tetromino)) {
                        /* Test 4. */
                        rotated_tetromino = create_tetromino(name, x, y + 2, rotation);
                        if (tetromino_invalid_position(game, &rotated_tetromino)) {
                            /* Test 5. */
                            rotated_tetromino = create_tetromino(name, x + 1, y + 2, rotation);
                            if (tetromino_invalid_position(game, &rotated_tetromino)) {
                                break;
                            }
                        }
                    }
                }
                place_tetromino(game, &rotated_tetromino);
                current_tetromino = rotated_tetromino;
                return;
            case 5:
                /* Test 2. */
                rotated_tetromino = create_tetromino(name, x - 1, y, rotation);
                if (tetromino_invalid_position(game, &rotated_tetromino)) {
                    /* Test 3. */
                    rotated_tetromino = create_tetromino(name, x - 1, y + 1, rotation);
                    if (tetromino_invalid_position(game, &rotated_tetromino)) {
                        /* Test 4. */
                        rotated_tetromino = create_tetromino(name, x, y - 2, rotation);
                        if (tetromino_invalid_position(game, &rotated_tetromino)) {
                            /* Test 5. */
                            rotated_tetromino = create_tetromino(name, x - 1, y - 2, rotation);
                            if (tetromino_invalid_position(game, &rotated_tetromino)) {
                                break;
                            }
                        }
                    }
                }
                place_tetromino(game, &rotated_tetromino);
                current_tetromino = rotated_tetromino;
                return;
            case 6:
                /* Test 2. */
                rotated_tetromino = create_tetromino(name, x - 1, y, rotation);
                if (tetromino_invalid_position(game, &rotated_tetromino)) {
                    /* Test 3. */
                    rotated_tetromino = create_tetromino(name, x - 1, y + 1, rotation);
                    if (tetromino_invalid_position(game, &rotated_tetromino)) {
                        /* Test 4. */
                        rotated_tetromino = create_tetromino(name, x, y - 2, rotation);
                        if (tetromino_invalid_position(game, &rotated_tetromino)) {
                            /* Test 5. */
                            rotated_tetromino = create_tetromino(name, x - 1, y - 2, rotation);
                            if (tetromino_invalid_position(game, &rotated_tetromino)) {
                                break;
                            }
                        }
                    }
                }
                place_tetromino(game, &rotated_tetromino);
                current_tetromino = rotated_tetromino;
                return;
            case 7:
                /* Test 2. */
                rotated_tetromino = create_tetromino(name, x + 1, y, rotation);
                if (tetromino_invalid_position(game, &rotated_tetromino)) {
                    /* Test 3. */
                    rotated_tetromino = create_tetromino(name, x + 1, y - 1, rotation);
                    if (tetromino_invalid_position(game, &rotated_tetromino)) {
                        /* Test 4. */
                        rotated_tetromino = create_tetromino(name, x, y + 2, rotation);
                        if (tetromino_invalid_position(game, &rotated_tetromino)) {
                            /* Test 5. */
                            rotated_tetromino = create_tetromino(name, x + 1, y + 2, rotation);
                            if (tetromino_invalid_position(game, &rotated_tetromino)) {
                                break;
                            }
                        }
                    }
                }
                place_tetromino(game, &rotated_tetromino);
                current_tetromino = rotated_tetromino;
                return;
        }
    }

    /* I Wall kicks. */
    if (name == I) {
        switch (index) {
            case 0:
                /* Test 2. */
                rotated_tetromino = create_tetromino(name, x - 2, y, rotation);
                if (tetromino_invalid_position(game, &rotated_tetromino)) {
                    /* Test 3. */
                    rotated_tetromino = create_tetromino(name, x + 1, y, rotation);
                    if (tetromino_invalid_position(game, &rotated_tetromino)) {
                        /* Test 4. */
                        rotated_tetromino = create_tetromino(name, x - 2, y + 1, rotation);
                        if (tetromino_invalid_position(game, &rotated_tetromino)) {
                            /* Test 5. */
                            rotated_tetromino = create_tetromino(name, x + 1, y - 2, rotation);
                            if (tetromino_invalid_position(game, &rotated_tetromino)) {
                                break;
                            }
                        }
                    }
                }
                place_tetromino(game, &rotated_tetromino);
                current_tetromino = rotated_tetromino;
                return;
            case 1:
                /* Test 2. */
                rotated_tetromino = create_tetromino(name, x + 2, y, rotation);
                if (tetromino_invalid_position(game, &rotated_tetromino)) {
                    /* Test 3. */
                    rotated_tetromino = create_tetromino(name, x - 1, y, rotation);
                    if (tetromino_invalid_position(game, &rotated_tetromino)) {
                        /* Test 4. */
                        rotated_tetromino = create_tetromino(name, x + 2, y - 1, rotation);
                        if (tetromino_invalid_position(game, &rotated_tetromino)) {
                            /* Test 5. */
                            rotated_tetromino = create_tetromino(name, x - 1, y + 2, rotation);
                            if (tetromino_invalid_position(game, &rotated_tetromino)) {
                                break;
                            }
                        }
                    }
                }
                place_tetromino(game, &rotated_tetromino);
                current_tetromino = rotated_tetromino;
                return;
            case 2:
                /* Test 2. */
                rotated_tetromino = create_tetromino(name, x - 1, y, rotation);
                if (tetromino_invalid_position(game, &rotated_tetromino)) {
                    /* Test 3. */
                    rotated_tetromino = create_tetromino(name, x + 2, y, rotation);
                    if (tetromino_invalid_position(game, &rotated_tetromino)) {
                        /* Test 4. */
                        rotated_tetromino = create_tetromino(name, x - 1, y - 2, rotation);
                        if (tetromino_invalid_position(game, &rotated_tetromino)) {
                            /* Test 5. */
                            rotated_tetromino = create_tetromino(name, x + 2, y + 1, rotation);
                            if (tetromino_invalid_position(game, &rotated_tetromino)) {
                                break;
                            }
                        }
                    }
                }
                place_tetromino(game, &rotated_tetromino);
                current_tetromino = rotated_tetromino;
                return;
            case 3:
                /* Test 2. */
                rotated_tetromino = create_tetromino(name, x + 1, y, rotation);
                if (tetromino_invalid_position(game, &rotated_tetromino)) {
                    /* Test 3. */
                    rotated_tetromino = create_tetromino(name, x - 2, y, rotation);
                    if (tetromino_invalid_position(game, &rotated_tetromino)) {
                        /* Test 4. */
                        rotated_tetromino = create_tetromino(name, x + 1, y + 2, rotation);
                        if (tetromino_invalid_position(game, &rotated_tetromino)) {
                            /* Test 5. */
                            rotated_tetromino = create_tetromino(name, x - 2, y - 1, rotation);
                            if (tetromino_invalid_position(game, &rotated_tetromino)) {
                                break;
                            }
                        }
                    }
                }
                place_tetromino(game, &rotated_tetromino);
                current_tetromino = rotated_tetromino;
                return;
            case 4:
                /* Test 2. */
                rotated_tetromino = create_tetromino(name, x + 2, y, rotation);
                if (tetromino_invalid_position(game, &rotated_tetromino)) {
                    /* Test 3. */
                    rotated_tetromino = create_tetromino(name, x - 1, y, rotation);
                    if (tetromino_invalid_position(game, &rotated_tetromino)) {
                        /* Test 4. */
                        rotated_tetromino = create_tetromino(name, x + 2, y - 1, rotation);
                        if (tetromino_invalid_position(game, &rotated_tetromino)) {
                            /* Test 5. */
                            rotated_tetromino = create_tetromino(name, x - 1, y + 2, rotation);
                            if (tetromino_invalid_position(game, &rotated_tetromino)) {
                                break;
                            }
                        }
                    }
                }
                place_tetromino(game, &rotated_tetromino);
                current_tetromino = rotated_tetromino;
                return;
            case 5:
                /* Test 2. */
                rotated_tetromino = create_tetromino(name, x - 2, y, rotation);
                if (tetromino_invalid_position(game, &rotated_tetromino)) {
                    /* Test 3. */
                    rotated_tetromino = create_tetromino(name, x + 1, y, rotation);
                    if (tetromino_invalid_position(game, &rotated_tetromino)) {
                        /* Test 4. */
                        rotated_tetromino = create_tetromino(name, x - 2, y + 1, rotation);
                        if (tetromino_invalid_position(game, &rotated_tetromino)) {
                            /* Test 5. */
                            rotated_tetromino = create_tetromino(name, x + 1, y - 2, rotation);
                            if (tetromino_invalid_position(game, &rotated_tetromino)) {
                                break;
                            }
                        }
                    }
                }
                place_tetromino(game, &rotated_tetromino);
                current_tetromino = rotated_tetromino;
                return;
            case 6:
                /* Test 2. */
                rotated_tetromino = create_tetromino(name, x + 1, y, rotation);
                if (tetromino_invalid_position(game, &rotated_tetromino)) {
                    /* Test 3. */
                    rotated_tetromino = create_tetromino(name, x - 2, y, rotation);
                    if (tetromino_invalid_position(game, &rotated_tetromino)) {
                        /* Test 4. */
                        rotated_tetromino = create_tetromino(name, x + 1, y + 2, rotation);
                        if (tetromino_invalid_position(game, &rotated_tetromino)) {
                            /* Test 5. */
                            rotated_tetromino = create_tetromino(name, x - 2, y - 1, rotation);
                            if (tetromino_invalid_position(game, &rotated_tetromino)) {
                                break;
                            }
                        }
                    }
                }
                place_tetromino(game, &rotated_tetromino);
                current_tetromino = rotated_tetromino;
                return;
            case 7:
                /* Test 2. */
                rotated_tetromino = create_tetromino(name, x - 1, y, rotation);
                if (tetromino_invalid_position(game, &rotated_tetromino)) {
                    /* Test 3. */
                    rotated_tetromino = create_tetromino(name, x + 2, y, rotation);
                    if (tetromino_invalid_position(game, &rotated_tetromino)) {
                        /* Test 4. */
                        rotated_tetromino = create_tetromino(name, x - 1, y - 2, rotation);
                        if (tetromino_invalid_position(game, &rotated_tetromino)) {
                            /* Test 5. */
                            rotated_tetromino = create_tetromino(name, x + 2, y + 1, rotation);
                            if (tetromino_invalid_position(game, &rotated_tetromino)) {
                                break;
                            }
                        }
                    }
                }
                place_tetromino(game, &rotated_tetromino);
                current_tetromino = rotated_tetromino;
                return;
        }
    }


    /* Try replacing the breaks in the switch statement above with this down here. */
    place_tetromino(game, &current_tetromino);
    return;
}

void fast_drop(game_t *game, int *frame) {
    for (int i = 0; i < ROWS; i++) {
        move_tetromino(game, DOWN);
    }

    place_random_tetronimo(game);
    
    *frame = 0;
}

void game_update(game_t *game, int *frame) {
    if (*frame > SPEED) {
        g_int x = current_tetromino.x;
        g_int y = current_tetromino.y;

        move_tetromino(game, DOWN);

        /* Lock piece and generate new one. */
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
        for (g_int j = 0; j < ROWS + EXTRA_HIDDEN_ROWS; j++) {
            game->grid[i][j] = GREY;
        }
    }

    generate_bag();

    place_random_tetronimo(game);
}