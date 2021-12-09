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

#pragma bss-name(push, "ZEROPAGE")
unsigned char *current_room_ptr;
unsigned char *up_room_ptr, *down_room_ptr, *left_room_ptr, *right_room_ptr;
#pragma bss-name(pop)
char room_buffer[240];

signed int player_x, player_y;
signed int player_dx, player_dy;
direction_t player_direction;

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

#define MENU_SCANLINE 0xc0

void dungeon_handler() {
  double_buffer[double_buffer_index++] = MENU_SCANLINE - 1;
  double_buffer[double_buffer_index++] = 0xf6;
  double_buffer[double_buffer_index++] = 8;
  temp_int = 0x2000; // TODO: switch to dialogue
  double_buffer[double_buffer_index++] = temp_int;
  double_buffer[double_buffer_index++] = 0;
  double_buffer[double_buffer_index++] = ((temp_int & 0xF8) << 2);
}

void dungeon_draw_sprites() {
  // render player
  temp = 2 * player_direction;
  if (player_dx >= FP(0x02, 0x00) ||
      player_dx <= -FP(0x02, 0x00) ||
      player_dy >= FP(0x02, 0x00) ||
      player_dy <= -FP(0x02, 0x00)) {
    if (get_frame_count() & 0x04) temp++;
  } else if (player_dx >= FP(0x01, 0x00) ||
             player_dx <= -FP(0x01, 0x00) ||
             player_dy >= FP(0x01, 0x00) ||
             player_dy <= -FP(0x01, 0x00)) {
    if (get_frame_count() & 0x10) temp++;
  }
  oam_meta_spr(INT(player_x), INT(player_y), (const unsigned char *) metasprites_pointers[temp]);
}
