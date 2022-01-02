#ifndef _RENDERING_H
#define _RENDERING_H

/* Window size.*/
#define SCREEN_WIDTH 300
#define SCREEN_HEIGHT (2 * SCREEN_WIDTH)

/* Object sizes. */
#define SQUARE_WIDTH (SCREEN_WIDTH / COLS)

void render_all(SDL_Renderer *renderer, game_t *game);

int render_init(SDL_Renderer **renderer, SDL_Window **window);

void render_quit(SDL_Renderer *renderer, SDL_Window *window);

#endif