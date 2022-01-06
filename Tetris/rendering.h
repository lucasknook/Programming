#ifndef _RENDERING_H
#define _RENDERING_H

/* Window size.*/
#define WINDOW_WIDTH 400
#define WINDOW_HEIGHT (2 * WINDOW_WIDTH + EXTRA_ROWS * (WINDOW_WIDTH / COLS))

/* Window position. */
#define WINDOW_POS_X 1200
#define WINDOW_POS_Y 0

/* Misc. */
#define FPS 60

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
#define DARK_GREY (SDL_Color) {50, 50, 50} //Game over block

int compare_colors(SDL_Color color_1, SDL_Color color_2);

void render_all(SDL_Renderer *renderer, game_t *game);

int render_init(SDL_Renderer **renderer, SDL_Window **window);

void render_quit(SDL_Renderer *renderer, SDL_Window *window);

#endif