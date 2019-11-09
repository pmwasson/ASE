
#include <Arduboy2.h>
#include "BoxCursor.h"
#include "VarFont6.h"
#include "Menu.h"

Arduboy2  arduboy;
BoxCursor cursor;
VarFont6  font6;
Menu      menu;

static const uint16_t bufferSize = 512;
static const uint8_t modeMainMenu = 0;
static const uint8_t modeCanvas = 1;
static const uint8_t modeSubMenu = 2;

uint8_t width;
uint8_t height;
uint8_t sprite[512];
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
  for (int i=0; i<bufferSize; i++) {
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
      else if (menu.mainSelection == menu.mainSave) {
        save(menu.subSelection == menu.subSerialWithMask);
        mode = modeMainMenu; // After saving return
      }
      else if (menu.mainSelection == menu.mainLoad) {
        load(menu.subSelection == menu.subSerialWithMask);
        mode = modeMainMenu; // After loading
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
  return (2*(frame*menu.sizeWidth*((menu.sizeHeight+7)>>3)+x+(y/8)*menu.sizeWidth));
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

uint16_t frameSize(bool withMask) {
  return (menu.sizeHeight+7)/8 * menu.sizeWidth * (withMask+1);
}
void save(bool withMask) {
  uint16_t offset = 0;
  uint8_t increment = withMask ? 1 : 2;
  uint16_t loopSize = frameSize(withMask);
  Serial.print(F("const unsigned char PROGMEM sprite[] = {\n"));
  Serial.print(F("// For load, cut and paste between START and END\n"));
  if (withMask) {
    Serial.print(F("// Choose LOAD WITH MASK\n"));
  }
  else {
    Serial.print(F("// Choose LOAD NO MASK\n"));    
  }
  Serial.print(F("// START\n"));
  Serial.print(menu.sizeWidth);
  Serial.print(F(","));
  Serial.print(menu.sizeHeight);
  for (uint8_t frame=0; frame < menu.frameTotal; frame++) {
    Serial.println(F(","));
    Serial.print(sprite[offset]);
    offset += increment;
    for (uint16_t i=0 ; i < loopSize-1; i++) {
      Serial.print(F(","));
      Serial.print(sprite[offset]);
      offset += increment;
    }
  }
  Serial.println(F("\n// END\n};"));
}

/* Test:
const unsigned char PROGMEM sprite[] = {
// For load, cut and paste between START and END
// Choose LOAD WITH MASK
// START
16,16,
0,0,0,0,192,192,160,224,99,227,174,254,170,254,87,255,87,255,170,254,174,254,99,227,160,224,192,192,0,0,0,0,0,0,24,24,27,31,7,15,192,199,223,255,253,255,5,135,5,7,253,255,223,255,192,199,7,15,27,31,24,24,0,0,
0,0,0,0,192,192,160,224,99,227,174,254,170,254,87,255,87,255,170,254,174,254,99,227,160,224,192,192,0,0,0,0,0,0,24,24,27,31,7,15,192,199,223,255,253,255,5,135,5,7,253,255,223,255,192,199,7,15,27,31,24,24,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,8,8,24,24,28,28,60,60,62,62,126,126,127,127,255,255,255,255,127,127,126,126,62,62,60,60,28,28,24,24,8,8,
248,248,24,248,28,252,60,252,62,254,126,254,127,255,127,255,255,255,127,255,126,255,62,254,60,252,28,252,24,248,248,248,15,15,24,31,24,31,60,63,60,63,126,127,126,127,255,255,254,255,126,127,126,127,60,63,60,63,24,31,24,31,15,15
// END


const unsigned char PROGMEM spriteEyeball[] = {
// For load, cut and paste between START and END
// Choose LOAD WITH MASK
// START
16,16,
120,248,140,252,228,252,246,254,162,254,91,255,169,255,21,255,13,255,21,255,173,255,89,255,170,254,242,254,68,252,24,248,3,3,6,7,5,7,13,15,10,15,27,31,22,31,21,31,22,31,21,31,22,31,23,31,26,31,13,15,6,7,3,3,
120,248,188,252,220,252,238,254,174,254,87,255,167,255,23,255,7,255,23,255,167,255,87,255,174,254,238,254,92,252,24,248,3,3,6,7,5,7,13,15,10,15,27,31,22,31,21,31,22,31,21,31,22,31,23,31,26,31,13,15,6,7,3,3,
248,248,252,252,124,252,126,254,190,254,63,255,191,255,63,255,63,255,63,255,191,255,63,255,190,254,190,190,60,188,248,248,3,3,6,7,5,7,13,15,10,15,27,31,22,31,21,31,22,31,21,31,22,31,23,31,26,31,13,15,6,7,3,3,
248,248,252,252,252,252,254,254,254,254,255,255,255,255,255,255,255,255,255,255,255,255,255,255,254,254,254,254,252,252,120,248,2,3,6,7,5,7,13,15,11,15,27,31,23,31,23,31,23,31,23,31,23,31,23,31,27,31,13,15,6,7,3,3
// END
};

};
 */

uint16_t load(bool withMask) {
  
  menu.sizeWidth = Serial.parseInt();
  menu.sizeHeight = Serial.parseInt();
  menu.frameCurrent = 0;
  
  uint16_t offset = 0;
  while(Serial.available() && (offset < bufferSize)) {
    sprite[offset++] = Serial.parseInt();
    if (!withMask) {
      sprite[offset] = sprite[offset-1];
      offset++;
    }
  }

  menu.frameTotal = offset/frameSize(withMask);

  return(offset);
}
