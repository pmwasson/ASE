#include "Menu.h"

extern Arduboy2 arduboy;
extern VarFont6 font6;

bool Menu::mainMenu(bool hasFocus) {
  font6.setCursor(1,1);
  font6.print(F("DRAW\n"
                "FRAME\n"
                "MODIFY\n"
                "PREVIEW\n"
                "SIZE\n"
                "CLEAR\n"
                "LOAD\n"
                "SAVE\n"
                "INFO\n"));
  highlight(mainSelection,0,mainMenuLeft,mainMenuRight,hasFocus);
  if (hasFocus) {
    return readSelectButtons(mainSelection,mainMenuItems);
  }
  return false;
}

bool Menu::subMenu() {
  uint8_t top = mainSelection*highlightYSpacing;
  
  switch(mainSelection) {
    case mainFrame:
      font6.setCursor(subMenuLeft+1,top+1);
      font6.print(F("CURRENT\n"
                    "COPY TO\n"
                    "SWAP WITH\n"
                    "TOTAL"));
      font6.setCursor(subMenuRight+5,top);
      font6.print(frameCurrent);
      font6.print('\n');
      font6.print(frameCopyTo);             
      font6.print('\n');
      font6.print(frameSwapWith);             
      font6.print('\n');
      font6.print(frameTotal);             
      highlight(subSelection,top,subMenuLeft,subMenuRight,true);
      readSelectButtons(subSelection,frameMenuItems);
      if (subSelection == subFrameCurrent) {
        readNumericButtons(frameCurrent,0,frameTotal-1);
      }
      else if (subSelection == subFrameTotal) {
        if (readNumericButtons(frameTotal,1,frameMax)) {
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
    case mainSize:  
      font6.setCursor(subMenuLeft+1,top+1);
      font6.print(F("WIDTH\n"
                    "HEIGHT"));
      font6.setCursor(subMenuRight+5,top);
      font6.print(sizeWidth);
      font6.print('\n');
      font6.print(sizeHeight);             
      highlight(subSelection,top,subMenuLeft,subMenuRight,true);
      readSelectButtons(subSelection,sizeMenuItems);
      if (subSelection == subSizeWidth) {
        return readNumericButtons(sizeWidth,minWidth,maxWidth);
      }
      else if (subSelection == subSizeHeight) {
        return readNumericButtons(sizeHeight,minHeight,maxHeight);
      }
      return false;
    case mainClear:  
      font6.setCursor(subMenuLeft+1,top+1);
      font6.print(F("THIS FRAME\n"
                    "ALL FRAMES"));
      highlight(subSelection,top,subMenuLeft,subMenuRight,true);
      readSelectButtons(subSelection,clearMenuItems);
      return arduboy.justPressed(B_BUTTON);
    case mainModify:  
      font6.setCursor(subMenuLeft+1,top+1);
      font6.print(F("MODE\n"  // flip, ins, del
                    "DIR\n"  // LEFT< DOWN, UP, RIGHT
                    "DO IT!"
                    ));
      font6.setCursor(subMenuRightShort+5,top+1);

      if (modifyTransform == modifyTransformFlip) {
        font6.print(F("FLIP\n"));
      }
      else if (modifyTransform == modifyTransformInsert) {
        font6.print(F("INSERT\n"));
      }
      else {
        font6.print(F("DELETE\n"));
      }

      if (modifyDirection == modifyDirectionUp) {
        font6.print(F("UP"));
      }
      else if (modifyDirection == modifyDirectionRight) {
        font6.print(F("RIGHT"));
      }
      else if (modifyDirection == modifyDirectionDown) {
        font6.print(F("DOWN"));
      }
      else {
        font6.print(F("LEFT"));        
      }
      
      highlight(subSelection,top,subMenuLeft,subMenuRightShort,true);
      readSelectButtons(subSelection,modifyMenuItems);

      if (subSelection == subModifyTransform) {
        readNumericButtons(modifyTransform,0,2);
      }
      else if (subSelection == subModifyDirection) {
        readNumericButtons(modifyDirection,0,3);
      }
      else {
        return arduboy.justPressed(B_BUTTON);
      }
      return false;
    case mainPreview:
      font6.setCursor(subMenuLeft+1,top+1);
      font6.print(F("BG\n"
                    "TILE\n"
                    "ANIM.\n"));
                    
      font6.setCursor(subMenuRightShort+2,top+1);

      if (previewBackground == previewBackgroundBlack) {
        font6.print(F("BLACK\n"));
      }
      else if (previewBackground == previewBackgroundWhite) {
        font6.print(F("WHITE\n"));
      }
      else {
        font6.print(F("CHECKER\n"));
      }

      if (previewTile == previewTileNo) {
        font6.print(F("NO\n"));
      }
      else {
        font6.print(F("YES\n"));
      }
      font6.print(previewAnimate);
                    
      highlight(subSelection,top,subMenuLeft,subMenuRightShort,true);
      readSelectButtons(subSelection,previewMenuItems);

      if (subSelection == subPreviewBackground) {
        readNumericButtons(previewBackground,previewBackgroundBlack,previewBackgroundChecker);
      }
      else if (subSelection == subPreviewTile) {
        readNumericButtons(previewTile,previewTileNo,previewTileYes);
      }
      else {
        readNumericButtons(previewAnimate,0,9);        
      }
      return false;
    case mainLoad:
      font6.setCursor(subMenuLeft+1,top+1);
      font6.print(F("WITH MASK\n"
                    "NO MASK\n"
                    "EXAMPLE"));
      highlight(subSelection,top,subMenuLeft,subMenuRight,true);
      readSelectButtons(subSelection,loadMenuItems);
      return arduboy.justPressed(B_BUTTON);
    case mainSave:
      font6.setCursor(subMenuLeft+1,top+1);
      font6.print(F("WITH MASK\n"
                    "NO MASK"));
      highlight(subSelection,top,subMenuLeft,subMenuRight,true);
      readSelectButtons(subSelection,saveMenuItems);
      return arduboy.justPressed(B_BUTTON);

    case mainInfo:
      font6.setCursor(subMenuLeft+1,1);      
      font6.print(F("ARDUBOY SPRITE\n"
                    "EDITOR (ASE)\n"
                    "VERSION 0.2\n\n"
                    "PAUL WASSON,\n"
                    "NOV 2019\n\n"
                    "GAME JAM 4\n"
                    "SUBMISSION"));
      return false;
  }
}


void Menu::highlight(uint8_t selection, uint8_t top, uint8_t left, uint8_t right, bool blink) {
#ifdef BLINKING_CURSOR
  if (blink && ((arduboy.frameCount % 32) < 16)) {
    return;
  }
#endif
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

uint16_t Menu::frameSize(bool withMask) {
  return (sizeHeight+7)/8 * sizeWidth * (withMask+1);
}

void Menu::newSize() {
  frameMax = min(bufferSize/frameSize(true),64);
  frameTotal = min(frameMax,8); 
  frameCurrent = 0;
}

uint8_t Menu::previewFrame() {
  if (previewAnimate == 0) {
    return frameCurrent;
  }
  else {
    return (frameCurrent + previewAnimateOffset) % frameTotal;
  }
}
