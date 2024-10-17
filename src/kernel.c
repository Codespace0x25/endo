#include <stdint.h>
#include "./include/Display.h"
#include "./include/IDT.h"
#include "./include/System.h"
#include "./include/Keybard.h"

void kernel_main() {
  initTTY(0x0f);
//    __asm__ __volatile__ ("cli");
    clearScreen();
    printf("Kernel loading...\n");
/*  uncoment this out when fixed   
    pic_remap();
    initIdt();
*/   
/*    __asm__ __volatile__ ("sti");
    printf("Keyboard set up...\n");
    printf("IDT set up...\n");
*/    
    printf("Kernel started\n");
    setDColor(0x04);
    printf((char *)Hart);
    setDColor(0x0f); 
    while (1) {
    // kernle loop
    }
}
