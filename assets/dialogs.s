.include "../src/charmap.inc"
.segment "RODATA"
.export _dialog_getting_key
_dialog_getting_key: .byte 1, "Oi, o que t", $5f, " acontecendo? Ouvi uma baita correria, sinto um cheiro de queimado... E ali", $5f, "s, como voc", $64, " est", $5f, " a", $65, " fora?", 2, "N", $62, "o tenho tempo pra explicar, mas est", $5f, " acontecendo uma rebeli", $62, "o, a ala dos magos foi tomada por assassinos. Tenho que ir, boa sorte.", 1, "Espera, pode ao menos me tirar daqui?", 2, "Est", $5f, " bem, pegue este molho de chaves, com sorte uma delas deve servir. Adeus!", 0
.export _dialog_magic_is_back
_dialog_magic_is_back: .byte 1, "Parece que minha magia s", $66, " era bloqueada dentro da cela trancada. Agora posso ao menos conjurar umas bolas de fogo.", 0
.export _dialog_needs_help
_dialog_needs_help: .byte 2, "Uhhhhgh...", 1, "Um mago ferido apareceu do nada, devo ajud", $5f, "-lo?", 0
.export _dialog_helping
_dialog_helping: .byte 2, "Obrigado por tentar me ajudar, mas j", $5f, " ", $63, " tarde demais para mim. Por", $63, "m como gratid", $62, "o quero que fique com isto: um anel Invi, capaz de torn", $5f, "-lo invis", $65, "vel. Tome cuidado pois ele consome muito poder e seu efeito dura apenas poucos segundos. Adeus, meu jovem...", 0
.export _dialog_invi_would_help
_dialog_invi_would_help: .byte 1, "Uma sala t", $62, "o bem patrulhada, deve ser bem dif", $65, "cil algu", $63, "m passar por ela sem a magia Invi...", 0
