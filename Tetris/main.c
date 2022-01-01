#include <stdio.h>
#include <stdio.h>

#include <SDL2/SDL.h>

#include "game.h"
#include "rendering.h"

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
                default:
                    break;
            }
        }

        render_all(renderer, &game);
    }

    render_quit(renderer, window);

    return EXIT_SUCCESS;
}