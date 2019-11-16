
#include <Arduboy2.h>
#include <Sprites.h>
#include "VarFont6.h"
#include "Menu.h"
#include "SpriteBuffer.h"
#include "Canvas.h"


Arduboy2  arduboy;
SpriteBuffer sb;
VarFont6  font6;
Menu      menu;
Canvas    canvas;

static const uint8_t modeMainMenu = 0;
static const uint8_t modeCanvas = 1;
static const uint8_t modeSubMenu = 2;

uint8_t color;
uint8_t mode;

void setup() {
  arduboy.begin();
  arduboy.setFrameRate(60);
  Serial.begin(9600);
  mode = modeMainMenu;
  resetSprite();
}

void resetSprite() {
  // Clear buffer
  sb.clearSprite();

  // Set default parameters
  menu.sizeWidth = 16;
  menu.sizeHeight = 16;

  // Init structures
  menu.newSize();
  canvas.init();
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
    canvas.draw(mode == modeCanvas);
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
          sb.copyFrame(menu.frameCurrent,menu.frameCopyTo);
          menu.frameCurrent = menu.frameCopyTo;
        }
        else {
          sb.swapFrame(menu.frameCurrent,menu.frameSwapWith);
        }
        menu.subSelection = menu.subFrameCurrent;
      }
      else if (menu.mainSelection == menu.mainClear) {
        if (menu.subSelection == menu.subClearThis) {
          sb.clearFrame(menu.frameCurrent);
        }
        else {
          sb.clearSprite();
        }
        mode = modeMainMenu; // After clearing, return
      }
      else if (menu.mainSelection == menu.mainSave) {
        if (menu.subSelection == menu.subSerialEEPROM) {
          sb.saveEEPROM();
        }
        else {
          sb.save(menu.subSelection == menu.subSerialWithMask);
        }
        mode = modeMainMenu; // After saving return
      }
      else if (menu.mainSelection == menu.mainLoad) {
        uint16_t loadSize = 0;
        bool withMask = true;
        if (menu.subSelection == menu.subSerialExample) {
          loadSize = sb.loadExample();
        }
        else if (menu.subSelection == menu.subSerialEEPROM) {
          loadSize = sb.loadEEPROM();
        }
        else {
          withMask = menu.subSelection == menu.subSerialWithMask;
          loadSize = sb.load(withMask);
        }
        // Limited error checking
        if ((menu.sizeWidth < menu.minWidth) || (menu.sizeWidth > menu.maxWidth) || (menu.sizeHeight < menu.minHeight) || (menu.sizeHeight > menu.maxHeight)) {
          resetSprite();
        }
        else {
          menu.newSize();
          menu.frameTotal = loadSize/menu.frameSize(withMask);
        }
        canvas.init();
        mode = modeMainMenu; // After loading
      }
      else if (menu.mainSelection == menu.mainSize) {
        menu.newSize();
        canvas.init();
      }
      else if (menu.mainSelection == menu.mainModify) {
        if (menu.modifyTransform == menu.modifyTransformFlip) {
          sb.flipFrame(menu.frameCurrent);
        }
        else if (menu.modifyTransform == menu.modifyTransformInsert) {
          sb.insertLine(menu.frameCurrent);
        }
        else {
          sb.deleteLine(menu.frameCurrent);
        }
      }
    }
    if (arduboy.justPressed(A_BUTTON)) {
      mode = modeMainMenu;
    }
  }
  else {
    bool moved = canvas.directionalButtons();  

    if (arduboy.justPressed(B_BUTTON)) {
      color = sb.readSprite(menu.frameCurrent, canvas.cursorX,canvas.cursorY);
      if (color == 0) {
        color = 3;
      }
      else if (color == 3) {
        color = 2;
      }
      else {
        color = 0;
      }
      sb.writeSprite(menu.frameCurrent, canvas.cursorX, canvas.cursorY, color);
    } 
    else if (arduboy.pressed(B_BUTTON) && moved) {
      sb.writeSprite(menu.frameCurrent, canvas.cursorX, canvas.cursorY, color);
    }

    if (arduboy.justPressed(A_BUTTON)) {
      mode = modeMainMenu;
    }
  }
  arduboy.display();
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
  font6.print(canvas.cursorX);
  font6.print(F(" Y:"));
  font6.print(canvas.cursorY);
  font6.print(F("\nS: "));
  font6.print(menu.sizeWidth);
  font6.print(F("*"));
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
  uint8_t xoffset=(32-menu.sizeWidth)/2;
  uint8_t yoffset=(32-menu.sizeHeight)/2;
  uint16_t xpad = menu.sizeWidth * 16;
  uint16_t ypad = menu.sizeHeight * 16;
  
  for (uint8_t x=0; x<32; x++) {
    if (menu.previewTile || ((x >= xoffset) && (x<(xoffset+menu.sizeWidth)))) {
      for (uint8_t y=0; y<32; y++) {
        if (menu.previewTile || ((y >= yoffset) && (y<(yoffset+menu.sizeHeight)))) {
          uint8_t color = sb.readSprite(frame,(xpad+x-xoffset)%menu.sizeWidth,(ypad+y-yoffset)%menu.sizeHeight);
          if (color>=2) {
            arduboy.drawPixel(96+x,y,color==3 ? WHITE : BLACK);        
          }
        }
      }
    }
  }
  // Modify cursor
  if ((mode == modeSubMenu) && (menu.mainSelection == menu.mainModify) && (arduboy.frameCount % 32 < 16)) {
    if ((menu.modifyDirection == menu.modifyDirectionUp) || (menu.modifyDirection == menu.modifyDirectionDown)) {
      uint8_t y = canvas.cursorY + yoffset;
      for (uint8_t x=96; x < 128; x++) {
        arduboy.sBuffer[(y>>3) * WIDTH + x] ^= 1 << (y%8); 
      }
    }
    else {
      uint8_t x = 96 + canvas.cursorX + xoffset;
      for (uint8_t y=0; y < 4; y++) {
        arduboy.sBuffer[y * WIDTH + x] ^= 0xff; 
      }
    }
  }
}
