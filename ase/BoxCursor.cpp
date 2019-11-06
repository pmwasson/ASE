#include "BoxCursor.h"

extern Arduboy2 arduboy;

BoxCursor::BoxCursor() {
  repeatCount = 0;
}

void BoxCursor::setOffset(const uint8_t x, const uint8_t y) {
  offsetX = x;
  offsetY = y;
}

void BoxCursor::setPos(const uint8_t x, const uint8_t y) {
  cursorX = x;
  cursorY = y;
}

void BoxCursor::setSize(const uint8_t x, const uint8_t y) {
  width = x;
  height = y;
}

bool BoxCursor::directionalButtons() {
  if (arduboy.justPressed(LEFT_BUTTON)) {
    left();
    return true;
  }
  else if (arduboy.pressed(LEFT_BUTTON)) {
    if (repeatCount++ > REPEAT_LIMIT) {
      repeatCount = REPEAT_LIMIT;
      left();
      return true;
    }
  }
  else if (arduboy.justPressed(RIGHT_BUTTON)) {
    right();
    return true;
  }
  else if (arduboy.pressed(RIGHT_BUTTON)) {
    if (repeatCount++ > REPEAT_LIMIT) {
      repeatCount = REPEAT_LIMIT;
      right();
      return true;
    }
  }
  else if (arduboy.justPressed(UP_BUTTON)) {
    up();
    return true;
  }
  else if (arduboy.pressed(UP_BUTTON)) {
    if (repeatCount++ > REPEAT_LIMIT) {
      repeatCount = REPEAT_LIMIT;
      up();
      return true;
    }
  }
  else if (arduboy.justPressed(DOWN_BUTTON)) {
    down();
    return true;
  }
  else if (arduboy.pressed(DOWN_BUTTON)) {
    if (repeatCount++ > REPEAT_LIMIT) {
      repeatCount = REPEAT_LIMIT;
      down();
      return true;
    }
  }
  else {
    repeatCount = 0;
  }
  return false;
}

void BoxCursor::left() {
  if (cursorX == 0) {
    cursorX = width-1;
  }
  else {
    cursorX--;
  }
}

void BoxCursor::right() {
  if (cursorX >= width-1) {
    cursorX = 0;
  }
  else {
    cursorX++;
  }
}

void BoxCursor::up() {
  if (cursorY == 0) {
    cursorY = height-1;
  }
  else {
    cursorY--;
  }
}

void BoxCursor::down() {
  if (cursorY >= height-1) {
    cursorY = 0;
  }
  else {
    cursorY++;
  }
}

void BoxCursor::draw() {
  arduboy.drawRect(offsetX+cursorX*4,offsetY+cursorY*4,4,4,(arduboy.frameCount&0x1f) < 16 ? WHITE : BLACK);
  //arduboy.drawRect(offsetX+cursorX*4,offsetY+cursorY*4,4,4);
}
