.include "../src/charmap.inc"
.segment "RODATA"
.export _dialog_getting_key
_dialog_getting_key: .byte 1, "Oi, o que ta acontecendo? Ouvi uma baita correria, sinto um cheiro de queimado... E alias, como voce esta ai fora?", 2, "Nao tenho tempo pra explicar, mas esta acontecendo uma rebeliao, a ala dos magos foi tomada por assassinos. Tenho que ir, boa sorte.", 1, "Espera, pode ao menos me tirar daqui?", 2, "Esta bem, pegue esse molho de chaves, com sorte uma delas deve servir. Adeus!", 0
.export _dialog_magic_is_back
_dialog_magic_is_back: .byte 1, "Parece que minha magia so era bloqueada dentro da cela trancada. Agora posso ao menos conjurar umas bolas de fogo.", 0
.export _dialog_needs_help
_dialog_needs_help: .byte 2, "Uhhhhgh...", 1, "Um mago ferido apareceu do nada, devo ajuda-lo?", 0
.export _dialog_helping
_dialog_helping: .byte 2, "Obrigado por tentar me ajudar, mas ja e' tarde demais para mim. Porem como gratidao quero que fique com isso: um anel de invisibilidade. Tome cuidado pois ele requer muito poder e seu efeito dura pouco tempo. Adeus, jovem...", 0
.export _dialog_invi_would_help
_dialog_invi_would_help: .byte 1, "Uma sala tao bem patrulhada, deve ser bem dificil alguem passar por ela sem a magia Invi...", 0
