#ifndef _CUTSCENE_H_
#define _CUTSCENE_H_

void init_cutscene(int * cutscene);
void check_trigger();
void cutscene_handler();
void cutscene_draw_sprites(void);

extern const int beginning_cutscene;

#endif
