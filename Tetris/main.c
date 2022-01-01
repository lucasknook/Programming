#include <stdio.h>
#include <stdio.h>

#include <SDL2/SDL.h>

#include "rendering.h"
#include "game.h"

int main(int argc, char *argv[]) {
    /* Initialize video. */
    if(SDL_Init(SDL_INIT_VIDEO)) {
        fprintf(stderr, "Failed to initialize sdl2: %s\n", SDL_GetError());
        return EXIT_FAILURE;
    }

    /* Create a window. */
    SDL_Window *window = SDL_CreateWindow("Tetris", 100, 100, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    if (!window) {
        fprintf(stderr, "SDL_CreateWindow ERROR: %s\n", SDL_GetError());
        return EXIT_FAILURE;
    }

    /* Create a renderer. */
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (!renderer) {
        SDL_DestroyWindow(window);
        fprintf(stderr, "SDL_CreateRenderer ERROR: %s\n", SDL_GetError());
        return EXIT_FAILURE;
    }

    /* Game setup. */
    game_t game;
    game_setup(&game);

    /* Event handler. */
    SDL_Event event;
    while (game.status == ONGOING) {

        SDL_SetRenderDrawColor(renderer, 100, 100, 100, 255);
        SDL_RenderClear(renderer);
        SDL_RenderPresent(renderer);

        SDL_Delay(2000);
        break;
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return EXIT_SUCCESS;
}