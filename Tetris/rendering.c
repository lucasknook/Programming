#include <SDL2/SDL.h>

#include "game.h"

#include "rendering.h"

void render_board(SDL_Renderer *renderer, game_t *game) {

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
    *window = SDL_CreateWindow("Tetris", 100, 100, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
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