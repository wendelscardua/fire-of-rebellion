#ifndef _CUTSCENE_H_
#define _CUTSCENE_H_

void reset_cutscenes();
void init_cutscene(int * cutscene);
void check_trigger();
void cutscene_handler();
void draw_cutscene_sprites();

extern const int beginning_cutscene;

#endif
