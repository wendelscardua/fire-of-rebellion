#ifndef _MAIN_H_
#define _MAIN_H_

typedef enum {
              Title,
              Main,
              GameOver
} game_state_t;

#pragma bss-name(push, "ZEROPAGE")
extern game_state_t current_game_state;
#pragma zpsym("current_game_state")
#pragma bss-name(pop)

#define BG_MAIN_0 0
#define BG_MAIN_1 1
#define BG_MAIN_2 2
#define BG_MAIN_3 3
#define SPRITE_0 4
#define SPRITE_1 6

void go_to_title();

#endif
