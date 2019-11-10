
#include <Arduboy2.h>
#include <Sprites.h>
#include "BoxCursor.h"
#include "VarFont6.h"
#include "Menu.h"
#include "IncludeSprites.h"

Arduboy2  arduboy;
BoxCursor cursor;
VarFont6  font6;
Menu      menu;

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
  arduboy.setFrameRate(60);
  cursor.setOffset(31,0);
  cursor.setPos(0,0);
  cursor.setSize(16,16);
  Serial.begin(9600);
  mode = modeMainMenu;
  resetSprite();
}

void resetSprite() {
  menu.sizeWidth = 16;
  menu.sizeHeight = 16;
  menu.newSize();
  clearSprite();
}

void clearSprite() {
  for (int i=0; i<menu.bufferSize; i++) {
    sprite[i] = 0;
  }  
}


void loop() {
  if (!(arduboy.nextFrame()))
    return;

  // Set up
  arduboy.pollButtons();
  arduboy.clear();
  drawScreen();
  drawPreview();
  if (mode != modeSubMenu) {
    drawCanvas();
  }

  // Animate
  menu.previewAnimateOffset += arduboy.everyXFrames(41 - menu.previewAnimate*4);

  // Menu
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
        if (menu.subSelection == menu.subSerialExample) {
          loadExample();
        }
        else {
          load(menu.subSelection == menu.subSerialWithMask);
        }
        mode = modeMainMenu; // After loading
      }
      else if (menu.mainSelection == menu.mainSize) {
        menu.newSize();
      }
      else if (menu.mainSelection == menu.mainModify) {
        if (menu.modifyTransform == menu.modifyTransformFlip) {
          flipFrame(menu.frameCurrent);
        }
        else if (menu.modifyTransform == menu.modifyTransformInsert) {
          insertLine(menu.frameCurrent);
        }
        else {
          deleteLine(menu.frameCurrent);
        }
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

  if (menu.previewBackground > menu.previewBackgroundBlack) {
    uint8_t color = (menu.previewBackground == menu.previewBackgroundWhite) ? 0xff: 0x00;
    for (uint8_t y=0; y<4; y++) {
      for (uint8_t x=96; x<128; x++) {
        arduboy.sBuffer[x+y*WIDTH] = color | (x%2 ? 0x55 : 0xaa);
      }
    }
  }
  
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
  font6.print(F(")\nAF: "));
  font6.print(menu.previewFrame());
}

void drawPreview() {  
  uint8_t frame = menu.previewFrame();
  uint8_t xoffset=16-menu.sizeWidth/2;
  uint8_t yoffset=16-menu.sizeHeight/2;
  
  for (uint8_t x=0; x<32; x++) {
    if (menu.previewTile || ((x >= xoffset) && (x<(xoffset+menu.sizeWidth)))) {
      for (uint8_t y=0; y<32; y++) {
        if (menu.previewTile || ((y >= yoffset) && (y<(yoffset+menu.sizeHeight)))) {
          uint8_t color = readSprite(frame,(32+x-xoffset)%menu.sizeWidth,(32+y-yoffset)%menu.sizeHeight);
          if (color>=2) {
            arduboy.drawPixel(96+x,y,color==3 ? WHITE : BLACK);        
          }
        }
      }
    }
  }
  // Modify cursor
  if ((mode == modeSubMenu) && (menu.mainSelection == menu.mainModify)) {
    if ((menu.modifyDirection == menu.modifyDirectionUp) || (menu.modifyDirection == menu.modifyDirectionDown)) {
      uint8_t y = cursor.cursorY + yoffset;
      for (uint8_t x=96; x < 128; x++) {
        arduboy.sBuffer[(y>>3) * WIDTH + x] ^= 1 << (y%8); 
      }
    }
    else {
      uint8_t x = 96 + cursor.cursorX + xoffset;
      for (uint8_t y=0; y < 4; y++) {
        arduboy.sBuffer[y * WIDTH + x] ^= 0xff; 
      }
    }
  }
}

void drawCanvas() {
  for (int x=0; x<menu.sizeWidth; x++) {
    for (int y=0; y<menu.sizeHeight; y++) {
      uint8_t color = readSprite(menu.frameCurrent,x,y);
      if (color==3) {
        arduboy.fillRect(cursor.offsetX+x*4,cursor.offsetY+y*4,4,4);
      }
      else if (color==0) {
        arduboy.drawPixel(cursor.offsetX+x*4+1,cursor.offsetY+y*4+1);
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

void save(bool withMask) {
  uint16_t offset = 0;
  uint8_t increment = withMask ? 1 : 2;
  uint16_t loopSize = menu.frameSize(withMask);
  Serial.print(F("const unsigned char PROGMEM sprite"));
  if (withMask) {
    Serial.print(F("_and_mask"));  
  }
  Serial.print(F("[] = {\n"));
  Serial.print(F("// For load, cut and paste between START and END, and send over serial port\n"));
  if (withMask) {
    Serial.print(F("// then choose LOAD WITH MASK\n"));
  }
  else {
    Serial.print(F("// then choose LOAD NO MASK\n"));    
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

 */

uint16_t load(bool withMask) {
  
  menu.sizeWidth = Serial.parseInt();
  menu.sizeHeight = Serial.parseInt();
  menu.frameCurrent = 0;
  
  uint16_t offset = 0;
  while(Serial.available() && (offset < menu.bufferSize)) {
    sprite[offset++] = Serial.parseInt();
    if (!withMask) {
      sprite[offset] = sprite[offset-1];
      offset++;
    }
  }

  // Limited error checking
  if ((menu.sizeWidth < menu.minWidth) || (menu.sizeWidth > menu.maxWidth) || (menu.sizeHeight < menu.minHeight) || (menu.sizeHeight > menu.maxHeight)) {
    resetSprite();
  }
  
  menu.newSize();
  menu.frameTotal = offset/menu.frameSize(withMask);
  return(offset);
}

void loadExample() {  
  menu.sizeWidth = (uint8_t)pgm_read_byte_near(exampleSprite+0);
  menu.sizeHeight = (uint8_t)pgm_read_byte_near(exampleSprite+1);
  menu.frameCurrent = 0;
  menu.newSize();
  menu.frameTotal = exampleSpriteFrames;
  
  for (uint16_t offset=0; offset < (2 + menu.frameSize(true) * menu.frameTotal); offset++) {
    sprite[offset] = (uint8_t)pgm_read_byte_near(exampleSprite+offset+2);
  }
}

void flipFrame(uint8_t frame) {
  if ((menu.modifyDirection == menu.modifyDirectionRight) || (menu.modifyDirection == menu.modifyDirectionLeft)) {
    for (uint8_t y=0; y < menu.sizeHeight; y++) {
      for (uint8_t x=0; x < menu.sizeWidth/2; x++) {
        uint8_t otherX = menu.sizeWidth - x - 1;
        uint8_t temp = readSprite(menu.frameCurrent,x,y);
        writeSprite(frame,x,y,readSprite(frame,otherX,y));
        writeSprite(frame,otherX,y,temp);
      }
    }      
  }
  else {
    for (uint8_t x=0; x < menu.sizeWidth; x++) {
      for (uint8_t y=0; y < menu.sizeHeight/2; y++) {
        uint8_t otherY = menu.sizeHeight - y - 1;
        uint8_t temp = readSprite(menu.frameCurrent,x,y);
        writeSprite(frame,x,y,readSprite(frame,x,otherY));
        writeSprite(frame,x,otherY,temp);
      }
    }      
  }
}


void deleteLine(uint8_t frame) {
  if (menu.modifyDirection == menu.modifyDirectionRight) {
    for (int8_t y=0; y < menu.sizeHeight; y++) {
      for (int8_t x=cursor.cursorX; x < menu.sizeWidth; x++) {
        uint8_t temp = x < (menu.sizeWidth-1) ? readSprite(menu.frameCurrent,x+1,y) : 0;
        writeSprite(frame,x,y,temp);
      }
    }      
  }
  else if (menu.modifyDirection == menu.modifyDirectionLeft) {
    for (int8_t y=0; y < menu.sizeHeight; y++) {
      for (int8_t x=cursor.cursorX; x >= 0; x--) {
        uint8_t temp = x > 0 ? readSprite(menu.frameCurrent,x-1,y) : 0;
        writeSprite(frame,x,y,temp);
      }
    }      
  }
  else if (menu.modifyDirection == menu.modifyDirectionDown) {
    for (int8_t x=0; x < menu.sizeWidth; x++) {
      for (int8_t y=cursor.cursorY; y < menu.sizeHeight; y++) {
        uint8_t temp = y < (menu.sizeHeight-1) ? readSprite(menu.frameCurrent,x,y+1) : 0;
        writeSprite(frame,x,y,temp);
      }
    }      
  }
  else if (menu.modifyDirection == menu.modifyDirectionUp) {
    for (int8_t x=0; x < menu.sizeWidth; x++) {
      for (int8_t y=cursor.cursorY; y >= 0; y--) {
        uint8_t temp = y > 0 ? readSprite(menu.frameCurrent,x,y-1) : 0;
        writeSprite(frame,x,y,temp);
      }
    }
  }
}

void insertLine(uint8_t frame) {
  if (menu.modifyDirection == menu.modifyDirectionRight) {
    for (int8_t y=0; y < menu.sizeHeight; y++) {
      for (int8_t x=menu.sizeWidth-1; x > cursor.cursorX; x--) {
        uint8_t temp = readSprite(menu.frameCurrent,x-1,y);
        writeSprite(frame,x,y,temp);
      }
    }      
  }
  else if (menu.modifyDirection == menu.modifyDirectionLeft) {
    for (int8_t y=0; y < menu.sizeHeight; y++) {
      for (int8_t x=0; x < cursor.cursorX; x++) {
        uint8_t temp = readSprite(menu.frameCurrent,x+1,y);
        writeSprite(frame,x,y,temp);
      }
    }      
  }
  else if (menu.modifyDirection == menu.modifyDirectionDown) {
    for (int8_t x=0; x < menu.sizeWidth; x++) {
      for (int8_t y=menu.sizeHeight-1; y > cursor.cursorY; y--) {
        uint8_t temp = readSprite(menu.frameCurrent,x,y-1);
        writeSprite(frame,x,y,temp);
      }
    }      
  }
  else if (menu.modifyDirection == menu.modifyDirectionUp) {
    for (int8_t x=0; x < menu.sizeWidth; x++) {
      for (int8_t y=0; y < cursor.cursorY; y++) {
        uint8_t temp = readSprite(menu.frameCurrent,x,y+1);
        writeSprite(frame,x,y,temp);
      }
    }
  }
}
