#include "lib/nesdoug.h"
#include "lib/neslib.h"
#include "lib/unrle.h"
#include "music/soundtrack.h"
#include "mmc3/mmc3_code.h"
#include "irq_buffer.h"
#include "main.h"
#include "temp.h"
#include "wram.h"
#include "ends_and_beginnings.h"
#include "../assets/nametables.h"
#include "../assets/palettes.h"
#include "../assets/sprites.h"

#pragma code-name ("CODE")
#pragma rodata-name ("RODATA")

#pragma bss-name (push, "ZEROPAGE")
int text_wall_scroll;
unsigned char scroll_delay;
#pragma bss-name (pop)

void start_text_wall(unsigned char * nametable) {
  if (irq_array[0] != 0xff) {
    while(!is_irq_done() ){}
    irq_array[0] = 0xff;
    double_buffer[0] = 0xff;
  }

  clear_vram_buffer();

  pal_fade_to(4, 0);
  ppu_off();
  vram_adr(NTADR_A(0,0));
  vram_unrle(empty_nametable);
  vram_adr(NTADR_C(0,0));
  vram_unrle(nametable);
  music_stop();

  set_scroll_x(0);
  set_scroll_y(0);

  oam_clear();

  set_chr_mode_2(BG_MAIN_0);
  set_chr_mode_3(BG_MAIN_1);
  set_chr_mode_4(BG_MAIN_2);
  set_chr_mode_5(BG_MAIN_3);
  set_chr_mode_0(SPRITE_0);
  set_chr_mode_1(SPRITE_1);

  pal_bg(bg_palette);
  pal_spr(sprites_palette);

  ppu_on_all();
  pal_fade_to(0, 4);
  text_wall_scroll = 0;
  scroll_delay = 0;
}

void go_to_bad_ending() {
  current_game_state = GameOver;
  start_text_wall(bad_ending_nametable);
  music_play(Cave);
}

void go_to_good_ending() {
  current_game_state = GameOver;
  start_text_wall(good_ending_nametable);
  music_play(VictoryLoop);
}

void go_to_prologue() {
  current_game_state = Prologue;
  start_text_wall(prologue_nametable);
  music_play(Cave);
}

#define DELAY 2
void text_wall_handler() {
  if (scroll_delay == 0) {
    if (text_wall_scroll < 0x100) {
      text_wall_scroll = add_scroll_y(1, text_wall_scroll);
      set_scroll_y(text_wall_scroll);
    }
    scroll_delay = DELAY;
  } else {
    --scroll_delay;
  }

  pad_poll(0);

  pad1_new = get_pad_new(0);

  if (pad1_new & (PAD_START | PAD_A)) {
    if (text_wall_scroll < 0x100) {
      text_wall_scroll = 0x100;
      set_scroll_y(text_wall_scroll);
    } else {
      switch(current_game_state) {
      case Prologue:
        start_game();
        break;
      case GameOver:
        go_to_title();
        break;
      }

    }
    return;
  }
}
