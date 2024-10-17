#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include "./include/System.h"

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

#define KBD_DATA_PORT 0x60
#define IDT_SIZE 256
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
  idt_reg.limit = (sizeof(struct idtEntry)*IDT_SIZE) -1;
  idt_reg.base = (uint32_t)&idt;
  setIdtGate(0x21, (uint32_t)KeyboardInterruptHandler);
  loadIdt(&idt_reg);
}
 
void pic_remap(){
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
}



static volatile unsigned char LatestScanCode;

void keyboardHandler(){

  unsigned char  scan = INP(KBD_DATA_PORT);
  LatestScanCode = scan;
}
unsigned char getScanCode(){
  return LatestScanCode;
}
