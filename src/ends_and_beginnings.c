#include "lib/nesdoug.h"
#include "lib/neslib.h"
#include "lib/unrle.h"
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

void go_to_game_over() {
  current_game_state = GameOver;
  if (irq_array[0] != 0xff) {
    while(!is_irq_done() ){}
    irq_array[0] = 0xff;
    double_buffer[0] = 0xff;
  }

  clear_vram_buffer();

  pal_fade_to(4, 0);
  ppu_off(); // screen off
  // draw some things
  vram_adr(NTADR_A(0,0));
  vram_unrle(title_nametable);
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
}

void game_over_handler() {
  pad_poll(0);

  pad1_new = get_pad_new(0);

  if (pad1_new & (PAD_START | PAD_A)) {
    go_to_title();
    return;
  }
}
