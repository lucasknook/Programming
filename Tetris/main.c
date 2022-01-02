#include <stdio.h>

#include <SDL2/SDL.h>

#include "game.h"
#include "logic.h"
#include "rendering.h"
#include "tetrominos.h"

int main(int argc, char *argv[]) {
    /* Window and renderer setup. */
    SDL_Window *window;
    SDL_Renderer *renderer;
    if (render_init(&renderer, &window) == EXIT_FAILURE) {
        return EXIT_FAILURE;
    }

    /* Game setup. */
    game_t game;
    game_setup(&game);

    /* Event handler. */
    SDL_Event event;
    while (game.status == ONGOING) {
        while (SDL_PollEvent(&event)) {
            switch (event.type) {
                case SDL_QUIT:
                    game.status = ENDED;
                    break;
                case SDL_KEYDOWN:
                    if (event.key.keysym.sym == SDLK_a) {                    
                        move_left(&game);
                        break;
                    }

                    if (event.key.keysym.sym == SDLK_d) {                    
                        move_right(&game);
                        break;
                    }

                    if (event.key.keysym.sym == SDLK_s) {                    
                        move_down(&game);
                        break;
                    }

                    /* Testing purposes. */
                    if (event.key.keysym.sym == SDLK_w) {                    
                        move_up(&game);
                        break;
                    }
                    break;
                default:
                    break;
            }
        }

        render_all(renderer, &game);
    }

    render_quit(renderer, window);

    return EXIT_SUCCESS;
}