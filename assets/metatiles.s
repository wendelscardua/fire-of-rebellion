.segment "RODATA"

.export _metatiles
_metatiles:
; ground
.byte $80, $81, $90, $91, $02
; front wall
.byte $82, $83, $92, $93, $01
; top wall
.byte $84, $85, $94, $95, $01
; bars
.byte $86, $86, $96, $96, $00
