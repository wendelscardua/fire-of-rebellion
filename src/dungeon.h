#ifndef _DUNGEON_H_
#define _DUNGEON_H_

typedef enum
  {
   Cutscene,
   Moving
  } dungeon_mode_t;

extern signed int player_x, player_y;
extern dungeon_mode_t current_dungeon_mode;

void init_dungeon(void);
void dungeon_handler(void);
void dungeon_draw_sprites(void);

#endif
