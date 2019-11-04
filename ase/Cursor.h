#pragma once
#include <Arduboy2.h>

class Cursor {

   public:
      Cursor();
      void setSize(uint8_t width, uint8_t height);
      void setPos(uint8_t x, uint8_t y);
      void directionalButtons();
      void up()    {y -= height;}
      void down()  {y += height;}
      void left()  {x -= width;}
      void right() {x += width;}
      void draw();

      uint8_t x;
      uint8_t y;
      
   private:
      const uint8_t BLINK_RATE = 10;
      const uint8_t REPEAT_LIMIT = BLINK_RATE*3;

      uint8_t width;
      uint8_t height;
      uint8_t count; 
      uint8_t repeatCount;
};
