#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <stdlib.h>
#include <stdio.h>

#include "game.h"
#include "rendering.h"

SDL_Rect create_rectangle(int x, int y) {
    SDL_Rect rectangle;
    rectangle.w = SCREEN_WIDTH / COLS;
    rectangle.h = SCREEN_HEIGHT / ROWS;
    rectangle.x = rectangle.w * x;
    rectangle.y = rectangle.h * y;

    return rectangle;
}

void draw_character(SDL_Surface *surface, SDL_Texture *texture, SDL_Renderer *renderer, SDL_Rect rectangle, SDL_Color color, TTF_Font *font, char *character) {
    surface = TTF_RenderText_Solid(font, character, color);
    texture = SDL_CreateTextureFromSurface(renderer, surface);

    int text_width = 0;
    int text_height = 0;

    SDL_QueryTexture(texture, NULL, NULL, &text_width, &text_height);
    SDL_Rect dstrect = {rectangle.x + rectangle.w / 3, rectangle.y + rectangle.h / 5, text_width, text_height};
    SDL_RenderCopy(renderer, texture, NULL, &dstrect);

    /* Cleanup texture and surface. */
    SDL_FreeSurface(surface);
    SDL_DestroyTexture(texture);
}

void draw_rectangle(SDL_Renderer *renderer, int x, int y, struct game *game, TTF_Font *font) {
    /* Create a color, surface and texture. */
    SDL_Surface *surface = NULL; /* This NULL is here because asan will whine otherwise idk why. */
    SDL_Texture *texture;

    int number = game->grid[x][y];

    /* Create a rectangle. */
    SDL_Rect rectangle = create_rectangle(x, y);

    /* Draw the cells outline. */
    if (number == BLANC) {
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    } else {
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    }
    /* Draw the rectangle outline. */
    SDL_RenderDrawRect(renderer, &rectangle);

    /* Draw the cells background. */
    if (number == BLANC) {
        SDL_SetRenderDrawColor(renderer, 90, 90, 90, 150);
    } else {
        SDL_SetRenderDrawColor(renderer, 190, 190, 190, 150);
    }
    /* Fill the rectangle. */
    SDL_RenderFillRect(renderer, &rectangle);

    /* Add the number or letter. */
    switch (number) {
        case BOMB:
            draw_character(surface, texture, renderer, rectangle, (SDL_Color) {0, 0, 0}, font, "B");
            break;
        case FLAG:
            draw_character(surface, texture, renderer, rectangle, (SDL_Color) {255, 0, 0}, font, "F");
            break;
        case 1:
            draw_character(surface, texture, renderer, rectangle, (SDL_Color) {0, 0, 255}, font, "1");
            break;
        case 2:
            draw_character(surface, texture, renderer, rectangle, (SDL_Color) {0, 128, 0}, font, "2");
            break;
        case 3:
            draw_character(surface, texture, renderer, rectangle, (SDL_Color) {255, 0, 0}, font, "3");
            break;
        case 4:
            draw_character(surface, texture, renderer, rectangle, (SDL_Color) {0, 0, 128}, font, "4");
            break;
        case 5:
            draw_character(surface, texture, renderer, rectangle, (SDL_Color) {128, 0, 0}, font, "5");
            break;
        case 6:
            draw_character(surface, texture, renderer, rectangle, (SDL_Color) {0, 128, 128}, font, "6");
            break;
        case 7:
            draw_character(surface, texture, renderer, rectangle, (SDL_Color) {0, 0, 0}, font, "7");
            break;
        case 8:
            draw_character(surface, texture, renderer, rectangle, (SDL_Color) {110, 110, 110}, font, "8");
            break;
    }

    /* Placing the cleanup functions here causes a lot of memory leaks. idk why. */
}

void draw_sidebar(SDL_Renderer *renderer, struct game *game, TTF_Font *font) {
    char undiscovered[10];
    
    snprintf(undiscovered, 10, "%d", game->undiscovered);

    SDL_Rect rectangle;
    rectangle.w = 2 * (SCREEN_WIDTH / COLS);
    rectangle.h = (SCREEN_HEIGHT / ROWS);
    rectangle.x = rectangle.w / 2 * COLS;
    rectangle.y = rectangle.h;

    SDL_Surface *surface; /* But here it works just fine????. */
    SDL_Texture *texture;

    switch (game->state) {
        case ONGOING:
            draw_character(surface, texture, renderer, rectangle, (SDL_Color) {0, 0, 0}, font, undiscovered);
            break;
        case GAME_OVER:
            draw_character(surface, texture, renderer, rectangle, (SDL_Color) {0, 0, 0}, font, ":(");
            break;
        case GAME_WON:
            draw_character(surface, texture, renderer, rectangle, (SDL_Color) {0, 0, 0}, font, ":)");
            break;
    }
}

void render_game(SDL_Renderer *renderer, struct game *game, TTF_Font *font) {
    for (int i = 0; i < COLS; i++) {
        for (int j = 0; j < ROWS; j++) {
            draw_rectangle(renderer, i, j, game, font);
        }
    }

    draw_sidebar(renderer, game, font);
}

void quit_game(struct game *game) {
    game->state = ENDED;
}