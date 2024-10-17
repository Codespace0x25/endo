#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include "./include/Display.h"
#include "./include/System.h"

#define VGA_CTRL_REGISTER 0x3d4
#define VGA_DATA_REGISTER 0x3d5
#define VGA_OFFSET_LOW    0x0f
#define VGA_OFFSET_HIGH   0x0E

#define MAX_ROWS 25
#define MAX_COLS 80
#define VIDEO_ADDRESS 0xb8000


void setCursor(int offset){
  offset /= 2;
  OUTP(VGA_CTRL_REGISTER, VGA_OFFSET_HIGH);
  OUTP(VGA_DATA_REGISTER, (unsigned char) (offset >> 8));
  OUTP(VGA_CTRL_REGISTER, VGA_OFFSET_LOW);
  OUTP(VGA_DATA_REGISTER, (unsigned char) (offset & 0xff));
}

int getCurser(){
  OUTP(VGA_CTRL_REGISTER, VGA_OFFSET_HIGH);
  int offset = INP(VGA_DATA_REGISTER) <<8;
  OUTP(VGA_CTRL_REGISTER, VGA_OFFSET_LOW);
  offset += INP(VGA_DATA_REGISTER);
  return offset*2;
}

void setCharAtVideoMemory(char character, int offset, unsigned char color) {
  unsigned char *vidmem = (unsigned char *) VIDEO_ADDRESS;
  vidmem[offset] = character;
  vidmem[offset+1] = color;
}

int getRowFromOffset(int offset){
  return offset/(2*MAX_COLS);
}
int getOffset(int col, int row){
  return 2*(row * MAX_COLS + col);
}
int moveOffSetToNewLine(int offset){
  return getOffset(0,getRowFromOffset(offset) +1);
}

int scrollLN(int offset){
  memcopy(
    (char *) (getOffset(0,1)+VIDEO_ADDRESS),
    (char *) (getOffset(0,0)+VIDEO_ADDRESS), 
    MAX_COLS * (MAX_ROWS-1)*2);
  for (int col=0;col < MAX_COLS; col++) {
    setCharAtVideoMemory(' ', getOffset(col, MAX_ROWS - 1),DColor);
  }
  return offset - 2 * MAX_COLS;
}
void clearScreen(){
  for (int i = 0; i < MAX_COLS*MAX_ROWS; ++i) {
    setCharAtVideoMemory(' ', i*2, DColor); 
  }
  setCursor(getOffset(0,0));
}

void printf(char *string){

  int offset = getCurser();
  int i=0;
  while (string[i]!=0) {
    if (offset >= MAX_ROWS * MAX_COLS * 2) {
      offset = scrollLN(offset);
    }
    else if (string[i]=='\n') {
      offset = moveOffSetToNewLine(offset);
    }
    else {
      setCharAtVideoMemory(string[i], offset, DColor);
      offset +=2;
    }
    i++;
  }
  setCursor(offset);
}

