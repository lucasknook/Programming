#ifndef RENDERING_H_
#define RENDERING_H_

void render_game(SDL_Renderer *renderer, struct game *game, TTF_Font *font);

void quit_game(struct game *game);

#endif