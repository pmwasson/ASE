#include "Canvas.h"
#include "SpriteBuffer.h"
#include "Menu.h"

extern Arduboy2 arduboy;
extern SpriteBuffer sb;
extern Menu menu;

void Canvas::init() {
  offsetX = (menu.sizeWidth - canvasWidth)/2;
  offsetY = (menu.sizeHeight - canvasHeight)/2;
  cursorX = menu.sizeWidth/2;
  cursorY = menu.sizeHeight/2;
  repeatCount = 0;
}

void Canvas::draw(bool active) {
  uint8_t color = colorChecker;
  for (int x=0; x< canvasWidth; x++) {
    for (int y=0; y< canvasHeight; y++) {
      uint8_t color = colorChecker;

      if (inSprite(x,y)) {
        color = sb.readSprite(menu.frameCurrent,x+offsetX,y+offsetY);
      }

      // Overwrite color with scroll bars
      if ( ((x == 0) && (offsetX > 0)) ||
                ((x == canvasWidth-1) && (offsetX + canvasWidth < menu.sizeWidth)) ) {
        color = (y==0 || y==canvasHeight-1) ? colorBlack : colorVerticalBar;
      }
      else if ( ((y == 0) && (offsetY > 0)) ||
                ((y == canvasHeight-1) && (offsetY + canvasHeight < menu.sizeHeight)) ) {
        color = (x==0 || x==canvasWidth-1) ? colorBlack : colorHorizontalBar;
      }

      drawSquare(color,x,y);
    }
  }

  // Draw cursor if active
  if (active) {
    arduboy.drawRect(screenX+(cursorX-offsetX)*4,screenY+(cursorY-offsetY)*4,4,4,(arduboy.frameCount%32) < 16 ? WHITE : BLACK);
  }
}

bool Canvas::inSprite(uint8_t x, uint8_t y) {
  return ((x+offsetX >= 0) && (y+offsetY >= 0) && (x+offsetX < menu.sizeWidth) && (y+offsetY < menu.sizeHeight));
}

void Canvas::drawSquare(uint8_t color, uint8_t x, uint8_t y) {
  uint8_t mask = (y % 2) ? 0xf : 0xf0;
  uint8_t set[4] = {0,0,0,0};
  
  if (color < colorBlack) {
    set[2] = 0x22; // Transparent
  }
  else if (color == colorBlack) {
    set[0] = set[1] = set[2] = set[3] = 0;
  }
  else if (color == colorWhite) {
    set[0] = set[1] = set[2] = set[3] = 0xff;
  }
  else if (color == colorChecker) {
    set[0] = set[2] = 0x55;
    set[1] = set[3] = 0xaa;
  }
  else if (color == colorVerticalBar) {
    set[0] = 0x22;
    set[1] = 0x55;
    set[2] = 0x88;
  }
  else {
    set[0] = 0x22;
    set[1] = 0x11;
    set[2] = 0x22;
    set[3] = 0x44;
  }
     
  uint16_t offset = (screenY+y>>1)*WIDTH+x*4+screenX;
  for (uint8_t i=0; i<4; i++) {
    arduboy.sBuffer[offset+i] = (arduboy.sBuffer[offset+i]&mask)| (set[i]&~mask);
  }
}


bool Canvas::directionalButtons() {
  if (arduboy.justPressed(LEFT_BUTTON)) {
    left();
    return true;
  }
  else if (arduboy.pressed(LEFT_BUTTON)) {
    if (repeatCount++ > repeatLimit) {
      repeatCount = repeatLimit;
      left();
      return true;
    }
  }
  else if (arduboy.justPressed(RIGHT_BUTTON)) {
    right();
    return true;
  }
  else if (arduboy.pressed(RIGHT_BUTTON)) {
    if (repeatCount++ > repeatLimit) {
      repeatCount = repeatLimit;
      right();
      return true;
    }
  }
  else if (arduboy.justPressed(UP_BUTTON)) {
    up();
    return true;
  }
  else if (arduboy.pressed(UP_BUTTON)) {
    if (repeatCount++ > repeatLimit) {
      repeatCount = repeatLimit;
      up();
      return true;
    }
  }
  else if (arduboy.justPressed(DOWN_BUTTON)) {
    down();
    return true;
  }
  else if (arduboy.pressed(DOWN_BUTTON)) {
    if (repeatCount++ > repeatLimit) {
      repeatCount = repeatLimit;
      down();
      return true;
    }
  }
  else {
    repeatCount = 0;
  }
  return false;
}

void Canvas::left() {
  
  if (cursorX == 0) {
    cursorX = menu.sizeWidth-1;
    if (menu.sizeWidth > canvasWidth) {
      offsetX = cursorX - (canvasWidth-1);
    }
  }
  else {
    cursorX--;
    if ((cursorX - offsetX == 0) && (offsetX > 0)) {
      offsetX--;
    }
  }
}

void Canvas::right() {
  if (cursorX == menu.sizeWidth-1) {
    cursorX = 0;
    if (menu.sizeWidth > canvasWidth) {
      offsetX = 0;
    }
  }
  else {
    cursorX++;
    if ((cursorX - offsetX == canvasWidth-1) && (cursorX < menu.sizeWidth-1)) {
      offsetX++;
    }
  }
}

void Canvas::up() {
  if (cursorY == 0) {
    cursorY = menu.sizeHeight-1;
    if (menu.sizeHeight > canvasHeight) {
      offsetY = cursorY - (canvasHeight-1);
    }
  }
  else {
    cursorY--;
  }
  if ((cursorY - offsetY == 0) && (offsetY > 0)) {
      offsetY--;
  }
}

void Canvas::down() {
  if (cursorY == menu.sizeHeight-1) {
    cursorY = 0;
    if (menu.sizeHeight > canvasHeight) {
      offsetY = 0;
    }
  }
  else {
    cursorY++;
    if ((cursorY - offsetY == canvasHeight-1) && (cursorY < menu.sizeHeight-1)) {
      offsetY++;
    }
  }
}
