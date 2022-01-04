#include <SDL2/SDL.h>
#include <stdio.h>

#include "game.h"

#include "rendering.h"

/* If the colors are the same, return 0, otherwise, return 1. */
int compare_colors(SDL_Color color_1, SDL_Color color_2) {
    if (color_1.r == color_2.r && color_1.g == color_2.g && color_1.b == color_2.b) {
        return 0;
    }

    return 1;
}

void render_square(SDL_Renderer *renderer, SDL_Color color, g_int x, g_int y) {
    /* Create rectangle. */
    SDL_Rect rectangle;
    rectangle.w = SQUARE_WIDTH;
    rectangle.h = SQUARE_WIDTH;
    rectangle.x = SQUARE_WIDTH * x;
    rectangle.y = SQUARE_WIDTH * y;
    
    /* Set rectangle background color (based on grid value). */
    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, 255);
    /* Render rectangle background. */
    SDL_RenderFillRect(renderer, &rectangle);

    if (!compare_colors(color, GREY) && y < EXTRA_ROWS) {
        return;
    }

    /* Set rectangle outline color (black). */
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    /* Render rectangle outline. */
    SDL_RenderDrawRect(renderer, &rectangle);
}

void render_board(SDL_Renderer *renderer, game_t *game) {
    for (g_int i = 0; i < COLS; i++) {
        for (g_int j = 0; j < ROWS; j++) {
            render_square(renderer, game->grid[i][j], i, j);
        }
    }

    /* Render the game over line. */
    SDL_SetRenderDrawColor(renderer, RED.r, RED.g, RED.b, 255);
    for (int i = EXTRA_ROWS * SQUARE_WIDTH - 3; i < EXTRA_ROWS * SQUARE_WIDTH + 3; i++) {
        SDL_RenderDrawLine(renderer, 0, i , WINDOW_WIDTH, i);
    }
}

void render_all(SDL_Renderer *renderer, game_t *game) {
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    render_board(renderer, game);

    SDL_RenderPresent(renderer);
}

int render_init(SDL_Renderer **renderer, SDL_Window **window) {
    /* Initialize video. */
    if(SDL_Init(SDL_INIT_VIDEO)) {
        fprintf(stderr, "Failed to initialize sdl2: %s\n", SDL_GetError());
        return EXIT_FAILURE;
    }

    /* Create a window. */
    *window = SDL_CreateWindow("Tetris", WINDOW_POS_X, WINDOW_POS_Y, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN);
    if (!window) {
        fprintf(stderr, "SDL_CreateWindow ERROR: %s\n", SDL_GetError());
        return EXIT_FAILURE;
    }

    /* Create a renderer. */
    *renderer = SDL_CreateRenderer(*window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (!renderer) {
        SDL_DestroyWindow(*window);
        fprintf(stderr, "SDL_CreateRenderer ERROR: %s\n", SDL_GetError());
        return EXIT_FAILURE;
    }

    return 0;
}

void render_quit(SDL_Renderer *renderer, SDL_Window *window) {
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}