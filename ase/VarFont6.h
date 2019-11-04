#pragma once
#include <Arduboy2.h>

class VarFont6 : public Print {
  public:
    VarFont6();   

    virtual size_t write(uint8_t); // used by the Arduino Print class
    size_t printChar(const char c, const int8_t x, int8_t y);
    void setCursor(const int8_t x, const int8_t y, const bool inv=false);
    void setInverse(const bool inv);

  private:
    const uint8_t LINE_HEIGHT = 7;
    const uint8_t LETTER_SPACING = 1;
    int8_t cursorX;
    int8_t baseX;
    int8_t cursorY;
    bool inverse;
};
