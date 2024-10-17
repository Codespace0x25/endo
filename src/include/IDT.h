#include <stdint.h>
#include "System.h"

void setIdtGate(int n, uint32_t handler);
void initIdt();
void pic_remap();
