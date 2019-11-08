#include "Menu.h"

extern Arduboy2 arduboy;
extern VarFont6 font6;

bool Menu::mainMenu(bool hasFocus) {
  font6.setCursor(1,1);
  font6.print(F("DRAW\n"
                "FRAME\n"
                "MODIFY\n"
                "PREVIEW\n"
                "NEW\n"
                "LOAD\n"
                "SAVE\n"
                "INFO\n"));
  highlight(mainSelection,0,mainMenuLeft,mainMenuRight);
  if (hasFocus) {
    return readSelectButtons(mainSelection,mainMenuItems);
  }
  return false;
}

bool Menu::subMenu() {
  uint8_t top;
  
  switch(mainSelection) {
    case mainFrame:
      top = mainSelection*highlightYSpacing;
      font6.setCursor(subMenuLeft+1,top);
      font6.print(F("CURRENT\n"
                    "COPY TO\n"
                    "SWAP WITH\n"
                    "TOTAL\n"));
      font6.setCursor(subMenuRight+5,top);
      font6.print(frameCurrent);
      font6.print('\n');
      font6.print(frameCopyTo);             
      font6.print('\n');
      font6.print(frameSwapWith);             
      font6.print('\n');
      font6.print(frameTotal);             
      highlight(subSelection,top,subMenuLeft,subMenuRight);
      readSelectButtons(subSelection,frameMenuItems);
      if (subSelection == subFrameCurrent) {
        readNumericButtons(frameCurrent,0,frameTotal-1);
      }
      else if (subSelection == subFrameTotal) {
        if (readNumericButtons(frameTotal,1,4)) {
          // set to safe values
          frameCurrent = 0;
          frameCopyTo = 0;
          frameSwapWith = 0;
        }
      }
      else if (subSelection == subFrameCopyTo) {
        readNumericButtons(frameCopyTo,0,frameTotal-1);
        return arduboy.justPressed(B_BUTTON);
      }
      else {
        readNumericButtons(frameSwapWith,0,frameTotal-1);
        return arduboy.justPressed(B_BUTTON);
      }
      return false;
    case mainInfo:
      font6.setCursor(subMenuLeft+1,1);      
      font6.print(F("ARDUBOY SPRITE\n"
                    "EDITOR (ASE)\n"
                    "Version 0.1\n\n"
                    "PAUL WASSON,\n"
                    "NOV 2019\n\n"
                    "GAME JAM 4\n"
                    "SUBMISSION"));
      return false;
  }
}


void Menu::highlight(uint8_t selection, uint8_t top, uint8_t left, uint8_t right) {
  uint8_t y = top + selection * highlightYSpacing;
  uint8_t offset = y & 0x7;
  for (uint8_t x=left; x < right; x++) {
    uint8_t column = 0xff << offset;
    uint8_t row = y>>3;
    uint16_t location = row*WIDTH+x;
    arduboy.sBuffer[location] ^= column;
    if ((offset > 0) && (row < (HEIGHT-1))) {
      arduboy.sBuffer[location+WIDTH] ^= ~column;  
    }
  }
}

bool Menu::readSelectButtons(uint8_t &selection, uint8_t selectionMax) {
  if (arduboy.justPressed(DOWN_BUTTON)) {
    selection += 1;
    if (selection >= selectionMax) {
      selection = 0;
    }
  }
  else if (arduboy.justPressed(UP_BUTTON)) {
    if (selection == 0) {
      selection = selectionMax-1;
    }
    else {
      selection--;
    }
  }
  return arduboy.justPressed(B_BUTTON);
}

bool Menu::readNumericButtons(uint8_t &value, uint8_t minValue, uint8_t maxValue) {
  uint8_t prev = value;
  if (arduboy.justPressed(LEFT_BUTTON)) {
    if (value > minValue) {
      value--;
    }
  }
  else if (arduboy.justPressed(RIGHT_BUTTON)) {
    if (value < maxValue) {
      value++;
    }
    return (value != prev);
  }
}
