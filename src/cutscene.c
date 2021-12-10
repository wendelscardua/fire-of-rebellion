#include "lib/nesdoug.h"
#include "lib/neslib.h"
#include "lib/unrle.h"
#include "mmc3/mmc3_code.h"
#include "directions.h"
#include "dungeon.h"
#include "irq_buffer.h"
#include "temp.h"
#include "main.h"
#include "../assets/metatiles.h"
#include "../assets/sprites.h"
#include "../assets/dialogs.h"

#define FP(integer,fraction) (((integer)<<6)|((fraction)>>2))
#define INT(unsigned_fixed_point) ((unsigned_fixed_point>>6)&0xff)

#pragma bss-name(push, "ZEROPAGE")
int * current_cutscene;
char * current_dialog;
unsigned char current_speaker;
unsigned char dialog_row;
unsigned char dialog_column;
unsigned char npc_x, npc_y, npc_enabled;
unsigned char checklist_index;
unsigned char trigger_x1, trigger_y1, trigger_x2, trigger_y2;
#pragma bss-name(pop)

unsigned char cutscene_checklist[2];

#pragma code-name("CODE")
#pragma rodata-name("RODATA")

typedef enum {
              PutNPC,
              MoveNPC,
              TurnNPC,
              StartDialog,
              DungeonUnlock,
              CloseDialog
} cutscene_command_t;

const int beginning_cutscene[] =
  {
   0,
   0x30, 0x70, 0x50, 0x80,
   PutNPC, 0x78, 0xb0, Up,
   MoveNPC, 0x24,
   TurnNPC, Left,
   MoveNPC, 0x40,
   TurnNPC, Up,
   StartDialog, (int) dialog_getting_key,
   DungeonUnlock,
   CloseDialog
  };

void init_cutscene(int * cutscene) {
  current_cutscene = cutscene;
  if (current_cutscene == 0) return;
  checklist_index = *cutscene; cutscene++;
  trigger_x1 = *cutscene; cutscene++;
  trigger_y1 = *cutscene; cutscene++;
  trigger_x2 = *cutscene; cutscene++;
  trigger_y2 = *cutscene; cutscene++;
}

void check_trigger() {
  if (current_cutscene == 0) return;
  if (INT(player_x) >= trigger_x1 && INT(player_x) <= trigger_x2 &&
      INT(player_y) >= trigger_y1 && INT(player_y) <= trigger_y2) {
    current_dungeon_mode = Cutscene;
  }
}

void cutscene_handler() {
  if (current_cutscene == 0) return;
}
