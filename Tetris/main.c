#include <stdio.h>
#include <SDL2/SDL.h>
#include <stdbool.h>

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

    int frame = 0;

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
                        move_tetromino(&game, LEFT);
                        break;
                    }

                    if (event.key.keysym.sym == SDLK_d) {                    
                        move_tetromino(&game, RIGHT);
                        break;
                    }

                    if (event.key.keysym.sym == SDLK_s) {                    
                        move_tetromino(&game, DOWN);
                        break;
                    }

                    if (event.key.keysym.sym == SDLK_RETURN) {                    
                        rotate_tetromino(&game, 1);
                        break;
                    }

                    if (event.key.keysym.sym == SDLK_RSHIFT) {                    
                        rotate_tetromino(&game, 0);
                        break;
                    }

                    if (event.key.keysym.sym == SDLK_w) {                    
                        fast_drop(&game, &frame);
                        break;
                    }

                    break;
                default:
                    break;
            }
        }

        game_update(&game, &frame);

        render_all(renderer, &game);

    }

    render_quit(renderer, window);

    return EXIT_SUCCESS;
}