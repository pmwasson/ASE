
#include <Arduboy2.h>
#include "BoxCursor.h"
#include "VarFont6.h"
#include "Menu.h"

Arduboy2  arduboy;
BoxCursor cursor;
VarFont6  font6;
Menu      menu;

static const uint8_t modeMainMenu = 0;
static const uint8_t modeCanvas = 1;
static const uint8_t modeSubMenu = 2;

uint8_t width;
uint8_t height;
uint8_t sprite[256];
uint8_t color;
uint8_t mode;

void setup() {
  arduboy.begin();
  arduboy.setFrameRate(30);
  cursor.setOffset(31,0);
  cursor.setPos(0,0);
  cursor.setSize(16,16);
  Serial.begin(9600);
  mode = modeMainMenu;
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
  drawCanvas(mode == modeSubMenu);
  
  bool performAction = menu.mainMenu(mode == modeMainMenu);
  
  if (mode == modeMainMenu) {
    if (performAction) {
      if (menu.mainSelection == menu.mainDraw) {
        mode = modeCanvas;
      }
      else {
        mode = modeSubMenu;
        menu.subSelection = 0;
      }
    }
  }
  else if (mode == modeSubMenu) {
    if (menu.subMenu()) {
      if (menu.mainSelection == menu.mainFrame) {
        if (menu.subSelection == menu.subFrameCopyTo) {
          copyFrame(menu.frameCurrent,menu.frameCopyTo);
          menu.frameCurrent = menu.frameCopyTo;
        }
        else {
          swapFrame(menu.frameCurrent,menu.frameSwapWith);
        }
        menu.subSelection = menu.subFrameCurrent;
      }
      else if (menu.mainSelection == menu.mainClear) {
        if (menu.subSelection == menu.subClearThis) {
          clearFrame(menu.frameCurrent);
        }
        else {
          clearSprite();
        }
        mode = modeMainMenu; // After clearing, return
      }
    }
    if (arduboy.justPressed(A_BUTTON)) {
      mode = modeMainMenu;
    }
  }
  else {
    cursor.draw();

    bool moved = cursor.directionalButtons();  

    if (arduboy.justPressed(B_BUTTON)) {
      color = readSprite(menu.frameCurrent, cursor.cursorX,cursor.cursorY);
      if (color == 0) {
        color = 3;
      }
      else if (color == 3) {
        color = 2;
      }
      else {
        color = 0;
      }
      writeSprite(menu.frameCurrent, cursor.cursorX, cursor.cursorY, color);
    } 
    else if (arduboy.pressed(B_BUTTON) && moved) {
      writeSprite(menu.frameCurrent, cursor.cursorX, cursor.cursorY, color);
    }

    if (arduboy.justPressed(A_BUTTON)) {
      mode = modeMainMenu;
    }
}
  


  arduboy.display();
}

uint16_t spriteIndex(uint8_t frame, uint8_t x, uint8_t y) {
  return (2*(frame*menu.sizeWidth*menu.sizeHeight/8+x+(y/8)*menu.sizeWidth));
}

uint8_t readSprite(uint8_t frame, uint8_t x, uint8_t y) {
  uint16_t i = spriteIndex(frame,x,y);
  return (1 & (sprite[i] >> (y % 8))) +
         ((1 & (sprite[i+1] >> (y % 8))) << 1);
  
}

void writeSprite(uint8_t frame, uint8_t x, uint8_t y, uint8_t color) {
  uint16_t i = spriteIndex(frame,x,y);
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
  font6.print(menu.sizeWidth);
  font6.print(F(" * "));
  font6.print(menu.sizeHeight);
  font6.print(F("\nF: "));
  font6.print(menu.frameCurrent);
  font6.print(F(" ("));
  font6.print(menu.frameTotal);
  font6.print(F(")\nC: "));
  font6.print(color);
}

void drawCanvas(bool previewOnly) {
  for (int x=0; x<menu.sizeWidth; x++) {
    for (int y=0; y<menu.sizeHeight; y++) {
      uint8_t color = readSprite(menu.frameCurrent,x,y);
      if (color==3) {
        arduboy.drawPixel(96+8+x,8+y);        
      }
      if (!previewOnly) {
        if (color==3) {
          arduboy.fillRect(cursor.offsetX+x*4,cursor.offsetY+y*4,4,4);
        }
        else if (color==0) {
          arduboy.drawPixel(cursor.offsetX+x*4+1,cursor.offsetY+y*4+1);
        }
      }
    }
  }
}

void clearFrame(uint8_t frame) {
  for (int x=0; x<menu.sizeWidth; x++) {
    for (int y=0; y<menu.sizeHeight; y++) {
      writeSprite(frame,x,y,0);
    }
  }
}

void copyFrame(uint8_t fromFrame, uint8_t toFrame) {
  for (int x=0; x<menu.sizeWidth; x++) {
    for (int y=0; y<menu.sizeHeight; y++) {
      writeSprite(toFrame,x,y,readSprite(fromFrame,x,y));
    }
  }
}

void swapFrame(uint8_t frameA, uint8_t frameB) {
  for (int x=0; x<menu.sizeWidth; x++) {
    for (int y=0; y<menu.sizeHeight; y++) {
      uint8_t temp = readSprite(frameA,x,y);
      writeSprite(frameA,x,y,readSprite(frameB,x,y));
      writeSprite(frameB,x,y,temp);
    }
  }
}
