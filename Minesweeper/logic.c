#include <stdlib.h>
#include <time.h>

#include "game.h"
#include "logic.h"

/* Game struct board is used to store the randomly generated minesweeper field,
 * while the game struct game is used to render the game on the screen.
*/

int generate_random(int l, int r) {
    return (int) ((rand() % (r - l + 1)) + l);
}

void generate_bombs(struct game *game, int x, int y) {
    /* Set the random seed. */
    srand(time(0));

    int bomb_amount = 0;
    while (bomb_amount < BOMBS) {
        int random_x = generate_random(0, COLS - 1);
        int random_y = generate_random(0, ROWS - 1);

        /* If there is already a bomb present or 
         * an attempt to place a bomb at the mouse cursor is made,
         * generate a new random value.
         */
        if (game->solution[random_x][random_y] == BOMB || (random_x == x && random_y == y)) {
            continue;
        } else {
            game->solution[random_x][random_y] = BOMB;
            bomb_amount++;
        }
    }
}

int bomb_neighbours(struct game *game, int x, int y) {
    int neighbours = 0;

    /* Check top three. */
    if (y > 0) {
        /* Center. */
        if (game->solution[x][y - 1] == BOMB) {
            neighbours++;
        }

        /* Left. */
        if (x > 0) {
            if (game->solution[x - 1][y - 1] == BOMB) {
                neighbours++;
            }
        }

        /* Right. */
        if (x < COLS - 1) {
            if (game->solution[x + 1][y - 1] == BOMB) {
                neighbours++;
            }
        }
    }

    /* Check bottom three. */
    if (y < ROWS - 1) {
        /* Center. */
        if (game->solution[x][y + 1] == BOMB) {
            neighbours++;
        }

        /* Left. */
        if (x > 0) {
            if (game->solution[x - 1][y + 1] == BOMB) {
                neighbours++;
            }
        }

        /* Right. */
        if (x < COLS - 1) {
            if (game->solution[x + 1][y + 1] == BOMB) {
                neighbours++;
            }
        }
    }

    /* Left center. */
    if (x > 0) {
        if (game->solution[x - 1][y] == BOMB) {
            neighbours++;
        } 
    }

    /* Right center. */
    if (x < COLS - 1) {
        if (game->solution[x + 1][y] == BOMB) {
            neighbours++;
        } 
    }

    return neighbours;
}

void generate_numbers(struct game *game) {
    for (int i = 0; i < COLS; i++) {
        for (int j = 0; j < ROWS; j++) {

            if (game->solution[i][j] == BLANC) {
                /* Find the amount of bombs around the cell. */
                int number = bomb_neighbours(game, i, j);

                if (number == BLANC) {
                    game->solution[i][j] = FILLED;
                } else {
                    game->solution[i][j] = number;
                }
            }
        }
    }
}

void generate_board(struct game *game, int x, int y) {
    /* Set everything to blanc. */
    for (int i = 0; i < COLS; i++) {
        for (int j = 0; j < ROWS; j++) {
            game->solution[i][j] = BLANC;
        }
    }

    generate_bombs(game, x, y);
    generate_numbers(game);
}

void reveal_solution(struct game *game) {
    for (int i = 0; i < COLS; i++) {
        for (int j = 0; j < ROWS; j++) {
            game->grid[i][j] = game->solution[i][j];
        }
    }
}

/* Recursive flood filling function. */
void flood_fill(struct game *game, int x, int y) {
    if (game->grid[x][y] == FLAG) {
        game->undiscovered++;
    }

    if (game->grid[x][y] == game->solution[x][y]) {
        return;
    }

    game->grid[x][y] = game->solution[x][y];
    
    if (game->solution[x][y] < FILLED) {
        return;
    }

    if (y > 0) {
        flood_fill(game, x, y - 1);
    }

    if (y < ROWS - 1) {
        flood_fill(game, x, y + 1);
    }

    if (x > 0) {
        flood_fill(game, x - 1, y);
    }

    if (x < COLS - 1) {
        flood_fill(game, x + 1, y);
    }
}

void on_sidebar_click(struct game *game, int x, int y) {
    /* Check for win. */
    int correct_flags = 0;

    for (int i = 0; i < COLS; i++) {
        for (int j = 0; j < ROWS; j++) {
            if (game->undiscovered == 0 && (game->grid[i][j] == FLAG && game->solution[i][j] == BOMB)) {
                correct_flags++;
            }
        }
    }

    if (correct_flags == BOMBS) {
        game->state = GAME_WON;
    } else {
        game->state = GAME_OVER;
    }
}

void on_left_click(struct game *game, int x, int y) {
    if (x >= COLS) {
        on_sidebar_click(game, x, y);
        return;
    }

    if (game->first_click) {
        generate_board(game, x, y);
        game->first_click = 0;
    }

    if (game->grid[x][y] == FLAG) {
        return;
    }

    if (game->solution[x][y] == BOMB) {
        /* Make the background of the cell red, maybe an extra define called BOMB_DEFEAT. */
        reveal_solution(game);

        game->state = GAME_OVER;
        
        return;
    }

    /* Use flood fill. */
    flood_fill(game, x, y);
}

void on_right_click(struct game *game, int x, int y) {
    if (x >= COLS) {
        return;
    }

    if (game->grid[x][y] == BLANC && game->undiscovered > 0) {
        game->grid[x][y] =  FLAG;
        game->undiscovered--;
        return;
    }
    if (game->grid[x][y] == FLAG) {
        game->grid[x][y] = BLANC;
        game->undiscovered++;
        return;
    }
}