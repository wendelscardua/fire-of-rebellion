#include "lib/nesdoug.h"
#include "lib/neslib.h"
#include "lib/unrle.h"
#include "mmc3/mmc3_code.h"
#include "directions.h"
#include "dungeon.h"
#include "irq_buffer.h"
#include "temp.h"
#include "../assets/palettes.h"
#include "../assets/nametables.h"
#include "../assets/metatiles.h"
#include "../assets/sprites.h"
#include "../assets/dungeon.h"

#define FP(integer,fraction) (((integer)<<6)|((fraction)>>2))
#define INT(unsigned_fixed_point) ((unsigned_fixed_point>>6)&0xff)

#define BG_MAIN_0 0
#define BG_MAIN_1 1
#define BG_MAIN_2 2
#define BG_MAIN_3 3
#define SPRITE_0 4
#define SPRITE_1 6

#define MAX_SPEED FP(0x01, 0x20)
#define ACCELERATION FP(0x00, 0x20)
#define FRICTION FP(0x00, 0x10)

#define MAX_ENTITIES 16

typedef enum
  {
   Cutscene,
   Moving,
   Dialogue,
   Prompt
  } dungeon_mode_t;

typedef enum
  {
   Fire,
   Patrol
  } entity_t;

typedef struct
{
  unsigned char x;
  unsigned char y;
} patrol_coordinates_t;
#pragma bss-name(push, "ZEROPAGE")

unsigned char *current_room_ptr;
unsigned char *up_room_ptr, *down_room_ptr, *left_room_ptr, *right_room_ptr;
patrol_coordinates_t (*entity_patrol_points[MAX_ENTITIES])[];

#pragma bss-name(pop)

char room_buffer[240];

signed int player_x, player_y;
signed int player_dx, player_dy;
direction_t player_direction;
unsigned char player_lives, player_energy;

dungeon_mode_t current_dungeon_mode;

unsigned char num_entities;
entity_t entity_type[MAX_ENTITIES];
signed int entity_x[MAX_ENTITIES], entity_y[MAX_ENTITIES];
signed int entity_dx[MAX_ENTITIES], entity_dy[MAX_ENTITIES];
signed int entity_target_x[MAX_ENTITIES], entity_target_y[MAX_ENTITIES];
unsigned char entity_lives[MAX_ENTITIES];
direction_t entity_direction[MAX_ENTITIES];
unsigned char num_entity_patrol_points[MAX_ENTITIES];
unsigned char entity_patrol_index[MAX_ENTITIES];

#define MAX_PLAYER_FIRE 5
signed int player_fire_x[MAX_PLAYER_FIRE], player_fire_y[MAX_PLAYER_FIRE];
signed int player_fire_dx[MAX_PLAYER_FIRE], player_fire_dy[MAX_PLAYER_FIRE];
direction_t player_fire_direction[MAX_PLAYER_FIRE];
unsigned char player_fire_active[MAX_PLAYER_FIRE];

const char empty_row[32] =
  {
   0, 0, 0, 0, 0, 0, 0, 0,
   0, 0, 0, 0, 0, 0, 0, 0,
   0, 0, 0, 0, 0, 0, 0, 0,
   0, 0, 0, 0, 0, 0, 0, 0
  };

void load_room(unsigned char *room_ptr);
void refresh_hud();

void init_dungeon () {
  load_room((unsigned char*) starting_room);
  player_x = FP(0x80, 0x00); // TODO: back to 0x30
  player_y = FP(0x40, 0x00);
  player_dx = 0x00;
  player_dy = 0x00;
  player_direction = Down;
  player_lives = 3;
  player_energy = 5;
  current_dungeon_mode = Moving;
  refresh_hud();
}

void load_room(unsigned char *room_ptr) {
  current_room_ptr = room_ptr;

  up_room_ptr = *(unsigned char **) current_room_ptr;
  current_room_ptr += 2;
  down_room_ptr = *(unsigned char **) current_room_ptr;
  current_room_ptr += 2;
  left_room_ptr = *(unsigned char **) current_room_ptr;
  current_room_ptr += 2;
  right_room_ptr = *(unsigned char **) current_room_ptr;
  current_room_ptr += 2;

  num_entities = *current_room_ptr; ++current_room_ptr;
  for(i = 0; i < num_entities; ++i) {
    entity_type[i] = *current_room_ptr; ++current_room_ptr;
    entity_x[i] = entity_target_x[i] = FP(*current_room_ptr, 0x00), ++current_room_ptr;
    entity_y[i] = entity_target_y[i] = FP(*current_room_ptr, 0x00), ++current_room_ptr;
    entity_dx[i] = entity_dy[i] = entity_direction[i] = entity_patrol_index[i] = 0;

    switch(entity_type[i]) {
    case Fire:
      entity_lives[i] = 0xff;
      break;
    case Patrol:
      entity_lives[i] = 0x01;
      temp = num_entity_patrol_points[i] = *current_room_ptr; ++current_room_ptr;
      entity_patrol_points[i] = (patrol_coordinates_t(*)[]) current_room_ptr;
      current_room_ptr += (2 * temp);
      break;
    }
  }

  set_unrle_buffer((unsigned char *) room_buffer);
  unrle_to_buffer(current_room_ptr);

  pal_fade_to(4, 0);
  ppu_off();

  pal_bg(bg_palette);
  pal_spr(sprites_palette);

  // draw some things
  vram_adr(NTADR_C(0,0));
  vram_unrle(hud_nametable);
  vram_adr(NTADR_A(0,0));

  set_mt_pointer(metatiles);
  set_data_pointer(room_buffer);

  temp_int = 0x2000;
  temp = 0;
  for(temp_y = 0; temp_y < 12; temp_y+=2) {
    for(temp_x = 0; temp_x < 16; temp_x+=2) {
      temp_int = 0x2000 + 2 * temp_x + 0x40 * temp_y;
      buffer_4_mt(temp_int, (temp_y << 4) | temp_x);
      flush_vram_update_nmi();
      clear_vram_buffer();
    }
  }

  for(temp_y = 24; temp_y < 30; temp_y++) {
    multi_vram_buffer_horz(empty_row, 32, NTADR_A(0, temp_y));
    flush_vram_update_nmi();
    clear_vram_buffer();
  }

  for(i = 0; i < MAX_PLAYER_FIRE; i++) {
    player_fire_active[i] = 0;
  }

  set_chr_mode_2(BG_MAIN_0);
  set_chr_mode_3(BG_MAIN_1);
  set_chr_mode_4(BG_MAIN_2);
  set_chr_mode_5(BG_MAIN_3);
  set_chr_mode_0(SPRITE_0);
  set_chr_mode_1(SPRITE_1);
  oam_clear();
  dungeon_draw_sprites();
  ppu_on_all();
  set_scroll_y(0);
  pal_fade_to(0, 4);
}

void refresh_hud() {
  for(i = 0; i < 3; i++) {
    if (i < player_lives) {
      one_vram_buffer(0x04, NTADR_C(8 + i, 1));
    } else {
      one_vram_buffer(0x00, NTADR_C(8 + i, 1));
    }
  }
  for(i = 0; i < 5; i++) {
    if (i < player_energy) {
      one_vram_buffer(0x05, NTADR_C(8 + i, 2));
    } else {
      one_vram_buffer(0x00, NTADR_C(8 + i, 2));
    }
  }
}

unsigned char point_room_collision(unsigned char x, unsigned char y) {
  temp_char = room_buffer[(y & 0xf0) | (x >> 4)];
  // 0 is ground, 1 is front wall, 2 is top wall and 3 is iron bar
  return (temp_char != 0);
}

unsigned char player_room_collision(unsigned char x, unsigned char y) {
  return point_room_collision(x, y + 8) ||
    point_room_collision(x + 15, y + 8) ||
    point_room_collision(x, y + 15) ||
    point_room_collision(x + 15, y + 15);
}

void victory() {
  // player escaped dungeon, show victory screen
}

unsigned char exiting_room() {
  temp_x = INT(player_x);
  temp_y = INT(player_y);
  if (temp_y < 0x08) {
    player_y = FP(0xa0, 0x80);
    if (up_room_ptr == 0) victory();
    else load_room(up_room_ptr);
    return 1;
  }

  if (temp_y >= 0xa8) {
    player_y = FP(0x08, 0x80);
    if (down_room_ptr == 0) victory();
    else load_room(down_room_ptr);
    return 1;
  }

  if (temp_x < 0x08) {
    player_x = FP(0xef, 0x80);
    if (left_room_ptr == 0) victory();
    else load_room(left_room_ptr);
    return 1;
  }

  if (temp_x >= 0xf0) {
    player_x = FP(0x08, 0x80);
    if (right_room_ptr == 0) victory();
    else load_room(right_room_ptr);
    return 1;
  }

  return 0;
}

void dungeon_moving_handler() {
  if (pad1 & PAD_UP) {
    player_dx = 0;
    player_dy -= ACCELERATION;
    player_direction = Up;
    if (player_dy < -MAX_SPEED) {
      player_dy = -MAX_SPEED;
    }
  }
  if (pad1 & PAD_DOWN) {
    player_dx = 0;
    player_direction = Down;
    player_dy += ACCELERATION;
    if (player_dy > MAX_SPEED) {
      player_dy = MAX_SPEED;
    }
  }
  if (pad1 & PAD_LEFT) {
    player_dy = 0;
    player_dx -= ACCELERATION;
    player_direction = Left;
    if (player_dx < -MAX_SPEED) {
      player_dx = -MAX_SPEED;
    }
  }
  if (pad1 & PAD_RIGHT) {
    player_dy = 0;
    player_direction = Right;
    player_dx += ACCELERATION;
    if (player_dx > MAX_SPEED) {
      player_dx = MAX_SPEED;
    }
  }
  if (pad1_new & PAD_A) {
    if (player_energy > 0 && num_entities < MAX_ENTITIES) {
      --player_energy;
      refresh_hud();
      // spawn new player fire
      for(i = 0; i < MAX_PLAYER_FIRE; i++) {
        if (!player_fire_active[i]) break;
      }
      if (i == MAX_PLAYER_FIRE) i = 0;
      player_fire_x[i] = player_x + player_dx + FP(0x08, 0x00);
      player_fire_y[i] = player_y + player_dy + FP(0x08, 0x00);
      switch(player_direction) {
      case Up:
        player_fire_dx[i] = 0;
        player_fire_dy[i] = -4 * MAX_SPEED;
        break;
      case Down:
        player_fire_dx[i] = 0;
        player_fire_dy[i] = 4 * MAX_SPEED;
        break;
      case Left:
        player_fire_dx[i] = -4 * MAX_SPEED;
        player_fire_dy[i] = 0;
        break;
      case Right:
        player_fire_dx[i] = 4 * MAX_SPEED;
        player_fire_dy[i] = 0;
        break;
      }
      player_fire_direction[i] = player_direction;
      player_fire_active[i] = 1;
    }
  }

  player_x += player_dx;
  player_y += player_dy;

  if (exiting_room()) return;

  if (player_room_collision(INT(player_x), INT(player_y))) {
    player_x -= player_dx;
    player_y -= player_dy;
    player_dx = player_dy = 0;
  }

  if (player_dy > 0) {
    player_dy -= FRICTION;
    if (player_dy < 0) {
      player_dy = 0;
    }
  }
  if (player_dy < 0) {
    player_dy += FRICTION;
    if (player_dy > 0) {
      player_dy = 0;
    }
  }
  if (player_dx > 0) {
    player_dx -= FRICTION;
    if (player_dx < 0) {
      player_dx = 0;
    }
  }
  if (player_dx < 0) {
    player_dx += FRICTION;
    if (player_dx > 0) {
      player_dx = 0;
    }
  }
}

void entities_handler() {
  patrol_coordinates_t coordinates;

  for (i = 0; i < num_entities; i++) {
    switch (entity_type[i]) {
    case Fire:
      break;
    case Patrol:
      if (entity_x[i] == entity_target_x[i] && entity_y[i] == entity_target_y[i]) {
        temp = entity_patrol_index[i];
        ++temp;
        if (temp >= num_entity_patrol_points[i]) {
          temp = 0;
        }
        entity_patrol_index[i] = temp;
        coordinates = (*entity_patrol_points[i])[temp];
        entity_target_x[i] = FP(coordinates.x, 0x00);
        entity_target_y[i] = FP(coordinates.y, 0x00);
        entity_dx[i] = entity_target_x[i] - entity_x[i];
        entity_dy[i] = entity_target_y[i] - entity_y[i];
#define MAX_PATROL_SPEED FP(0x00, 0xC0)
        while(entity_dx[i] > MAX_PATROL_SPEED ||
              entity_dx[i] < -MAX_PATROL_SPEED ||
              entity_dy[i] > MAX_PATROL_SPEED ||
              entity_dy[i] < -MAX_PATROL_SPEED) {
          if (entity_dx[i] > 1 || entity_dx[i] < -1) {
            entity_dx[i] /= 2;
          }
          if (entity_dy[i] > 1 || entity_dy[i] < -1) {
            entity_dy[i] /= 2;
          }
        }
        if (entity_dy[i] > entity_dx[i] && entity_dy[i] > -entity_dx[i]) {
          entity_direction[i] = Down;
        }
        if (entity_dy[i] < entity_dx[i] && entity_dy[i] < -entity_dx[i]) {
          entity_direction[i] = Up;
        }
        if (entity_dx[i] > entity_dy[i] && entity_dx[i] > -entity_dy[i]) {
          entity_direction[i] = Right;
        }
        if (entity_dx[i] < entity_dy[i] && entity_dx[i] < -entity_dy[i]) {
          entity_direction[i] = Left;
        }
      } else {
        entity_x[i] += entity_dx[i];
        entity_y[i] += entity_dy[i];

        if ((entity_dx[i] > 0 && entity_x[i] >= entity_target_x[i]) ||
            (entity_dx[i] < 0 && entity_x[i] <= entity_target_x[i]) ) {
          entity_dx[i] = 0;
          entity_x[i] = entity_target_x[i];
        }

        if ((entity_dy[i] > 0 && entity_y[i] >= entity_target_y[i]) ||
            (entity_dy[i] < 0 && entity_y[i] <= entity_target_y[i]) ) {
          entity_dy[i] = 0;
          entity_y[i] = entity_target_y[i];
        }
      }
      break;
    }
  }
}

void player_fire_handler() {
  for(i = 0; i < MAX_PLAYER_FIRE; i++) {
    if (!player_fire_active[i]) continue;

    player_fire_x[i] += player_fire_dx[i];
    player_fire_y[i] += player_fire_dy[i];

    if (player_fire_x[i] < FP(0x10, 0x00) ||
        player_fire_x[i] >= FP(0xf0, 0x00) ||
        player_fire_y[i] < FP(0x10, 0x00) ||
        player_fire_y[i] >= FP(0xb0, 0x00)) {
      player_fire_active[i] = 0;
      continue;
    }

    if (point_room_collision(INT(player_fire_x[i]), INT(player_fire_y[i]))) {
      player_fire_active[i] = 0;
      continue;
    }
  }
}

#define MENU_SCANLINE 0xc0

void dungeon_handler() {
  double_buffer[double_buffer_index++] = MENU_SCANLINE - 1;
  double_buffer[double_buffer_index++] = 0xf6;
  double_buffer[double_buffer_index++] = 8;
  temp_int = 0x2000; // TODO: switch to dialogue
  double_buffer[double_buffer_index++] = temp_int;
  double_buffer[double_buffer_index++] = 0;
  double_buffer[double_buffer_index++] = ((temp_int & 0xF8) << 2);

  entities_handler();
  player_fire_handler();

  pad_poll(0);
  pad1 = pad_state(0);
  pad1_new = get_pad_new(0);

  switch(current_dungeon_mode) {
  case Moving: dungeon_moving_handler(); break;
  }
}

void dungeon_draw_sprites() {
  // render player
  temp = 2 * player_direction;
  if (player_dx != 0 || player_dy != 0) {
    if ((INT(player_x) ^ INT(player_y)) & 0b01000) temp++;
  }
  oam_meta_spr(INT(player_x), INT(player_y) - 1, (const unsigned char *) metasprites_pointers[temp]);

  for(i = 0; i < num_entities; i++) {
    switch(entity_type[i]) {
    case Fire:
      oam_meta_spr(INT(entity_x[i]), INT(entity_y[i]) - 1, (const unsigned char *) metasprites_pointers[16]);
      break;
    case Patrol:
      temp = 8 + 2 * entity_direction[i];
      if (entity_dx[i] != 0 || entity_dy[i] != 0) {
        if ((INT(entity_x[i]) ^ INT(entity_y[i])) & 0b01000) temp++;
      }
      oam_meta_spr(INT(entity_x[i]), INT(entity_y[i]) - 1, (const unsigned char *) metasprites_pointers[temp]);

      break;
    }
  }
  for(i = 0; i < MAX_PLAYER_FIRE; i++) {
    if (player_fire_active[i]) {
      switch(player_fire_direction[i]) {
      case Up:
        oam_spr(INT(player_fire_x[i]) - 4, INT(player_fire_y[i]) - 5, 0x60, 0x01);
        break;
      case Down:
        oam_spr(INT(player_fire_x[i]) - 4, INT(player_fire_y[i]) - 5, 0x60, 0x01 | OAM_FLIP_V);
        break;
      case Left:
        oam_spr(INT(player_fire_x[i]) - 4, INT(player_fire_y[i]) - 5, 0x50, 0x01 | OAM_FLIP_H);
        break;
      case Right:
        oam_spr(INT(player_fire_x[i]) - 4, INT(player_fire_y[i]) - 5, 0x50, 0x01);
        break;
      }
    }
  }
}
