#ifndef GAME_H_
#define GAME_H_

/* Change this to your liking, just
 * make sure the WIDTH to HEIGHT ratio is
 * the same as the COLS to ROWS ratio.
 */
#define SCREEN_WIDTH 900
#define SCREEN_HEIGHT 900

#define COLS 16
#define ROWS 16

/* It is advised to have around
 * (COLS * ROWS) / 5 BOMBS or mines
 */
#define BOMBS 40

/* Other defines. */
#define BLANC 0
#define FILLED 9
#define BOMB 10
#define FLAG 11

#define ENDED 0
#define ONGOING 1
#define GAME_OVER 2
#define GAME_WON 3

struct game {
    int grid[COLS][ROWS];
    int solution[COLS][ROWS];
    int first_click;
    
    int undiscovered;
    int state;
};

#endif