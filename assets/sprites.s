.include "../src/constants.inc"
.feature force_range

.segment "RODATA"
metasprites_0_data:

.byte   0,  0,$14,0
.byte   8,  0,$15,0
.byte   0,  8,$24,0
.byte   8,  8,$25,0
.byte $80


metasprites_1_data:


.byte   0,  0,$16,0
.byte   8,  0,$17,0
.byte   0,  8,$26,0
.byte   8,  8,$27,0
.byte $80


metasprites_2_data:


.byte   0,  0,$10,0
.byte   8,  0,$11,0
.byte   0,  8,$20,0
.byte   8,  8,$21,0
.byte $80


metasprites_3_data:


.byte   0,  0,$12,0
.byte   8,  0,$13,0
.byte   0,  8,$22,0
.byte   8,  8,$23,0
.byte $80


metasprites_4_data:


.byte   8,  0,$18,0|OAM_FLIP_H
.byte   0,  0,$19,0|OAM_FLIP_H
.byte   8,  8,$28,0|OAM_FLIP_H
.byte   0,  8,$29,0|OAM_FLIP_H
.byte $80


metasprites_5_data:


.byte   8,  0,$1a,0|OAM_FLIP_H
.byte   0,  0,$1b,0|OAM_FLIP_H
.byte   8,  8,$2a,0|OAM_FLIP_H
.byte   0,  8,$2b,0|OAM_FLIP_H
.byte $80


metasprites_6_data:


.byte   0,  0,$18,0
.byte   8,  0,$19,0
.byte   0,  8,$28,0
.byte   8,  8,$29,0
.byte $80


metasprites_7_data:


.byte   0,  0,$1a,0
.byte   8,  0,$1b,0
.byte   0,  8,$2a,0
.byte   8,  8,$2b,0
.byte $80


metasprites_8_data:


.byte   0,  0,$34,2
.byte   8,  0,$35,2
.byte   0,  8,$44,2
.byte   8,  8,$45,2
.byte $80


metasprites_9_data:


.byte   0,  0,$36,2
.byte   8,  0,$37,2
.byte   0,  8,$46,2
.byte   8,  8,$47,2
.byte $80


metasprites_10_data:


.byte   0,  0,$30,2
.byte   8,  0,$31,2
.byte   0,  8,$40,2
.byte   8,  8,$41,2
.byte $80


metasprites_11_data:


.byte   0,  0,$32,2
.byte   8,  0,$33,2
.byte   0,  8,$42,2
.byte   8,  8,$43,2
.byte $80


metasprites_12_data:


.byte   8,  0,$38,2|OAM_FLIP_H
.byte   0,  0,$39,2|OAM_FLIP_H
.byte   8,  8,$48,2|OAM_FLIP_H
.byte   0,  8,$49,2|OAM_FLIP_H
.byte $80


metasprites_13_data:


.byte   8,  0,$3a,2|OAM_FLIP_H
.byte   0,  0,$3b,2|OAM_FLIP_H
.byte   8,  8,$4a,2|OAM_FLIP_H
.byte   0,  8,$4b,2|OAM_FLIP_H
.byte $80


metasprites_14_data:


.byte   0,  0,$38,2
.byte   8,  0,$39,2
.byte   0,  8,$48,2
.byte   8,  8,$49,2
.byte $80


metasprites_15_data:


.byte   0,  0,$3a,2
.byte   8,  0,$3b,2
.byte   0,  8,$4a,2
.byte   8,  8,$4b,2
.byte $80


metasprites_16_data:


.byte   0,  0,$51,1
.byte   8,  0,$52,1
.byte   0,  8,$61,1
.byte   8,  8,$62,1
.byte $80

metasprites_17_data:

.byte   0,  0,$14,3
.byte   8,  0,$15,3
.byte   0,  8,$24,3
.byte   8,  8,$25,3
.byte $80


metasprites_18_data:


.byte   0,  0,$16,3
.byte   8,  0,$17,3
.byte   0,  8,$26,3
.byte   8,  8,$27,3
.byte $80


metasprites_19_data:


.byte   0,  0,$10,3
.byte   8,  0,$11,3
.byte   0,  8,$20,3
.byte   8,  8,$21,3
.byte $80


metasprites_20_data:


.byte   0,  0,$12,3
.byte   8,  0,$13,3
.byte   0,  8,$22,3
.byte   8,  8,$23,3
.byte $80


metasprites_21_data:


.byte   8,  0,$18,3|OAM_FLIP_H
.byte   0,  0,$19,3|OAM_FLIP_H
.byte   8,  8,$28,3|OAM_FLIP_H
.byte   0,  8,$29,3|OAM_FLIP_H
.byte $80


metasprites_22_data:


.byte   8,  0,$1a,3|OAM_FLIP_H
.byte   0,  0,$1b,3|OAM_FLIP_H
.byte   8,  8,$2a,3|OAM_FLIP_H
.byte   0,  8,$2b,3|OAM_FLIP_H
.byte $80


metasprites_23_data:


.byte   0,  0,$18,3
.byte   8,  0,$19,3
.byte   0,  8,$28,3
.byte   8,  8,$29,3
.byte $80


metasprites_24_data:


.byte   0,  0,$1a,3
.byte   8,  0,$1b,3
.byte   0,  8,$2a,3
.byte   8,  8,$2b,3
.byte $80

.export _metasprites_pointers
_metasprites_pointers:

; player (2 up, 2 down, 2 left, 2 right)
.word metasprites_0_data
.word metasprites_1_data
.word metasprites_2_data
.word metasprites_3_data
.word metasprites_4_data
.word metasprites_5_data
.word metasprites_6_data
.word metasprites_7_data
; patrol (2 up, 2 down, 2 left, 2 right)
.word metasprites_8_data
.word metasprites_9_data
.word metasprites_10_data
.word metasprites_11_data
.word metasprites_12_data
.word metasprites_13_data
.word metasprites_14_data
.word metasprites_15_data
; fire (fire)
.word metasprites_16_data
; npc (2 up, 2 down, 2 left, 2 right)
.word metasprites_17_data
.word metasprites_18_data
.word metasprites_19_data
.word metasprites_20_data
.word metasprites_21_data
.word metasprites_22_data
.word metasprites_23_data
.word metasprites_24_data
