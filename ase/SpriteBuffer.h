#pragma once
#include <Arduboy2.h>

class SpriteBuffer {
  public:
    void clearSprite();
    void writeSprite(uint8_t frame, uint8_t x, uint8_t y, uint8_t color);
    uint16_t spriteIndex(uint8_t frame, uint8_t x, uint8_t y);
    uint8_t readSprite(uint8_t frame, uint8_t x, uint8_t y);
    void clearFrame(uint8_t frame);
    void copyFrame(uint8_t fromFrame, uint8_t toFrame);
    void swapFrame(uint8_t frameA, uint8_t frameB);
    void save(bool withMask);
    uint16_t load(bool withMask);
    void loadExample();
    void flipFrame(uint8_t frame);
    void deleteLine(uint8_t frame); 
    void insertLine(uint8_t frame);

    static const uint16_t bufferSize = 512;
    uint8_t sprite[bufferSize];
};

    
