.enum entity_type
   Fire
   Patrol
.endenum
.segment "RODATA"
.export _starting_room
_starting_room: .word room_01_tmx
room_01_tmx:
.word room_03_tmx
.word 0
.word 0
.word room_02_tmx
.byte 2, entity_type::Fire, 112, 176, entity_type::Fire, 128, 168
.byte 4, 2, 4, 6, 0, 0, 2, 4, 7, 1, 1, 3, 1, 1, 2, 0, 0, 1, 2, 1, 3, 3, 1, 2, 2, 0, 4, 4, 2, 0, 4, 2, 2, 0, 4, 3, 2, 2, 0, 4, 4, 2, 0, 4, 2, 2, 0, 4, 3, 2, 2, 0, 4, 4, 2, 0, 4, 2, 2, 0, 4, 3, 1, 2, 0, 4, 4, 2, 0, 4, 2, 1, 0, 4, 4, 2, 0, 4, 4, 2, 0, 4, 8, 2, 4, 2, 0, 0, 2, 2, 0, 4, 7, 2, 2, 1, 1, 3, 3, 1, 1, 0, 4, 7, 2, 2, 0, 4, 13, 2, 4, 7, 0, 0, 2, 4, 6, 1, 4, 2, 3, 1, 4, 2, 0, 0, 1, 4, 3, 3, 1, 4, 1, 4, 0
room_02_tmx:
.word room_04_tmx
.word 0
.word room_01_tmx
.word 0
.byte 3, entity_type::Fire, 80, 144, entity_type::Fire, 216, 144, entity_type::Patrol, 160, 112, 9, 160, 112, 160, 144, 192, 144, 192, 64, 112, 64, 112, 32, 112, 64, 176, 64, 176, 112
.byte 4, 2, 4, 6, 0, 0, 2, 4, 7, 1, 1, 3, 1, 4, 2, 0, 0, 1, 3, 1, 1, 3, 1, 2, 2, 0, 4, 13, 2, 2, 0, 4, 13, 2, 1, 0, 4, 13, 2, 0, 4, 3, 2, 2, 0, 0, 2, 4, 2, 0, 0, 2, 4, 2, 0, 4, 3, 2, 1, 0, 0, 1, 2, 1, 0, 0, 1, 1, 2, 2, 0, 4, 2, 2, 0, 4, 3, 2, 0, 4, 4, 2, 2, 0, 4, 2, 2, 0, 4, 3, 2, 0, 4, 4, 2, 2, 0, 4, 2, 2, 0, 4, 3, 2, 0, 4, 4, 2, 4, 16, 1, 4, 2, 3, 1, 4, 2, 3, 3, 1, 4, 3, 3, 1, 4, 1, 4, 0
room_03_tmx:
.word 0
.word room_01_tmx
.word 0
.word room_04_tmx
.byte 1, entity_type::Patrol, 24, 40, 4, 24, 40, 216, 40, 216, 96, 24, 96
.byte 4, 2, 4, 16, 1, 4, 2, 3, 1, 4, 5, 3, 3, 1, 1, 2, 2, 0, 4, 13, 2, 2, 0, 4, 13, 2, 2, 0, 0, 2, 4, 9, 0, 0, 1, 2, 0, 0, 1, 4, 9, 0, 4, 2, 2, 0, 4, 14, 2, 0, 0, 2, 4, 9, 0, 0, 2, 2, 0, 0, 1, 4, 9, 0, 0, 2, 2, 0, 4, 13, 2, 4, 7, 0, 0, 2, 4, 6, 1, 1, 3, 1, 4, 3, 0, 0, 1, 4, 3, 3, 1, 4, 1, 4, 0
room_04_tmx:
.word 0
.word room_02_tmx
.word room_03_tmx
.word 0
.byte 2, entity_type::Patrol, 216, 72, 5, 216, 72, 216, 32, 168, 32, 168, 144, 216, 144, entity_type::Patrol, 72, 104, 5, 72, 104, 72, 64, 24, 64, 24, 176, 72, 176
.byte 4, 2, 4, 6, 0, 0, 2, 4, 7, 1, 3, 3, 1, 1, 2, 0, 0, 1, 1, 3, 1, 4, 2, 2, 2, 0, 4, 4, 2, 0, 4, 7, 2, 2, 0, 0, 2, 0, 0, 2, 0, 4, 4, 2, 0, 0, 2, 1, 0, 0, 2, 0, 0, 2, 4, 3, 0, 0, 2, 0, 0, 2, 0, 4, 2, 2, 0, 0, 2, 1, 1, 2, 0, 0, 2, 0, 0, 2, 0, 4, 2, 2, 0, 0, 2, 0, 0, 2, 0, 0, 2, 0, 0, 2, 2, 0, 0, 2, 0, 0, 2, 0, 0, 2, 0, 0, 2, 0, 0, 2, 2, 0, 0, 1, 0, 0, 1, 0, 0, 1, 0, 0, 1, 0, 0, 2, 2, 0, 4, 13, 2, 4, 7, 0, 0, 2, 4, 7, 1, 4, 2, 3, 3, 1, 0, 0, 1, 4, 2, 3, 1, 1, 2, 4, 0
