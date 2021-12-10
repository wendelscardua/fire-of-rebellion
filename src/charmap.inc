.charmap ',', $0e
.charmap '.', $0f
.charmap ':', $1a
.charmap '!', $1b
.charmap ' ', $04
.charmap '?', $1f
.charmap '-', $1d
.charmap $27, $3e
.repeat $40, i
  .charmap $40 + i, $20 + i
.endrepeat
