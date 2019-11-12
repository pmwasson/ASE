#pragma once
#include <Arduboy2.h>

class Canvas {
  public:
    uint8_t cursorX;
    uint8_t cursorY;

    void init();
    void draw(bool active);
    bool directionalButtons();
    
  private:

    // Constants
    static const uint8_t canvasWidth = 16; 
    static const uint8_t canvasHeight = 16;
    static const uint8_t screenX = 31; 
    static const uint8_t screenY = 0;
    static const uint8_t colorBlack = 2;
    static const uint8_t colorWhite = 3;
    static const uint8_t colorChecker = 4;
    static const uint8_t colorVerticalBar = 5;
    static const uint8_t colorHorizontalBar = 6;
    static const uint8_t repeatLimit = 30;

    // Members
    int8_t offsetX;
    int8_t offsetY;
    uint8_t repeatCount = 0;

    // Functions
    bool inSprite(uint8_t x, uint8_t y);
    void drawSquare(uint8_t color, uint8_t x, uint8_t y);
    void up();
    void down();
    void left();
    void right();
};
    
