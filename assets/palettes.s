.segment "RODATA"

.export _bg_palette
.export _sprites_palette
.export _bg_title_palette

_bg_palette: .incbin "bg.pal"
_sprites_palette: .incbin "sprites.pal"
_bg_title_palette: .incbin "bg-title.pal"
