#include "Menu.h"

extern Arduboy2 arduboy;
extern VarFont6 font6;

void Menu::draw() {
  font6.setCursor(1,1);
  font6.print(F("DRAW\n"
                "UNDO\n"
                "XFORM\n"
                "FRAME\n"
                "PREVIEW\n"
                "NEW\n"
                "LOAD\n"
                "SAVE\n"
                "INFO\n"));
  invertBox();
}

void Menu::invertBox() {
  uint8_t y = selection * boxYSpacing;
  uint8_t offset = y & 0x7;
  for (uint8_t x=boxX; x < boxX+boxWidth; x++) {
    uint8_t column = 0xff << offset;
    uint8_t row = y>>3;
    uint16_t location = row*WIDTH+x;
    arduboy.sBuffer[location] ^= column;
    if ((offset > 0) && (row < (HEIGHT-1))) {
      arduboy.sBuffer[location+WIDTH] ^= ~column;  
    }
  }
}

void Menu::readButtons() {
  if (arduboy.justPressed(DOWN_BUTTON)) {
    selection += 1;
    if (selection >= selectionCount) {
      selection = 0;
    }
  }
  else if (arduboy.justPressed(UP_BUTTON)) {
    if (selection == 0) {
      selection = selectionCount-1;
    }
    else {
      selection--;
    }
  }
}
