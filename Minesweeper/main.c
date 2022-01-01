#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

#include "game.h"
#include "logic.h"
#include "rendering.h"

int main(int argc, char *argv[]) {
    /* Initialize video. */
    if(SDL_Init(SDL_INIT_VIDEO)) {
        fprintf(stderr, "Failed to initialize sdl2: %s\n", SDL_GetError());
        return EXIT_FAILURE;
    }

    /* Init SDL_ttf. */
    TTF_Init();

    /* Load the font. */
    int fontsize = (5 * SCREEN_WIDTH) / (9 * COLS);
    TTF_Font *font = TTF_OpenFont("arial.ttf", fontsize);
    if (!font) {
        fprintf(stderr, "TTF_OpenFont Error: %s\n", TTF_GetError());
        return EXIT_FAILURE;
    }

    /* Create a window. */
    SDL_Window *window = SDL_CreateWindow("Minesweeper", 100, 100, SCREEN_WIDTH + 2 * (SCREEN_WIDTH / COLS), SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    if (!window) {
        fprintf(stderr, "SDL_CreateWindow ERROR: %s\n", SDL_GetError());
        return EXIT_FAILURE;
    }

    /* Create the renderer. */
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (!renderer) {
        SDL_DestroyWindow(window);
        fprintf(stderr, "SDL_CreateRenderer ERROR: %s\n", SDL_GetError());
        return EXIT_FAILURE;
    }

    /* Enable color blending. */
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);

    /* Create a struct game and initialize the grid with zero's. */
    struct game game;
    for (int i = 0; i < COLS; i++) {
        for (int j = 0; j < ROWS; j++) {
            game.grid[i][j] = BLANC;
        }
    }
    
    game.undiscovered = BOMBS;
    game.first_click = 1;
    game.state = ONGOING;

    /* Handle the events. */
    SDL_Event e;
    while (game.state != ENDED) {
        while (SDL_PollEvent(&e)) {
            switch (e.type) {
                case SDL_QUIT:
                    quit_game(&game);
                    break;
                case SDL_MOUSEBUTTONDOWN:
                    {
                        if (game.state == GAME_OVER || game.state == GAME_WON) {
                            break;
                        }

                        int grid_x = e.button.x / (SCREEN_WIDTH / COLS);
                        int grid_y = e.button.y / (SCREEN_HEIGHT / ROWS);
                        
                        if (e.button.button == SDL_BUTTON_LEFT) {
                            on_left_click(&game, grid_x, grid_y);
                            break;
                        }
                        
                        if (e.button.button == SDL_BUTTON_RIGHT) {
                            on_right_click(&game, grid_x, grid_y);
                            break;
                        }
                    }
                    break;
                default:
                    break;
            }
        }
        /* Draw the frames. */
        SDL_SetRenderDrawColor(renderer, 100, 100, 100, 255);
        SDL_RenderClear(renderer);
        render_game(renderer, &game, font);
        SDL_RenderPresent(renderer);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    TTF_CloseFont(font);
    TTF_Quit();
    SDL_Quit();

    return EXIT_SUCCESS;
}