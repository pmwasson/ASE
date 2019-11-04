#include "Cursor.h"

extern Arduboy2 arduboy;

Cursor::Cursor() {
  this->count = 0;
  this->repeatCount = 0;
}

void Cursor::setSize(uint8_t swidth, uint8_t sheight) {
  width = swidth;
  height = sheight;
}

void Cursor::setPos(uint8_t px, uint8_t py) {
  x = px;
  y = py;
}

void Cursor::directionalButtons() {
  if (arduboy.justPressed(LEFT_BUTTON)) {
    left();
  }
  else if (arduboy.pressed(LEFT_BUTTON)) {
    if (repeatCount++ > REPEAT_LIMIT) {
      repeatCount = REPEAT_LIMIT;
      left();
    }
  }
  else if (arduboy.justPressed(RIGHT_BUTTON)) {
    right();
  }
  else if (arduboy.pressed(RIGHT_BUTTON)) {
    if (repeatCount++ > REPEAT_LIMIT) {
      repeatCount = REPEAT_LIMIT;
      right();
    }
  }
  else if (arduboy.justPressed(UP_BUTTON)) {
    up();
  }
  else if (arduboy.pressed(UP_BUTTON)) {
    if (repeatCount++ > REPEAT_LIMIT) {
      repeatCount = REPEAT_LIMIT;
      up();
    }
  }
  else if (arduboy.justPressed(DOWN_BUTTON)) {
    down();
  }
  else if (arduboy.pressed(DOWN_BUTTON)) {
    if (repeatCount++ > REPEAT_LIMIT) {
      repeatCount = REPEAT_LIMIT;
      down();
    }
  }
  else {
    repeatCount = 0;
  }
}

void Cursor::draw() {
  if (count++ < BLINK_RATE) {
    arduboy.fillRect(x,y,width,height);
  }
  else {
    if (count > BLINK_RATE*2) {
      count=0;
    }
  }
}
