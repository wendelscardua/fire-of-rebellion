#include "lib/nesdoug.h"
#include "lib/neslib.h"
#include "lib/unrle.h"
#include "mmc3/mmc3_code.h"
#include "directions.h"
#include "irq_buffer.h"
#include "temp.h"
#include "../assets/palettes.h"
#include "../assets/nametables.h"
#include "../assets/metatiles.h"
#include "../assets/sprites.h"
#include "../assets/dungeon.h"

#define FP(integer,fraction) (((integer)<<8)|((fraction)>>0))
#define INT(unsigned_fixed_point) ((unsigned_fixed_point>>8)&0xff)

#define BG_MAIN_0 0
#define BG_MAIN_1 1
#define BG_MAIN_2 2
#define BG_MAIN_3 3
#define SPRITE_0 4
#define SPRITE_1 6

#define MAX_SPEED FP(0x01, 0x20)
#define ACCELERATION FP(0x00, 0x10)
#define FRICTION FP(0x00, 0x06)

typedef enum
  {
   Cutscene,
   Moving,
   Dialogue,
   Prompt
  } dungeon_mode_t;

#pragma bss-name(push, "ZEROPAGE")

unsigned char *current_room_ptr;
unsigned char *up_room_ptr, *down_room_ptr, *left_room_ptr, *right_room_ptr;

#pragma bss-name(pop)

char room_buffer[240];

signed int player_x, player_y;
signed int player_dx, player_dy;
direction_t player_direction;

dungeon_mode_t current_dungeon_mode;

const char empty_row[32] =
  {
   0, 0, 0, 0, 0, 0, 0, 0,
   0, 0, 0, 0, 0, 0, 0, 0,
   0, 0, 0, 0, 0, 0, 0, 0,
   0, 0, 0, 0, 0, 0, 0, 0
  };

void load_room(unsigned char *room_ptr);

void init_dungeon () {
  load_room((unsigned char*) starting_room);
  player_x = FP(0x30, 0x00);
  player_y = FP(0x40, 0x00);
  player_dx = 0x00;
  player_dy = 0x00;
  player_direction = Down;
  current_dungeon_mode = Moving;
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

  set_chr_mode_2(BG_MAIN_0);
  set_chr_mode_3(BG_MAIN_1);
  set_chr_mode_4(BG_MAIN_2);
  set_chr_mode_5(BG_MAIN_3);
  set_chr_mode_0(SPRITE_0);
  set_chr_mode_1(SPRITE_1);
  ppu_on_all();
  set_scroll_y(0);
  pal_fade_to(0, 4);
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

  player_x += player_dx;
  player_y += player_dy;

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

#define MENU_SCANLINE 0xc0

void dungeon_handler() {
  double_buffer[double_buffer_index++] = MENU_SCANLINE - 1;
  double_buffer[double_buffer_index++] = 0xf6;
  double_buffer[double_buffer_index++] = 8;
  temp_int = 0x2000; // TODO: switch to dialogue
  double_buffer[double_buffer_index++] = temp_int;
  double_buffer[double_buffer_index++] = 0;
  double_buffer[double_buffer_index++] = ((temp_int & 0xF8) << 2);

  pad_poll(0);
  pad1 = pad_state(0);
  pad1_new = get_pad_new(0);

  switch(current_dungeon_mode) {
  case Moving: dungeon_moving_handler(); break;
  }
}

#define FAST_SPEED MAX_SPEED
#define NORMAL_SPEED (MAX_SPEED / 4)

void dungeon_draw_sprites() {
  // render player
  temp = 2 * player_direction;
  if (player_dx >= FAST_SPEED ||
      player_dx <= -FAST_SPEED ||
      player_dy >= FAST_SPEED ||
      player_dy <= -FAST_SPEED) {
    if (get_frame_count() & 0x04) temp++;
  } else if (player_dx >= FP(0x00, 0x80) ||
             player_dx <= -FP(0x00, 0x80) ||
             player_dy >= FP(0x00, 0x80) ||
             player_dy <= -FP(0x00, 0x80)) {
    if (get_frame_count() & 0x08) temp++;
  }
  oam_meta_spr(INT(player_x), INT(player_y) - 1, (const unsigned char *) metasprites_pointers[temp]);
}
