#ifndef _TRIG_H_
#define _TRIG_H_

#pragma bss-name(push, "ZEROPAGE")
extern unsigned char atan2_x1, atan2_x2, atan2_y1, atan2_y2;
#pragma zpsym("atan2_x1")
#pragma zpsym("atan2_x2")
#pragma zpsym("atan2_y1")
#pragma zpsym("atan2_y2")
#pragma bss-name(pop)

#pragma code-name(push, "STARTUP")
unsigned char __fastcall__ atan2();
#pragma code-name(pop)

#endif
