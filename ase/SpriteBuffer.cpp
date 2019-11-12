#include "SpriteBuffer.h"
#include "IncludeSprites.h"
#include "Menu.h"
#include "Canvas.h"

extern Menu menu;
extern Canvas canvas;

void SpriteBuffer::clearSprite() {
  for (int i=0; i<menu.bufferSize; i++) {
    sprite[i] = 0;
  }  
}

void SpriteBuffer::writeSprite(uint8_t frame, uint8_t x, uint8_t y, uint8_t color) {
  uint16_t i = spriteIndex(frame,x,y);
  sprite[i] &= ~(0x1 << (y%8));
  sprite[i] |= (color&1) << (y%8);
  sprite[i+1] &= ~(0x1 << (y%8));
  sprite[i+1] |= (1&(color>>1)) << (y%8);
}

uint16_t SpriteBuffer::spriteIndex(uint8_t frame, uint8_t x, uint8_t y) {
  return (2*(frame*menu.sizeWidth*((menu.sizeHeight+7)>>3)+x+(y/8)*menu.sizeWidth));
}

uint8_t SpriteBuffer::readSprite(uint8_t frame, uint8_t x, uint8_t y) {
  uint16_t i = spriteIndex(frame,x,y);
  return (1 & (sprite[i] >> (y % 8))) +
         ((1 & (sprite[i+1] >> (y % 8))) << 1);
}

void SpriteBuffer::clearFrame(uint8_t frame) {
  for (int x=0; x<menu.sizeWidth; x++) {
    for (int y=0; y<menu.sizeHeight; y++) {
      writeSprite(frame,x,y,0);
    }
  }
}

void SpriteBuffer::copyFrame(uint8_t fromFrame, uint8_t toFrame) {
  for (int x=0; x<menu.sizeWidth; x++) {
    for (int y=0; y<menu.sizeHeight; y++) {
      writeSprite(toFrame,x,y,readSprite(fromFrame,x,y));
    }
  }
}

void SpriteBuffer::swapFrame(uint8_t frameA, uint8_t frameB) {
  for (int x=0; x<menu.sizeWidth; x++) {
    for (int y=0; y<menu.sizeHeight; y++) {
      uint8_t temp = readSprite(frameA,x,y);
      writeSprite(frameA,x,y,readSprite(frameB,x,y));
      writeSprite(frameB,x,y,temp);
    }
  }
}

void SpriteBuffer::save(bool withMask) {
  uint16_t offset = 0;
  uint8_t increment = withMask ? 1 : 2;
  uint16_t loopSize = menu.frameSize(withMask);
  Serial.print(F("const unsigned char PROGMEM sprite"));
  if (withMask) {
    Serial.print(F("_and_mask"));  
  }
  Serial.print(F("[] = {\n"));
  Serial.print(F("// For load, cut and paste between START and END, and send over serial port\n"));
  if (withMask) {
    Serial.print(F("// then choose LOAD WITH MASK\n"));
  }
  else {
    Serial.print(F("// then choose LOAD NO MASK\n"));    
  }
  Serial.print(F("// START\n"));
  Serial.print(menu.sizeWidth);
  Serial.print(F(","));
  Serial.print(menu.sizeHeight);
  for (uint8_t frame=0; frame < menu.frameTotal; frame++) {
    Serial.println(F(","));
    Serial.print(sprite[offset]);
    offset += increment;
    for (uint16_t i=0 ; i < loopSize-1; i++) {
      Serial.print(F(","));
      Serial.print(sprite[offset]);
      offset += increment;
    }
  }
  Serial.println(F("\n// END\n};"));
}

/* Test:
const unsigned char PROGMEM sprite[] = {
// For load, cut and paste between START and END
// Choose LOAD WITH MASK
// START
16,16,
0,0,0,0,192,192,160,224,99,227,174,254,170,254,87,255,87,255,170,254,174,254,99,227,160,224,192,192,0,0,0,0,0,0,24,24,27,31,7,15,192,199,223,255,253,255,5,135,5,7,253,255,223,255,192,199,7,15,27,31,24,24,0,0,
0,0,0,0,192,192,160,224,99,227,174,254,170,254,87,255,87,255,170,254,174,254,99,227,160,224,192,192,0,0,0,0,0,0,24,24,27,31,7,15,192,199,223,255,253,255,5,135,5,7,253,255,223,255,192,199,7,15,27,31,24,24,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,8,8,24,24,28,28,60,60,62,62,126,126,127,127,255,255,255,255,127,127,126,126,62,62,60,60,28,28,24,24,8,8,
248,248,24,248,28,252,60,252,62,254,126,254,127,255,127,255,255,255,127,255,126,255,62,254,60,252,28,252,24,248,248,248,15,15,24,31,24,31,60,63,60,63,126,127,126,127,255,255,254,255,126,127,126,127,60,63,60,63,24,31,24,31,15,15
// END

 */

uint16_t SpriteBuffer::load(bool withMask) {
  
  menu.sizeWidth = Serial.parseInt();
  menu.sizeHeight = Serial.parseInt();
  menu.frameCurrent = 0;
  
  uint16_t offset = 0;
  while(Serial.available() && (offset < menu.bufferSize)) {
    sprite[offset++] = Serial.parseInt();
    if (!withMask) {
      sprite[offset] = sprite[offset-1];
      offset++;
    }
  }
  return(offset);
}

void SpriteBuffer::loadExample() {  
  menu.sizeWidth = (uint8_t)pgm_read_byte_near(exampleSprite+0);
  menu.sizeHeight = (uint8_t)pgm_read_byte_near(exampleSprite+1);
  menu.frameCurrent = 0;
  menu.newSize();
  menu.frameTotal = exampleSpriteFrames;
  
  for (uint16_t offset=0; offset < (2 + menu.frameSize(true) * menu.frameTotal); offset++) {
    sprite[offset] = (uint8_t)pgm_read_byte_near(exampleSprite+offset+2);
  }
}

void SpriteBuffer::flipFrame(uint8_t frame) {
  if ((menu.modifyDirection == menu.modifyDirectionRight) || (menu.modifyDirection == menu.modifyDirectionLeft)) {
    for (uint8_t y=0; y < menu.sizeHeight; y++) {
      for (uint8_t x=0; x < menu.sizeWidth/2; x++) {
        uint8_t otherX = menu.sizeWidth - x - 1;
        uint8_t temp = readSprite(menu.frameCurrent,x,y);
        writeSprite(frame,x,y,readSprite(frame,otherX,y));
        writeSprite(frame,otherX,y,temp);
      }
    }      
  }
  else {
    for (uint8_t x=0; x < menu.sizeWidth; x++) {
      for (uint8_t y=0; y < menu.sizeHeight/2; y++) {
        uint8_t otherY = menu.sizeHeight - y - 1;
        uint8_t temp = readSprite(menu.frameCurrent,x,y);
        writeSprite(frame,x,y,readSprite(frame,x,otherY));
        writeSprite(frame,x,otherY,temp);
      }
    }      
  }
}


void SpriteBuffer::deleteLine(uint8_t frame) {
  if (menu.modifyDirection == menu.modifyDirectionRight) {
    for (int8_t y=0; y < menu.sizeHeight; y++) {
      for (int8_t x=canvas.cursorX; x < menu.sizeWidth; x++) {
        uint8_t temp = x < (menu.sizeWidth-1) ? readSprite(menu.frameCurrent,x+1,y) : 0;
        writeSprite(frame,x,y,temp);
      }
    }      
  }
  else if (menu.modifyDirection == menu.modifyDirectionLeft) {
    for (int8_t y=0; y < menu.sizeHeight; y++) {
      for (int8_t x=canvas.cursorX; x >= 0; x--) {
        uint8_t temp = x > 0 ? readSprite(menu.frameCurrent,x-1,y) : 0;
        writeSprite(frame,x,y,temp);
      }
    }      
  }
  else if (menu.modifyDirection == menu.modifyDirectionDown) {
    for (int8_t x=0; x < menu.sizeWidth; x++) {
      for (int8_t y=canvas.cursorY; y < menu.sizeHeight; y++) {
        uint8_t temp = y < (menu.sizeHeight-1) ? readSprite(menu.frameCurrent,x,y+1) : 0;
        writeSprite(frame,x,y,temp);
      }
    }      
  }
  else if (menu.modifyDirection == menu.modifyDirectionUp) {
    for (int8_t x=0; x < menu.sizeWidth; x++) {
      for (int8_t y=canvas.cursorY; y >= 0; y--) {
        uint8_t temp = y > 0 ? readSprite(menu.frameCurrent,x,y-1) : 0;
        writeSprite(frame,x,y,temp);
      }
    }
  }
}

void SpriteBuffer::insertLine(uint8_t frame) {
  if (menu.modifyDirection == menu.modifyDirectionRight) {
    for (int8_t y=0; y < menu.sizeHeight; y++) {
      for (int8_t x=menu.sizeWidth-1; x > canvas.cursorX; x--) {
        uint8_t temp = readSprite(menu.frameCurrent,x-1,y);
        writeSprite(frame,x,y,temp);
      }
    }      
  }
  else if (menu.modifyDirection == menu.modifyDirectionLeft) {
    for (int8_t y=0; y < menu.sizeHeight; y++) {
      for (int8_t x=0; x < canvas.cursorX; x++) {
        uint8_t temp = readSprite(menu.frameCurrent,x+1,y);
        writeSprite(frame,x,y,temp);
      }
    }      
  }
  else if (menu.modifyDirection == menu.modifyDirectionDown) {
    for (int8_t x=0; x < menu.sizeWidth; x++) {
      for (int8_t y=menu.sizeHeight-1; y > canvas.cursorY; y--) {
        uint8_t temp = readSprite(menu.frameCurrent,x,y-1);
        writeSprite(frame,x,y,temp);
      }
    }      
  }
  else if (menu.modifyDirection == menu.modifyDirectionUp) {
    for (int8_t x=0; x < menu.sizeWidth; x++) {
      for (int8_t y=0; y < canvas.cursorY; y++) {
        uint8_t temp = readSprite(menu.frameCurrent,x,y+1);
        writeSprite(frame,x,y,temp);
      }
    }
  }
}
