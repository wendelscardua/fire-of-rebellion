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

#define READ (*(current_cutscene++))

#pragma bss-name(push, "ZEROPAGE")
int * current_cutscene;
char * current_dialog;

unsigned char cutscene_aux;

unsigned char current_speaker;
unsigned char dialog_row;
unsigned char dialog_column;
unsigned char npc_x, npc_y, npc_direction, npc_enabled;
unsigned char checklist_index;
unsigned char trigger_x1, trigger_y1, trigger_x2, trigger_y2;
#pragma bss-name(pop)

unsigned char cutscene_checklist[2];

#pragma code-name("CODE")
#pragma rodata-name("RODATA")

typedef enum {
              NoCommand,
              PutNPC,
              RemoveNPC,
              MoveNPC,
              TurnNPC,
              StartDialog,
              DungeonUnlock,
              CloseDialog,
              QuitCutscene
} cutscene_command_t;

cutscene_command_t current_cutscene_command;

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
   TurnNPC, Right,
   MoveNPC, 0x40,
   TurnNPC, Up,
   MoveNPC, 0x90,
   RemoveNPC,
   DungeonUnlock,
   QuitCutscene
  };

void init_cutscene(int * cutscene) {
  current_cutscene = cutscene;
  if (current_cutscene == 0) return;
  checklist_index = READ;
  trigger_x1 = READ;
  trigger_y1 = READ;
  trigger_x2 = READ;
  trigger_y2 = READ;
  current_cutscene_command = NoCommand;
}

void check_trigger() {
  if (current_cutscene == 0) return;
  if (cutscene_checklist[checklist_index]) return;
  if (INT(player_x) >= trigger_x1 && INT(player_x) <= trigger_x2 &&
      INT(player_y) >= trigger_y1 && INT(player_y) <= trigger_y2) {
    current_dungeon_mode = Cutscene;
  }
}
void cutscene_handler() {
  if (current_cutscene == 0) return;
  switch(current_cutscene_command) {
  case NoCommand:
    // fetch next command
    current_cutscene_command = READ;
    switch(current_cutscene_command) {
    case NoCommand:
      break;
    case PutNPC:
      npc_enabled = 1;
      npc_x = READ;
      npc_y = READ;
      npc_direction = READ;
      current_cutscene_command = NoCommand;
      break;
    case RemoveNPC:
      npc_enabled = 0;
      current_cutscene_command = NoCommand;
      break;
    case MoveNPC:
      cutscene_aux = READ;
      break;
    case TurnNPC:
      npc_direction = READ;
      current_cutscene_command = NoCommand;
      break;
    case StartDialog:
      // TODO
      break;
    case DungeonUnlock:
      // TODO - specific for first room
      break;
    case QuitCutscene:
      current_cutscene = 0;
      break;
    }
    break;
  case MoveNPC:
    if (cutscene_aux == 0) {
      current_cutscene_command = NoCommand;
    } else {
      --cutscene_aux;
      switch(npc_direction) {
      case Up: --npc_y; break;
      case Down: ++npc_y; break;
      case Left: --npc_x; break;
      case Right: ++npc_x; break;
      }
    }
    break;
  case StartDialog:
    break;
  }
}

void draw_cutscene_sprites() {
  // render npc
  if (npc_enabled) {
    temp = 17 + 2 * npc_direction;
    if (current_cutscene_command == MoveNPC) {
      if ((npc_x ^ npc_y) & 0b01000) temp++;
    }
    oam_meta_spr(npc_x, npc_y - 1, (const unsigned char *) metasprites_pointers[temp]);
  }
}
