#include "lib/nesdoug.h"
#include "lib/neslib.h"
#include "lib/unrle.h"
#include "mmc3/mmc3_code.h"
#include "directions.h"
#include "dungeon.h"
#include "irq_buffer.h"
#include "temp.h"
#include "main.h"
#include  "charmap.h"
#include "../assets/metatiles.h"
#include "../assets/sprites.h"
#include "../assets/dialogs.h"

#define NUM_CUTSCENES 1

#define FIRST_DIALOG_COLUMN 5
#define FIRST_DIALOG_ROW 7
#define LAST_DIALOG_COLUMN 29
#define LAST_DIALOG_ROW 9

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
unsigned char yes_no_state;
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
              YesNoPrompt,
              DungeonUnlock,
              InviUnlock,
              QuitCutscene
} cutscene_command_t;

cutscene_command_t current_cutscene_command;

const char yes_no_text[] = "Sim    Nao";

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

const int helping_cutscene[] =
  {
   1,
   0x70, 0x60, 0x90, 0x74,
   PutNPC, 0x80, 0x68, Right,
   StartDialog, (int) dialog_needs_help,
   YesNoPrompt,
   StartDialog, (int) dialog_helping,
   InviUnlock,
   QuitCutscene
  };

void reset_cutscenes() {
  for(i = 0; i < NUM_CUTSCENES; i++) {
    cutscene_checklist[i] = 0;
  }
}

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

extern const char empty_row[];

void clean_dialog_window() {
  multi_vram_buffer_horz(empty_row, 25, NTADR_C(5, 7));
  multi_vram_buffer_horz(empty_row, 25, NTADR_C(5, 8));
  multi_vram_buffer_horz(empty_row, 25, NTADR_C(5, 9));
  dialog_row = FIRST_DIALOG_ROW;
  dialog_column = FIRST_DIALOG_COLUMN;
}

void check_trigger() {
  if (current_cutscene == 0) return;
  if (cutscene_checklist[checklist_index]) return;
  if (INT(player_x) >= trigger_x1 && INT(player_x) <= trigger_x2 &&
      INT(player_y) >= trigger_y1 && INT(player_y) <= trigger_y2) {
    clean_dialog_window();
    current_dungeon_mode = Cutscene;
  }
}

void dialog_handler() {
  temp = *current_dialog;
  if (temp == 0) {
    if (pad1_new & PAD_A) {
      clean_dialog_window();
      current_cutscene_command = NoCommand;
    }
  } else if (temp <= 2) {
    if (temp != current_speaker) {
      if (pad1_new & PAD_A) {
        current_speaker = temp;
        clean_dialog_window();
        ++current_dialog;
      }
    } else {
      ++current_dialog;
    }
  } else if (temp <= 4) {
    ++dialog_column;
    ++current_dialog;
  } else {
    temp = 0;
    while(current_dialog[temp] > 4) temp++;
    if (dialog_column + temp > LAST_DIALOG_COLUMN) {
      if (dialog_row == LAST_DIALOG_ROW) {
        if (pad1_new & PAD_A) {
          clean_dialog_window();
        }
      } else {
        ++dialog_row;
        dialog_column = FIRST_DIALOG_COLUMN;
      }
    } else {
      clear_vram_buffer();
      multi_vram_buffer_horz(current_dialog, temp, NTADR_C(dialog_column, dialog_row));
      ppu_wait_nmi();
      dialog_column += temp;
      current_dialog += temp;
    }
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
      current_dialog = (char *) READ;
      dialog_row = FIRST_DIALOG_ROW;
      dialog_column = FIRST_DIALOG_COLUMN;
      current_speaker = *current_dialog;
      break;
    case YesNoPrompt:
      multi_vram_buffer_horz(yes_no_text, 10, NTADR_C(11, 9));
      yes_no_state = 0;
      break;
    case DungeonUnlock:
      unlock_dungeon();
      current_cutscene_command = NoCommand;
      break;
    case InviUnlock:
      unlock_invi();
      current_cutscene_command = NoCommand;
      break;
    case QuitCutscene:
      current_cutscene = 0;
      current_dungeon_mode = Moving;
      cutscene_checklist[checklist_index] = 1;
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
    dialog_handler();
    break;
  case YesNoPrompt:
    if (pad1_new & (PAD_LEFT | PAD_RIGHT | PAD_UP | PAD_DOWN | PAD_SELECT)) {
      yes_no_state = !yes_no_state;
    } else if (pad1_new & PAD_A) {
      clean_dialog_window();
      if (yes_no_state == 0) {
        current_cutscene_command = NoCommand;
      } else {
        // quit scene (TODO: maybe allow branching?)
        current_cutscene = 0;
        current_dungeon_mode = Moving;
        cutscene_checklist[checklist_index] = 1;
      }
    }
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
  if (current_cutscene_command == StartDialog) {
    if (current_speaker == 1) {
      oam_meta_spr(0x14, 0xcc, (const unsigned char *) metasprites_pointers[2]);
    } else {
      oam_meta_spr(0x14, 0xcc, (const unsigned char *) metasprites_pointers[19]);
    }
  } else if (current_cutscene_command == YesNoPrompt) {
    if (yes_no_state == 0) {
      oam_spr(0x50, 0xd7, 0x50, 0x02);
    } else {
      oam_spr(0x87, 0xd7, 0x50, 0x02);
    }
  }
}
