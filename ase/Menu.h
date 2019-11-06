#pragma once
#include <Arduboy2.h>
#include "VarFont6.h"

class Menu {
  public:
    void draw();
    void readButtons();
    uint8_t selection = 0;

    // Menu results
    static const uint8_t selectDraw = 0;  
    static const uint8_t selectNew = 5;  
 
  private:
    // Constants
    static const uint8_t boxWidth = 30;
    static const uint8_t boxX = 0;
    static const uint8_t boxYSpacing = 7; 
    static const uint8_t selectionCount = 9;

    void invertBox();

};
