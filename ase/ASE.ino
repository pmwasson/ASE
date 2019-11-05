
#include <Arduboy2.h>
#include "BoxCursor.h"
#include "VarFont6.h"

/* some icon ideas:
 * (8 columns) 
0x00, 0x00, 0xF6, 0xF6, 0xF6, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x1F, 0x7F, 0x1F, 0x00, 0x00, 0x00, 
0x38, 0x44, 0x92, 0x8A, 0x82, 0x82, 0x44, 0x38, 
0x00, 0x00, 0x00, 0x7F, 0x7F, 0x00, 0x00, 0x00, 
0x00, 0x10, 0x30, 0x7E, 0x7E, 0x30, 0x10, 0x00, 
0x00, 0x7F, 0x49, 0x49, 0x4F, 0x49, 0x7E, 0x00, 
0x00, 0x08, 0x0C, 0x7E, 0x7E, 0x0C, 0x08, 0x00, 
0x00, 0x7F, 0x49, 0x49, 0x4F, 0x49, 0x7E, 0x00, 
0x00, 0x22, 0x14, 0x08, 0x22, 0x14, 0x08, 0x00, 
0x00, 0x3E, 0x22, 0xE2, 0xA2, 0xBE, 0x88, 0xF8,
 */

Arduboy2 arduboy;
BoxCursor cursor;
VarFont6 font6;

uint8_t sprite[256];

void setup() {
  arduboy.begin();
  arduboy.setFrameRate(30);
  cursor.setOffset(31,0);
  cursor.setPos(0,0);
  cursor.setSize(16,16);
  Serial.begin(9600);
  for (int i=0; i<256; i++) {
    sprite[i] = 0;
  }
}

void loop() {
  if (!(arduboy.nextFrame()))
    return;
  arduboy.pollButtons();
  arduboy.clear();

  arduboy.fillRect(0,0,31,8);
  arduboy.drawFastVLine(30,0,64);
  arduboy.drawFastVLine(95,0,64);
  arduboy.drawFastHLine(95,32,33);
  font6.setCursor(1,1);
  font6.setInverse(true);
  font6.print("DRAW\n");
  font6.setInverse(false);
  font6.print("SHIFT\n");
  font6.print("FRAME\n");
  font6.print("PREVIEW\n");
  font6.print("NEW\n");
  font6.print("LOAD\n");
  font6.print("SAVE\n");
  font6.setCursor(97,34,false);
  font6.print("C: ");
  font6.print(cursor.cursorX);
  font6.print(", ");
  font6.print(cursor.cursorY);
  font6.print("\nS: 16 x 16");
  font6.print("\nF: 1 / 8");
  font6.print("\nL: ");
  font6.print(arduboy.frameCount&0x1f);

  for (int x=0; x<16; x++) {
    for (int y=0; y<16; y++) {
      if (sprite[x+y*16]) {
        arduboy.fillRect(cursor.offsetX+x*4,cursor.offsetY+y*4,4,4);
        arduboy.drawPixel(96+8+x,8+y);
      }
    }
  }
  
  
  cursor.directionalButtons();
  cursor.draw();


  
  if (arduboy.justPressed(B_BUTTON)) {
    sprite[cursor.cursorX+cursor.cursorY*16] ^= 1;
  } 

  arduboy.display();
}
