;
; File generated by cc65 v 2.19 - Git c48e821c4
;
	.fopt		compiler,"cc65 v 2.19 - Git c48e821c4"
	.setcpu		"6502"
	.smart		on
	.autoimport	on
	.case		on
	.debuginfo	on
	.importzp	sp, sreg, regsave, regbank
	.importzp	tmp1, tmp2, tmp3, tmp4, ptr1, ptr2, ptr3, ptr4
	.macpack	longbranch
	.dbg		file, "src/dungeon.c", 3695, 1639049961
	.dbg		file, "src/lib/nesdoug.h", 6692, 1638737118
	.dbg		file, "src/lib/neslib.h", 8949, 1638737118
	.dbg		file, "src/lib/unrle.h", 125, 1638737118
	.dbg		file, "src/mmc3/mmc3_code.h", 2262, 1638737118
	.dbg		file, "src/directions.h", 163, 1639048136
	.dbg		file, "src/irq_buffer.h", 147, 1638737118
	.dbg		file, "src/temp.h", 613, 1638737118
	.dbg		file, "src/../assets/palettes.h", 57, 1638911019
	.dbg		file, "src/../assets/nametables.h", 78, 1639020653
	.dbg		file, "src/../assets/metatiles.h", 159, 1639019462
	.dbg		file, "src/../assets/sprites.h", 103, 1639049959
	.dbg		file, "src/../assets/dungeon.h", 168, 1639015544
	.dbg		sym, "multi_vram_buffer_horz", "00", extern, "_multi_vram_buffer_horz"
	.dbg		sym, "clear_vram_buffer", "00", extern, "_clear_vram_buffer"
	.dbg		sym, "get_frame_count", "00", extern, "_get_frame_count"
	.dbg		sym, "pal_fade_to", "00", extern, "_pal_fade_to"
	.dbg		sym, "set_scroll_y", "00", extern, "_set_scroll_y"
	.dbg		sym, "set_data_pointer", "00", extern, "_set_data_pointer"
	.dbg		sym, "set_mt_pointer", "00", extern, "_set_mt_pointer"
	.dbg		sym, "buffer_4_mt", "00", extern, "_buffer_4_mt"
	.dbg		sym, "flush_vram_update_nmi", "00", extern, "_flush_vram_update_nmi"
	.dbg		sym, "pal_bg", "00", extern, "_pal_bg"
	.dbg		sym, "pal_spr", "00", extern, "_pal_spr"
	.dbg		sym, "ppu_off", "00", extern, "_ppu_off"
	.dbg		sym, "ppu_on_all", "00", extern, "_ppu_on_all"
	.dbg		sym, "oam_meta_spr", "00", extern, "_oam_meta_spr"
	.dbg		sym, "vram_adr", "00", extern, "_vram_adr"
	.dbg		sym, "vram_unrle", "00", extern, "_vram_unrle"
	.dbg		sym, "set_unrle_buffer", "00", extern, "_set_unrle_buffer"
	.dbg		sym, "unrle_to_buffer", "00", extern, "_unrle_to_buffer"
	.dbg		sym, "set_chr_mode_0", "00", extern, "_set_chr_mode_0"
	.dbg		sym, "set_chr_mode_1", "00", extern, "_set_chr_mode_1"
	.dbg		sym, "set_chr_mode_2", "00", extern, "_set_chr_mode_2"
	.dbg		sym, "set_chr_mode_3", "00", extern, "_set_chr_mode_3"
	.dbg		sym, "set_chr_mode_4", "00", extern, "_set_chr_mode_4"
	.dbg		sym, "set_chr_mode_5", "00", extern, "_set_chr_mode_5"
	.dbg		sym, "double_buffer", "00", extern, "_double_buffer"
	.dbg		sym, "double_buffer_index", "00", extern, "_double_buffer_index"
	.dbg		sym, "temp", "00", extern, "_temp"
	.dbg		sym, "temp_x", "00", extern, "_temp_x"
	.dbg		sym, "temp_y", "00", extern, "_temp_y"
	.dbg		sym, "temp_int", "00", extern, "_temp_int"
	.dbg		sym, "bg_palette", "00", extern, "_bg_palette"
	.dbg		sym, "sprites_palette", "00", extern, "_sprites_palette"
	.dbg		sym, "hud_nametable", "00", extern, "_hud_nametable"
	.dbg		sym, "metatiles", "00", extern, "_metatiles"
	.dbg		sym, "metasprites_pointers", "00", extern, "_metasprites_pointers"
	.dbg		sym, "starting_room", "00", extern, "_starting_room"
	.import		_multi_vram_buffer_horz
	.import		_clear_vram_buffer
	.import		_get_frame_count
	.import		_pal_fade_to
	.import		_set_scroll_y
	.import		_set_data_pointer
	.import		_set_mt_pointer
	.import		_buffer_4_mt
	.import		_flush_vram_update_nmi
	.import		_pal_bg
	.import		_pal_spr
	.import		_ppu_off
	.import		_ppu_on_all
	.import		_oam_meta_spr
	.import		_vram_adr
	.import		_vram_unrle
	.import		_set_unrle_buffer
	.import		_unrle_to_buffer
	.import		_set_chr_mode_0
	.import		_set_chr_mode_1
	.import		_set_chr_mode_2
	.import		_set_chr_mode_3
	.import		_set_chr_mode_4
	.import		_set_chr_mode_5
	.import		_double_buffer
	.importzp	_double_buffer_index
	.importzp	_temp
	.importzp	_temp_x
	.importzp	_temp_y
	.importzp	_temp_int
	.import		_bg_palette
	.import		_sprites_palette
	.import		_hud_nametable
	.import		_metatiles
	.import		_metasprites_pointers
	.import		_starting_room
	.export		_current_room_ptr
	.export		_up_room_ptr
	.export		_down_room_ptr
	.export		_left_room_ptr
	.export		_right_room_ptr
	.export		_room_buffer
	.export		_player_x
	.export		_player_y
	.export		_player_dx
	.export		_player_dy
	.export		_player_direction
	.export		_empty_row
	.export		_load_room
	.export		_init_dungeon
	.export		_dungeon_handler
	.export		_dungeon_draw_sprites

.segment	"RODATA"

.segment	"RODATA"
.segment	"RODATA"
.segment	"RODATA"
.segment	"RODATA"
_empty_row:
	.byte	$00
	.byte	$00
	.byte	$00
	.byte	$00
	.byte	$00
	.byte	$00
	.byte	$00
	.byte	$00
	.byte	$00
	.byte	$00
	.byte	$00
	.byte	$00
	.byte	$00
	.byte	$00
	.byte	$00
	.byte	$00
	.byte	$00
	.byte	$00
	.byte	$00
	.byte	$00
	.byte	$00
	.byte	$00
	.byte	$00
	.byte	$00
	.byte	$00
	.byte	$00
	.byte	$00
	.byte	$00
	.byte	$00
	.byte	$00
	.byte	$00
	.byte	$00

.segment	"BSS"

.segment	"ZEROPAGE"
_current_room_ptr:
	.res	2,$00
_up_room_ptr:
	.res	2,$00
_down_room_ptr:
	.res	2,$00
_left_room_ptr:
	.res	2,$00
_right_room_ptr:
	.res	2,$00
.segment	"BSS"
_room_buffer:
	.res	240,$00
_player_x:
	.res	2,$00
_player_y:
	.res	2,$00
_player_dx:
	.res	2,$00
_player_dy:
	.res	2,$00
_player_direction:
	.res	1,$00

; ---------------------------------------------------------------
; void __near__ load_room (unsigned char *room_ptr)
; ---------------------------------------------------------------

.segment	"CODE"

.proc	_load_room: near

	.dbg	func, "load_room", "00", extern, "_load_room"
	.dbg	sym, "room_ptr", "00", auto, 0

.segment	"CODE"

;
; void load_room(unsigned char *room_ptr) {
;
	.dbg	line, "src/dungeon.c", 53
	jsr     pushax
;
; current_room_ptr = room_ptr;
;
	.dbg	line, "src/dungeon.c", 54
	ldy     #$01
	lda     (sp),y
	tax
	dey
	lda     (sp),y
	sta     _current_room_ptr
	stx     _current_room_ptr+1
;
; up_room_ptr = *(unsigned char **) current_room_ptr;
;
	.dbg	line, "src/dungeon.c", 56
	sta     ptr1
	stx     ptr1+1
	iny
	lda     (ptr1),y
	sta     _up_room_ptr+1
	dey
	lda     (ptr1),y
	sta     _up_room_ptr
;
; current_room_ptr += 2;
;
	.dbg	line, "src/dungeon.c", 57
	lda     #$02
	clc
	adc     _current_room_ptr
	sta     _current_room_ptr
	bcc     L0002
	inc     _current_room_ptr+1
;
; down_room_ptr = *(unsigned char **) current_room_ptr;
;
	.dbg	line, "src/dungeon.c", 58
L0002:	lda     _current_room_ptr+1
	sta     ptr1+1
	lda     _current_room_ptr
	sta     ptr1
	iny
	lda     (ptr1),y
	sta     _down_room_ptr+1
	dey
	lda     (ptr1),y
	sta     _down_room_ptr
;
; current_room_ptr += 2;
;
	.dbg	line, "src/dungeon.c", 59
	lda     #$02
	clc
	adc     _current_room_ptr
	sta     _current_room_ptr
	bcc     L0003
	inc     _current_room_ptr+1
;
; left_room_ptr = *(unsigned char **) current_room_ptr;
;
	.dbg	line, "src/dungeon.c", 60
L0003:	lda     _current_room_ptr+1
	sta     ptr1+1
	lda     _current_room_ptr
	sta     ptr1
	iny
	lda     (ptr1),y
	sta     _left_room_ptr+1
	dey
	lda     (ptr1),y
	sta     _left_room_ptr
;
; current_room_ptr += 2;
;
	.dbg	line, "src/dungeon.c", 61
	lda     #$02
	clc
	adc     _current_room_ptr
	sta     _current_room_ptr
	bcc     L0004
	inc     _current_room_ptr+1
;
; right_room_ptr = *(unsigned char **) current_room_ptr;
;
	.dbg	line, "src/dungeon.c", 62
L0004:	lda     _current_room_ptr+1
	sta     ptr1+1
	lda     _current_room_ptr
	sta     ptr1
	iny
	lda     (ptr1),y
	sta     _right_room_ptr+1
	dey
	lda     (ptr1),y
	sta     _right_room_ptr
;
; current_room_ptr += 2;
;
	.dbg	line, "src/dungeon.c", 63
	lda     #$02
	clc
	adc     _current_room_ptr
	sta     _current_room_ptr
	bcc     L0005
	inc     _current_room_ptr+1
;
; set_unrle_buffer((unsigned char *) room_buffer);
;
	.dbg	line, "src/dungeon.c", 65
L0005:	lda     #<(_room_buffer)
	ldx     #>(_room_buffer)
	jsr     _set_unrle_buffer
;
; unrle_to_buffer(current_room_ptr);
;
	.dbg	line, "src/dungeon.c", 66
	lda     _current_room_ptr
	ldx     _current_room_ptr+1
	jsr     _unrle_to_buffer
;
; pal_fade_to(4, 0);
;
	.dbg	line, "src/dungeon.c", 68
	lda     #$04
	jsr     pusha
	lda     #$00
	jsr     _pal_fade_to
;
; ppu_off();
;
	.dbg	line, "src/dungeon.c", 69
	jsr     _ppu_off
;
; pal_bg(bg_palette);
;
	.dbg	line, "src/dungeon.c", 71
	lda     #<(_bg_palette)
	ldx     #>(_bg_palette)
	jsr     _pal_bg
;
; pal_spr(sprites_palette);
;
	.dbg	line, "src/dungeon.c", 72
	lda     #<(_sprites_palette)
	ldx     #>(_sprites_palette)
	jsr     _pal_spr
;
; vram_adr(NTADR_C(0,0));
;
	.dbg	line, "src/dungeon.c", 75
	ldx     #$28
	lda     #$00
	jsr     _vram_adr
;
; vram_unrle(hud_nametable);
;
	.dbg	line, "src/dungeon.c", 76
	lda     #<(_hud_nametable)
	ldx     #>(_hud_nametable)
	jsr     _vram_unrle
;
; vram_adr(NTADR_A(0,0));
;
	.dbg	line, "src/dungeon.c", 77
	ldx     #$20
	lda     #$00
	jsr     _vram_adr
;
; set_mt_pointer(metatiles);
;
	.dbg	line, "src/dungeon.c", 79
	lda     #<(_metatiles)
	ldx     #>(_metatiles)
	jsr     _set_mt_pointer
;
; set_data_pointer(room_buffer);
;
	.dbg	line, "src/dungeon.c", 80
	lda     #<(_room_buffer)
	ldx     #>(_room_buffer)
	jsr     _set_data_pointer
;
; temp_int = 0x2000;
;
	.dbg	line, "src/dungeon.c", 82
	ldx     #$20
	lda     #$00
	sta     _temp_int
	stx     _temp_int+1
;
; temp = 0;
;
	.dbg	line, "src/dungeon.c", 83
	sta     _temp
;
; for(temp_y = 0; temp_y < 12; temp_y+=2) {
;
	.dbg	line, "src/dungeon.c", 84
L0014:	sta     _temp_y
	cmp     #$0C
	bcs     L0016
;
; for(temp_x = 0; temp_x < 16; temp_x+=2) {
;
	.dbg	line, "src/dungeon.c", 85
	lda     #$00
L0013:	sta     _temp_x
	cmp     #$10
	bcs     L0015
;
; temp_int = 0x2000 + 2 * temp_x + 0x40 * temp_y;
;
	.dbg	line, "src/dungeon.c", 86
	ldx     #$00
	lda     _temp_x
	asl     a
	bcc     L0012
	inx
L0012:	sta     ptr1
	txa
	clc
	adc     #$20
	sta     ptr1+1
	ldx     #$00
	lda     _temp_y
	jsr     shlax4
	jsr     shlax2
	clc
	adc     ptr1
	pha
	txa
	adc     ptr1+1
	tax
	pla
	sta     _temp_int
	stx     _temp_int+1
;
; buffer_4_mt(temp_int, (temp_y << 4) | temp_x);
;
	.dbg	line, "src/dungeon.c", 87
	jsr     pushax
	lda     _temp_y
	asl     a
	asl     a
	asl     a
	asl     a
	sta     ptr1
	lda     _temp_x
	ora     ptr1
	jsr     _buffer_4_mt
;
; flush_vram_update_nmi();
;
	.dbg	line, "src/dungeon.c", 88
	jsr     _flush_vram_update_nmi
;
; clear_vram_buffer();
;
	.dbg	line, "src/dungeon.c", 89
	jsr     _clear_vram_buffer
;
; for(temp_x = 0; temp_x < 16; temp_x+=2) {
;
	.dbg	line, "src/dungeon.c", 85
	lda     #$02
	clc
	adc     _temp_x
	jmp     L0013
;
; for(temp_y = 0; temp_y < 12; temp_y+=2) {
;
	.dbg	line, "src/dungeon.c", 84
L0015:	lda     #$02
	clc
	adc     _temp_y
	jmp     L0014
;
; for(temp_y = 24; temp_y < 30; temp_y++) {
;
	.dbg	line, "src/dungeon.c", 93
L0016:	lda     #$18
	sta     _temp_y
L0017:	lda     _temp_y
	cmp     #$1E
	bcs     L0018
;
; multi_vram_buffer_horz(empty_row, 32, NTADR_A(0, temp_y));
;
	.dbg	line, "src/dungeon.c", 94
	jsr     decsp3
	lda     #<(_empty_row)
	ldy     #$01
	sta     (sp),y
	iny
	lda     #>(_empty_row)
	sta     (sp),y
	lda     #$20
	ldy     #$00
	sta     (sp),y
	ldx     #$00
	lda     _temp_y
	jsr     aslax4
	stx     tmp1
	asl     a
	rol     tmp1
	pha
	lda     tmp1
	ora     #$20
	tax
	pla
	jsr     _multi_vram_buffer_horz
;
; flush_vram_update_nmi();
;
	.dbg	line, "src/dungeon.c", 95
	jsr     _flush_vram_update_nmi
;
; clear_vram_buffer();
;
	.dbg	line, "src/dungeon.c", 96
	jsr     _clear_vram_buffer
;
; for(temp_y = 24; temp_y < 30; temp_y++) {
;
	.dbg	line, "src/dungeon.c", 93
	inc     _temp_y
	jmp     L0017
;
; set_chr_mode_2(BG_MAIN_0);
;
	.dbg	line, "src/dungeon.c", 99
L0018:	lda     #$00
	jsr     _set_chr_mode_2
;
; set_chr_mode_3(BG_MAIN_1);
;
	.dbg	line, "src/dungeon.c", 100
	lda     #$01
	jsr     _set_chr_mode_3
;
; set_chr_mode_4(BG_MAIN_2);
;
	.dbg	line, "src/dungeon.c", 101
	lda     #$02
	jsr     _set_chr_mode_4
;
; set_chr_mode_5(BG_MAIN_3);
;
	.dbg	line, "src/dungeon.c", 102
	lda     #$03
	jsr     _set_chr_mode_5
;
; set_chr_mode_0(SPRITE_0);
;
	.dbg	line, "src/dungeon.c", 103
	lda     #$04
	jsr     _set_chr_mode_0
;
; set_chr_mode_1(SPRITE_1);
;
	.dbg	line, "src/dungeon.c", 104
	lda     #$06
	jsr     _set_chr_mode_1
;
; ppu_on_all();
;
	.dbg	line, "src/dungeon.c", 105
	jsr     _ppu_on_all
;
; set_scroll_y(0);
;
	.dbg	line, "src/dungeon.c", 106
	ldx     #$00
	txa
	jsr     _set_scroll_y
;
; pal_fade_to(0, 4);
;
	.dbg	line, "src/dungeon.c", 107
	lda     #$00
	jsr     pusha
	lda     #$04
	jsr     _pal_fade_to
;
; }
;
	.dbg	line, "src/dungeon.c", 108
	jmp     incsp2

	.dbg	line
.endproc

; ---------------------------------------------------------------
; void __near__ init_dungeon (void)
; ---------------------------------------------------------------

.segment	"CODE"

.proc	_init_dungeon: near

	.dbg	func, "init_dungeon", "00", extern, "_init_dungeon"

.segment	"CODE"

;
; load_room((unsigned char*) starting_room);
;
	.dbg	line, "src/dungeon.c", 45
	lda     _starting_room
	ldx     _starting_room+1
	jsr     _load_room
;
; player_x = FP(0x30, 0x00);
;
	.dbg	line, "src/dungeon.c", 46
	ldx     #$30
	lda     #$00
	sta     _player_x
	stx     _player_x+1
;
; player_y = FP(0x40, 0x00);
;
	.dbg	line, "src/dungeon.c", 47
	ldx     #$40
	sta     _player_y
	stx     _player_y+1
;
; player_dx = 0x00;
;
	.dbg	line, "src/dungeon.c", 48
	sta     _player_dx
	sta     _player_dx+1
;
; player_dy = 0x00;
;
	.dbg	line, "src/dungeon.c", 49
	sta     _player_dy
	sta     _player_dy+1
;
; player_direction = Down;
;
	.dbg	line, "src/dungeon.c", 50
	lda     #$01
	sta     _player_direction
;
; }
;
	.dbg	line, "src/dungeon.c", 51
	rts

	.dbg	line
.endproc

; ---------------------------------------------------------------
; void __near__ dungeon_handler (void)
; ---------------------------------------------------------------

.segment	"CODE"

.proc	_dungeon_handler: near

	.dbg	func, "dungeon_handler", "00", extern, "_dungeon_handler"

.segment	"CODE"

;
; double_buffer[double_buffer_index++] = MENU_SCANLINE - 1;
;
	.dbg	line, "src/dungeon.c", 113
	lda     _double_buffer_index
	inc     _double_buffer_index
	clc
	adc     #<(_double_buffer)
	sta     ptr1
	lda     #$00
	adc     #>(_double_buffer)
	sta     ptr1+1
	lda     #$BF
	ldy     #$00
	sta     (ptr1),y
;
; double_buffer[double_buffer_index++] = 0xf6;
;
	.dbg	line, "src/dungeon.c", 114
	lda     _double_buffer_index
	inc     _double_buffer_index
	clc
	adc     #<(_double_buffer)
	sta     ptr1
	tya
	adc     #>(_double_buffer)
	sta     ptr1+1
	lda     #$F6
	sta     (ptr1),y
;
; double_buffer[double_buffer_index++] = 8;
;
	.dbg	line, "src/dungeon.c", 115
	lda     _double_buffer_index
	inc     _double_buffer_index
	clc
	adc     #<(_double_buffer)
	sta     ptr1
	tya
	adc     #>(_double_buffer)
	sta     ptr1+1
	lda     #$08
	sta     (ptr1),y
;
; temp_int = 0x2000; // TODO: switch to dialogue
;
	.dbg	line, "src/dungeon.c", 116
	ldx     #$20
	sty     _temp_int
	stx     _temp_int+1
;
; double_buffer[double_buffer_index++] = temp_int;
;
	.dbg	line, "src/dungeon.c", 117
	lda     _double_buffer_index
	inc     _double_buffer_index
	clc
	adc     #<(_double_buffer)
	sta     ptr1
	tya
	adc     #>(_double_buffer)
	sta     ptr1+1
	lda     _temp_int
	sta     (ptr1),y
;
; double_buffer[double_buffer_index++] = 0;
;
	.dbg	line, "src/dungeon.c", 118
	lda     _double_buffer_index
	inc     _double_buffer_index
	clc
	adc     #<(_double_buffer)
	sta     ptr1
	tya
	adc     #>(_double_buffer)
	sta     ptr1+1
	tya
	sta     (ptr1),y
;
; double_buffer[double_buffer_index++] = ((temp_int & 0xF8) << 2);
;
	.dbg	line, "src/dungeon.c", 119
	lda     _double_buffer_index
	inc     _double_buffer_index
	clc
	adc     #<(_double_buffer)
	sta     ptr1
	tya
	adc     #>(_double_buffer)
	sta     ptr1+1
	lda     _temp_int
	and     #$F8
	asl     a
	asl     a
	sta     (ptr1),y
;
; }
;
	.dbg	line, "src/dungeon.c", 120
	rts

	.dbg	line
.endproc

; ---------------------------------------------------------------
; void __near__ dungeon_draw_sprites (void)
; ---------------------------------------------------------------

.segment	"CODE"

.proc	_dungeon_draw_sprites: near

	.dbg	func, "dungeon_draw_sprites", "00", extern, "_dungeon_draw_sprites"

.segment	"CODE"

;
; temp = 2 * player_direction;
;
	.dbg	line, "src/dungeon.c", 124
	lda     _player_direction
	asl     a
	sta     _temp
;
; if (player_dx >= FP(0x02, 0x00) ||
;
	.dbg	line, "src/dungeon.c", 125
	lda     _player_dx
	cmp     #$00
	lda     _player_dx+1
	sbc     #$02
	bvs     L0003
	eor     #$80
L0003:	bmi     L0018
;
; player_dx <= -FP(0x02, 0x00) ||
;
	.dbg	line, "src/dungeon.c", 126
	lda     _player_dx
	cmp     #$01
	lda     _player_dx+1
	sbc     #$FE
	bvc     L0005
	eor     #$80
L0005:	bmi     L0018
;
; player_dy >= FP(0x02, 0x00) ||
;
	.dbg	line, "src/dungeon.c", 127
	lda     _player_dy
	cmp     #$00
	lda     _player_dy+1
	sbc     #$02
	bvs     L0006
	eor     #$80
L0006:	bmi     L0018
;
; player_dy <= -FP(0x02, 0x00)) {
;
	.dbg	line, "src/dungeon.c", 128
	lda     _player_dy
	cmp     #$01
	lda     _player_dy+1
	sbc     #$FE
	bvc     L0007
	eor     #$80
L0007:	bpl     L0002
;
; if (get_frame_count() & 0x04) temp++;
;
	.dbg	line, "src/dungeon.c", 129
L0018:	jsr     _get_frame_count
	and     #$04
;
; } else if (player_dx >= FP(0x01, 0x00) ||
;
	.dbg	line, "src/dungeon.c", 130
	jmp     L001F
L0002:	lda     _player_dx
	cmp     #$00
	lda     _player_dx+1
	sbc     #$01
	bvs     L000C
	eor     #$80
L000C:	bmi     L0019
;
; player_dx <= -FP(0x01, 0x00) ||
;
	.dbg	line, "src/dungeon.c", 131
	lda     _player_dx
	cmp     #$01
	lda     _player_dx+1
	sbc     #$FF
	bvc     L000E
	eor     #$80
L000E:	bmi     L0019
;
; player_dy >= FP(0x01, 0x00) ||
;
	.dbg	line, "src/dungeon.c", 132
	lda     _player_dy
	cmp     #$00
	lda     _player_dy+1
	sbc     #$01
	bvs     L000F
	eor     #$80
L000F:	bmi     L0019
;
; player_dy <= -FP(0x01, 0x00)) {
;
	.dbg	line, "src/dungeon.c", 133
	lda     _player_dy
	cmp     #$01
	lda     _player_dy+1
	sbc     #$FF
	bvc     L0010
	eor     #$80
L0010:	bpl     L0012
;
; if (get_frame_count() & 0x10) temp++;
;
	.dbg	line, "src/dungeon.c", 134
L0019:	jsr     _get_frame_count
	and     #$10
L001F:	cmp     #$00
	beq     L0012
	inc     _temp
;
; oam_meta_spr(INT(player_x), INT(player_y), (const unsigned char *) metasprites_pointers[temp]);
;
	.dbg	line, "src/dungeon.c", 136
L0012:	jsr     decsp2
	lda     _player_x+1
	ldy     #$01
	sta     (sp),y
	lda     _player_y+1
	dey
	sta     (sp),y
	ldx     #$00
	lda     _temp
	asl     a
	bcc     L0017
	inx
	clc
L0017:	adc     #<(_metasprites_pointers)
	sta     ptr1
	txa
	adc     #>(_metasprites_pointers)
	sta     ptr1+1
	iny
	lda     (ptr1),y
	tax
	dey
	lda     (ptr1),y
	jmp     _oam_meta_spr

	.dbg	line
.endproc

