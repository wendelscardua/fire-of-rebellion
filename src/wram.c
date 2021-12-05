#include "lib/neslib.h"

#pragma bss-name(push, "XRAM")
// extra RAM at $6000-$7fff

#define WRAM_VERSION 0x0001
unsigned int wram_start;

// this one doesn't need to be saved, it's here because of space
// TODO: optimize it away
unsigned char unrle_buffer[1024];

#pragma bss-name(pop)

#pragma code-name ("CODE")

void init_wram (void) {
  if (wram_start != WRAM_VERSION)
    memfill(&wram_start,0,0x2000);
  wram_start = WRAM_VERSION;
}
