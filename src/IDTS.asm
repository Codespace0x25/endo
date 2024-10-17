[bits 32]

section .test
  global loadIdt
loadIdt:
  lidt [esp+4]
  ret
