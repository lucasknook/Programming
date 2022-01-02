#include <SDL2/SDL.h>

#include "game.h"
#include "rendering.h"

#include "logic.h"

void game_setup(game_t *game) {
    game->status = ONGOING;

    /* Set grid. */
    for (g_int i = 0; i < COLS; i++) {
        for (g_int j = 0; j < ROWS; j++) {
            game->grid[i][j] = GREY;
        }
    }

    /* Testing. */
    game->grid[0][0] = LIGHT_BLUE;
    game->grid[1][1] = BLUE;
    game->grid[2][2] = ORANGE;
    game->grid[3][3] = YELLOW;
    game->grid[4][4] = GREEN;
    game->grid[5][5] = PURPLE;
    game->grid[6][6] = RED;
}