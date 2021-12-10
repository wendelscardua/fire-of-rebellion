#include "lib/nesdoug.h"
#include "lib/neslib.h"
#include "lib/unrle.h"
#include "mmc3/mmc3_code.h"
#include "cutscene.h"
#include "directions.h"
#include "dungeon.h"
#include "irq_buffer.h"
#include "ends_and_beginnings.h"
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

#define ENERGY_RECOVERY_DELAY (60 * 8)

#define MAX_SPEED FP(0x01, 0x20)
#define ACCELERATION FP(0x00, 0x20)
#define FRICTION FP(0x00, 0x10)

#define MAX_ENTITIES 16

typedef enum
  {
   Fire,
   Patrol,
   Fireball
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
unsigned char has_fire, has_invi;
unsigned int energy_recovery_time;

signed int last_spawn_x, last_spawn_y;

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
unsigned char shooting_cooldown;

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

const unsigned char fire_text[9]=
  {
   0x26,0x4f,0x47,0x4f,0x00,0x1c,0x11,0x05,0x1e
  };

const unsigned char invi_text[9]=
  {
   0x29,0x4e,0x56,0x49,0x00,0x1c,0x15,0x05,0x1e
  };

void load_room(unsigned char *room_ptr);
void refresh_hud();

void init_dungeon () {
  player_x = FP(0x30, 0x00); // TODO: back to 0x30
  player_y = FP(0x40, 0x00);
  last_spawn_x = FP(0x80, 0x00);
  last_spawn_y = FP(0x40, 0x00);
  player_dx = 0x00;
  player_dy = 0x00;
  player_direction = Down;
  player_lives = 3;
  player_energy = 0;
  has_fire = 0;
  has_invi = 0;
  shooting_cooldown = 0;
  energy_recovery_time = 0;
  current_dungeon_mode = Moving;
  load_room((unsigned char*) starting_room);
}

void load_room(unsigned char *room_ptr) {
  current_room_ptr = room_ptr;

  up_room_ptr = *(unsigned char **) room_ptr;
  room_ptr += 2;
  down_room_ptr = *(unsigned char **) room_ptr;
  room_ptr += 2;
  left_room_ptr = *(unsigned char **) room_ptr;
  room_ptr += 2;
  right_room_ptr = *(unsigned char **) room_ptr;
  room_ptr += 2;
  init_cutscene(*(int **) room_ptr);
  room_ptr += 2;

  num_entities = *room_ptr; ++room_ptr;
  for(i = 0; i < num_entities; ++i) {
    entity_type[i] = *room_ptr; ++room_ptr;
    entity_x[i] = entity_target_x[i] = FP(*room_ptr, 0x00), ++room_ptr;
    entity_y[i] = entity_target_y[i] = FP(*room_ptr, 0x00), ++room_ptr;
    entity_dx[i] = entity_dy[i] = entity_direction[i] = entity_patrol_index[i] = 0;

    switch(entity_type[i]) {
    case Fire:
      entity_lives[i] = 0xff;
      break;
    case Patrol:
      entity_lives[i] = 0x01;
      temp = num_entity_patrol_points[i] = *room_ptr; ++room_ptr;
      entity_patrol_points[i] = (patrol_coordinates_t(*)[]) room_ptr;
      room_ptr += (2 * temp);
      break;
    }
  }

  set_unrle_buffer((unsigned char *) room_buffer);
  unrle_to_buffer(room_ptr);

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
  refresh_hud();
  ppu_on_all();
  set_scroll_y(0);
  pal_fade_to(0, 4);
}

void refresh_hud() {
  for(i = 0; i < 3; i++) {
    if (i < player_lives) {
      one_vram_buffer(0x3d, NTADR_C(8 + i, 1));
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

void damage_player() {
  --player_lives;
  refresh_hud();
  player_x = last_spawn_x;
  player_y = last_spawn_y;
  load_room(current_room_ptr);
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
  go_to_good_ending();
}

unsigned char exiting_room() {
  temp_x = INT(player_x);
  temp_y = INT(player_y);
  if (temp_y < 0x08) {
    last_spawn_x = player_x;
    last_spawn_y = player_y = FP(0xa0, 0x80);
    if (up_room_ptr == 0) victory();
    else load_room(up_room_ptr);
    return 1;
  }

  if (temp_y >= 0xa8) {
    last_spawn_x = player_x;
    last_spawn_y = player_y = FP(0x08, 0x80);
    if (down_room_ptr == 0) victory();
    else load_room(down_room_ptr);
    return 1;
  }

  if (temp_x < 0x08) {
    last_spawn_x = player_x = FP(0xef, 0x80);
    last_spawn_y = player_y;
    if (left_room_ptr == 0) victory();
    else load_room(left_room_ptr);
    return 1;
  }

  if (temp_x >= 0xf0) {
    last_spawn_x = player_x = FP(0x08, 0x80);
    last_spawn_y = player_y;
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
    if (has_fire && player_energy > 0 && num_entities < MAX_ENTITIES) {
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

  check_trigger();
}

unsigned char xm, ym;
unsigned char unobstructed_line(unsigned char x1, unsigned char y1, unsigned char x2, unsigned char y2) {
  if (x1 > x2) {
    xm = x1;
    x1 = x2;
    x2 = xm;
  }
  if (y1 > y2) {
    ym = y1;
    y1 = y2;
    y2 = ym;
  }
  if (x2 - x1 < 0x10) {
    xm = (x2 - x1) / 2 + x1;
    for(temp_y = y1; temp_y < y2; temp_y += 16) {
      if (point_room_collision(xm, temp_y)) return 0;
    }
    return 1;
  } else if (y2 - y1 < 0x10) {
    ym = (y2 - y1) / 2 + y1;
    for(temp_x = x1; temp_x < x2; temp_x += 16) {
      if (point_room_collision(temp_x, ym)) return 0;
    }
    return 1;
  }
  return 0;
}

void entities_handler() {
  patrol_coordinates_t coordinates;

  if (shooting_cooldown > 0) --shooting_cooldown;

  for (i = 0; i < num_entities; i++) {
    if (entity_lives[i] == 0) continue;
    switch (entity_type[i]) {
    case Fire:
      if (entity_x[i] >= player_x && entity_x[i] < player_x + FP(0x10, 00) &&
          entity_y[i] >= player_y && entity_y[i] < player_y + FP(0x10, 00)) {
        damage_player();
        break;
      }
      break;
    case Fireball:
      entity_x[i] += entity_dx[i];
      entity_y[i] += entity_dy[i];
      if (entity_x[i] >= player_x && entity_x[i] < player_x + FP(0x10, 00) &&
          entity_y[i] >= player_y && entity_y[i] < player_y + FP(0x10, 00)) {
        entity_lives[i] = 0;
        damage_player();
        break;
      }
      if (entity_x[i] < FP(0x10, 0x00) ||
          entity_x[i] >= FP(0xf0, 0x00) ||
          entity_y[i] < FP(0x10, 0x00) ||
          entity_y[i] >= FP(0xb0, 0x00) ||
          point_room_collision(INT(entity_x[i]), INT(entity_y[i]))) {
        entity_lives[i] = 0;
        break;
      }
      break;
    case Patrol:
      if (entity_x[i] >= player_x && entity_x[i] < player_x + FP(0x10, 00) &&
          entity_y[i] >= player_y && entity_y[i] < player_y + FP(0x10, 00)) {
        damage_player();
        break;
      }
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

      if (shooting_cooldown == 0 &&
          num_entities < MAX_ENTITIES &&
          ((get_frame_count() + i) & 0b011) == 0 &&
          unobstructed_line(INT(player_x) + 8, INT(player_y) + 4, INT(entity_x[i]) + 8, INT(entity_y[i]) + 4)) {
        shooting_cooldown = 24;
        for(temp = 0; temp < num_entities; temp++) {
          if (entity_lives[temp] == 0) break;
        }
        if (temp == num_entities) num_entities++;
        entity_type[temp] = Fireball;
        entity_lives[temp] = 0xff;
        entity_x[temp] = entity_x[i] + 0x08;
        entity_y[temp] = entity_y[i] + 0x08;
        if (player_x > entity_x[i] + FP(0x20, 0x00)) {
          entity_direction[temp] = Right;
        } else if (player_x < entity_x[i] - FP(0x10, 0x00)) {
          entity_direction[temp] = Left;
        } else if (player_y > entity_y[i] + FP(0x20, 0x00)) {
          entity_direction[temp] = Down;
        } else if (player_y < entity_y[i] - FP(0x10, 0x00)) {
          entity_direction[temp] = Up;
        } else {
          entity_direction[temp] = entity_direction[i];
        }

        switch(entity_direction[temp]) {
        case Up:
          entity_dx[temp] = 0;
          entity_dy[temp] = -2 * MAX_SPEED;
          break;
        case Down:
          entity_dx[temp] = 0;
          entity_dy[temp] = 2 * MAX_SPEED;
          break;
        case Left:
          entity_dx[temp] = -2 * MAX_SPEED;
          entity_dy[temp] = 0;
          break;
        case Right:
          entity_dx[temp] = 2 * MAX_SPEED;
          entity_dy[temp] = 0;
          break;
        }
        break;
      }
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

    for(temp = 0; temp < num_entities; temp++) {
      if ((entity_lives[temp] > 0) &&
          (entity_type[temp] == Patrol) &&
          (entity_x[temp] <= player_fire_x[i]) &&
          (player_fire_x[i] < entity_x[temp] + FP(0x10, 0x00)) &&
          (entity_y[temp] <= player_fire_y[i]) &&
          (player_fire_y[i] < entity_y[temp] + FP(0x10, 0x00))) {
        entity_lives[temp]--;
        player_fire_active[i] = 0;
        break;
      }
    }
  }
}

#define MENU_SCANLINE 0xc0

void dungeon_handler() {
  double_buffer[double_buffer_index++] = MENU_SCANLINE - 1;
  double_buffer[double_buffer_index++] = 0xf6;
  double_buffer[double_buffer_index++] = 8;
  if (current_dungeon_mode == Cutscene) {
    temp_int = 0x30;
  } else {
    temp_int = 0x00;
  }
  double_buffer[double_buffer_index++] = temp_int;
  double_buffer[double_buffer_index++] = 0;
  double_buffer[double_buffer_index++] = ((temp_int & 0xF8) << 2);

  if (has_fire && player_energy < 5) {
    if (energy_recovery_time == 0) {
      energy_recovery_time = ENERGY_RECOVERY_DELAY;
      player_energy++;
      refresh_hud();
    } else {
      --energy_recovery_time;
    }
  }

  entities_handler();
  player_fire_handler();
  if (player_lives == 0) {
    go_to_bad_ending();
    return;
  }

  pad_poll(0);
  pad1 = pad_state(0);
  pad1_new = get_pad_new(0);

  switch(current_dungeon_mode) {
  case Moving: dungeon_moving_handler(); break;
  case Cutscene: cutscene_handler(); break;
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
    if (entity_lives[i] == 0) continue;
    switch(entity_type[i]) {
    case Fire:
      oam_meta_spr(INT(entity_x[i]), INT(entity_y[i]) - 1, (const unsigned char *) metasprites_pointers[16]);
      break;
    case Fireball:
      switch(entity_direction[i]) {
      case Up:
        oam_spr(INT(entity_x[i]) - 4, INT(entity_y[i]) - 5, 0x60, 0x01);
        break;
      case Down:
        oam_spr(INT(entity_x[i]) - 4, INT(entity_y[i]) - 5, 0x60, 0x01 | OAM_FLIP_V);
        break;
      case Left:
        oam_spr(INT(entity_x[i]) - 4, INT(entity_y[i]) - 5, 0x50, 0x01 | OAM_FLIP_H);
        break;
      case Right:
        oam_spr(INT(entity_x[i]) - 4, INT(entity_y[i]) - 5, 0x50, 0x01);
        break;
      }
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
  if (current_dungeon_mode == Cutscene) draw_cutscene_sprites();
}
