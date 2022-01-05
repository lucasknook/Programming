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

    /* Frame counter. */
    int frame = 1;

    /* To make sure you cannot hold to rotate. */
    int pressed_rotate_left = 0;
    int pressed_rotate_right = 0;

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
                        move_tetromino(&game, &frame, LEFT);
                        break;
                    }

                    if (event.key.keysym.sym == SDLK_d) {                    
                        move_tetromino(&game, &frame, RIGHT);
                        break;
                    }

                    if (event.key.keysym.sym == SDLK_s) {                    
                        move_tetromino(&game, &frame, DOWN);
                        break;
                    }

                    if (event.key.keysym.sym == SDLK_RETURN && !pressed_rotate_right) {                    
                        rotate_tetromino(&game, &frame, 1);
                        pressed_rotate_right = 1;
                        break;
                    }

                    if (event.key.keysym.sym == SDLK_RSHIFT && !pressed_rotate_left) {                    
                        rotate_tetromino(&game, &frame, 0);
                        pressed_rotate_left = 1;
                        break;
                    }

                    if (event.key.keysym.sym == SDLK_w) {                    
                        fast_drop(&game, &frame);
                        break;
                    }

                    break;
                case SDL_KEYUP:
                    if (event.key.keysym.sym == SDLK_RETURN) {                    
                        pressed_rotate_right = 0;
                        break;
                    }

                    if (event.key.keysym.sym == SDLK_RSHIFT) {                    
                        pressed_rotate_left = 0;
                        break;
                    }
                default:
                    break;
            }
        }

        game_update(&game, &frame);

        render_all(renderer, &game);

        fprintf(stderr, "Frame: %d\n", frame);

        /* Next frame. */
        if (frame == 60) {
            frame = 1;
        } else {
            frame++;
        }
    }
    
    game_end(renderer, &game);

    render_quit(renderer, window);

    return EXIT_SUCCESS;
}