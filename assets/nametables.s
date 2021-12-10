.segment "RODATA"

.export _title_nametable, _hud_nametable
.export _prologue_nametable, _good_ending_nametable, _bad_ending_nametable
_title_nametable: .incbin "nametables/title.rle"
_hud_nametable: .incbin "nametables/hud.rle"
_prologue_nametable: .incbin "nametables/prologue.rle"
_good_ending_nametable: .incbin "nametables/good-ending.rle"
_bad_ending_nametable: .incbin "nametables/bad-ending.rle"
