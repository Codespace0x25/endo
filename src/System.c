#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include "./include/System.h"
#include "./include/Display.h"

unsigned char INP(Port port){
  unsigned char resolt;
  __asm__("in %%dx, %%al" : "=a" (resolt) : "d"(port));
  return resolt;

}
void OUTP(Port port, unsigned char data){
  __asm__("out %%al, %%dx" : : "a" (data) , "d" (port));
}

void memcopy(char *sorce, char *dest, int nbytes) {
    int i;
    for (i = 0; i < nbytes; i++) {
        *(dest + i) = *(sorce + i); // Use i instead of 1
    }
}
// this is the for interups


#define KBD_DATA_PORT 0x60
#define IDT_SIZE 256
#define PIC1		0x20		/* IO base address for master PIC */
#define PIC2		0xA0		/* IO base address for slave PIC */
#define PIC1_COMMAND	PIC1
#define PIC1_DATA	(PIC1+1)
#define PIC2_COMMAND	PIC2
#define PIC2_DATA	(PIC2+1)
void initKeyboardInterupt();

struct idtEntry {
  uint16_t offsetLow;
  uint16_t selector;
  uint8_t zero;
  uint8_t typeAttr;
  uint16_t offsetHigh;
}__attribute__((packed));

struct idtPtr{
  uint16_t limit;
  uint32_t base;
}__attribute__((packed));

struct idtEntry idt[IDT_SIZE];
struct idtPtr idt_reg;


void setIdtGate(int n, uint32_t handler){
  idt[n].offsetLow = handler & 0xffff;
  idt[n].selector = 0x08;
  idt[n].zero = 0;
  idt[n].typeAttr = 0x8E;
  idt[n].offsetHigh = (handler >> 16) & 0xffff;
}

extern void KeyboardInterruptHandler();
extern void loadIdt(struct idtPtr* idtptr);

void initIdt(){
  printf("IDT set up...\n");
  idt_reg.limit = (sizeof(struct idtEntry)*IDT_SIZE) -1;
  printf("Keyboard set up...\n");
  idt_reg.base = (uint32_t)&idt;
  setIdtGate(0x21, (uint32_t)KeyboardInterruptHandler);
  loadIdt(&idt_reg);
}
 void IRQ_set_mask(uint8_t IRQline) {
    uint16_t port;
    uint8_t value;

    if(IRQline < 8) {
        port = PIC1_DATA;
    } else {
        port = PIC2_DATA;
        IRQline -= 8;
    }
    value = INP(port) | (1 << IRQline);
    OUTP(port, value);        
}

void IRQ_clear_mask(uint8_t IRQline) {
    uint16_t port;
    uint8_t value;

    if(IRQline < 8) {
        port = PIC1_DATA;
    } else {
        port = PIC2_DATA;
        IRQline -= 8;
    }
    value = INP(port) & ~(1 << IRQline);
    OUTP(port, value);        
}
void pic_remap(){
  printf("remapinmg the pic...\n");
  OUTP(0x20, 0x11);
  OUTP(0xA0, 0x11);

  OUTP(0x21, 0x20);
  OUTP(0xA1, 0x28);

  OUTP(0x21, 0x04);
  OUTP(0xA1, 0x02);

  OUTP(0x21, 0x01);
  OUTP(0xA1, 0x01);

  OUTP(0x21, 0x0);
  OUTP(0xA1, 0x0);
  printf("masking 0x20..\n");
  IRQ_set_mask(0x20);
  printf("masked 0x20\n");
}



static volatile unsigned char LatestScanCode;

void keyboardHandler(){
  printf("keyPress haped\n");
  unsigned char  scan = INP(KBD_DATA_PORT);
  LatestScanCode = scan;
}
unsigned char getScanCode(){
  return LatestScanCode;
}
