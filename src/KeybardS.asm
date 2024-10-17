[bits 32]
section .text
extern keyboardHandler
global KeyboardInterruptHandler

KeyboardInterruptHandler:
    cli                   ;Disable interrupts
    pusha                 ;Save all registers
    call keyboardHandler  ;Call the keyboard handler
    mov al, 0x20          ;Send EOI to PIC
    out 0x20, al          ;Acknowledge interrupt
    popa                  ;restore registers
    iret                  ;Return from interrupt
