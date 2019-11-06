
#include <Arduboy2.h>
#include "BoxCursor.h"
#include "VarFont6.h"
#include "Menu.h"

Arduboy2  arduboy;
BoxCursor cursor;
VarFont6  font6;
Menu      menu;

static const uint8_t inMenu   = 0;
static const uint8_t inCanvas = 1;

uint8_t width;
uint8_t height;
uint8_t sprite[256];
uint8_t color;
uint8_t mode;
uint8_t frame;
uint8_t frameCount;

void setup() {
  arduboy.begin();
  arduboy.setFrameRate(30);
  cursor.setOffset(31,0);
  cursor.setPos(0,0);
  cursor.setSize(16,16);
  Serial.begin(9600);
  mode = inMenu;
  frame = 0;
  frameCount = 8;
  width = 16;
  height = 16;
  clearSprite();
}

void clearSprite() {
  for (int i=0; i<256; i++) {
    sprite[i] = 0;
  }  
}

void loop() {
  if (!(arduboy.nextFrame()))
    return;
  arduboy.pollButtons();
  arduboy.clear();
  drawScreen();
  menu.draw();
  
  if (mode == inMenu) {
    menu.readButtons();
    if (arduboy.justPressed(B_BUTTON)) {
      if (menu.selection == menu.selectDraw) {
        mode = inCanvas;
      }
      else if (menu.selection == menu.selectNew) {
        clearSprite();
      }
    }
  }
  else {
    cursor.draw();

    bool moved = cursor.directionalButtons();  

    if (arduboy.justPressed(B_BUTTON)) {
      color = readSprite(cursor.cursorX,cursor.cursorY);
      if (color == 0) {
        color = 3;
      }
      else if (color == 3) {
        color = 2;
      }
      else {
        color = 0;
      }
      writeSprite(cursor.cursorX, cursor.cursorY, color);
    } 
    else if (arduboy.pressed(B_BUTTON) && moved) {
      writeSprite(cursor.cursorX, cursor.cursorY, color);
    }

    if (arduboy.justPressed(A_BUTTON)) {
      mode = inMenu;
    }
}
  


  arduboy.display();
}

uint16_t spriteIndex(uint8_t x, uint8_t y) {
  return (2*(frame*width*height/8+x+(y/8)*width));
}

uint8_t readSprite(uint8_t x, uint8_t y) {
  uint16_t i = spriteIndex(x,y);
  return (1 & (sprite[i] >> (y % 8))) +
         ((1 & (sprite[i+1] >> (y % 8))) << 1);
  
}

void writeSprite(uint8_t x, uint8_t y, uint8_t color) {
  uint16_t i = spriteIndex(x,y);
  sprite[i] &= ~(0x1 << (y%8));
  sprite[i] |= (color&1) << (y%8);
  sprite[i+1] &= ~(0x1 << (y%8));
  sprite[i+1] |= (1&(color>>1)) << (y%8);
}


void drawScreen() {
  arduboy.drawFastVLine(30,0,64);
  arduboy.drawFastVLine(95,0,64);
  arduboy.drawFastHLine(95,32,33);

  font6.setCursor(97,33);
  font6.print(F("X:"));
  font6.print(cursor.cursorX);
  font6.print(F(" Y:"));
  font6.print(cursor.cursorY);
  font6.print(F("\nS: "));
  font6.print(width);
  font6.print(F(" * "));
  font6.print(height);
  font6.print(F("\nF: "));
  font6.print(frame);
  font6.print(F(" / "));
  font6.print(frameCount);
  font6.print(F("\nC: "));
  font6.print(color);

  for (int x=0; x<16; x++) {
    for (int y=0; y<16; y++) {
      uint8_t color = readSprite(x,y);
      if (color==3) {
        arduboy.fillRect(cursor.offsetX+x*4,cursor.offsetY+y*4,4,4);
        arduboy.drawPixel(96+8+x,8+y);
      }
      else if (color==0) {
        arduboy.drawPixel(cursor.offsetX+x*4+1,cursor.offsetY+y*4+1);
      }
    }
  }
}
