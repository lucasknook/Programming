#ifndef _RENDERING_H
#define _RENDERING_H

/* Window size.*/
#define WINDOW_WIDTH 500
#define WINDOW_HEIGHT (2 * WINDOW_WIDTH)

/* Window position. */
#define WINDOW_POS_X 0
#define WINDOW_POS_Y 0

/* Square size. */
#define SQUARE_WIDTH (WINDOW_WIDTH / COLS)

/* Square colors. */
#define LIGHT_BLUE (SDL_Color) {0, 240, 240} //I-block
#define BLUE (SDL_Color) {0, 0, 240} //J-block
#define ORANGE (SDL_Color) {240, 160, 0} //L-block
#define YELLOW (SDL_Color) {240, 240, 0} //O-block
#define GREEN (SDL_Color) {0, 240, 0} //S-block
#define PURPLE (SDL_Color) {160, 0, 240} //T-block
#define RED (SDL_Color) {240, 0, 0} //Z-block

#define GREY (SDL_Color) {100, 100, 100} //No block

void render_all(SDL_Renderer *renderer, game_t *game);

int render_init(SDL_Renderer **renderer, SDL_Window **window);

void render_quit(SDL_Renderer *renderer, SDL_Window *window);

#endif