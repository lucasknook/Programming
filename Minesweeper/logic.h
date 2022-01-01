#ifndef LOGIC_H_
#define LOGIC_H_

/* Here, x and y positions are referring to the square indices, not their pixel positions. */

void on_left_click(struct game *game, int x, int y);

void on_right_click(struct game *game, int x, int y);

#endif