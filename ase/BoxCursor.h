#pragma once
#include <Arduboy2.h>

class BoxCursor {

   public:
      BoxCursor();
      void setOffset(const uint8_t x, const uint8_t y);
      void setPos(const uint8_t x, const uint8_t y);
      void setSize(const uint8_t x, const uint8_t y);
      void directionalButtons();
      void up();
      void down();
      void left();
      void right();
      void draw();

      uint8_t cursorX;
      uint8_t cursorY;
      uint8_t offsetX;
      uint8_t offsetY;      
      
   private:
      const uint8_t REPEAT_LIMIT = 30;
      uint8_t width;
      uint8_t height;
      uint8_t repeatCount;
};
