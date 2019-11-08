#pragma once
#include <Arduboy2.h>
#include "VarFont6.h"

class Menu {
  public:
    
    bool mainMenu(bool hasFocus);
    bool subMenu();

    // Menu Selections
    uint8_t mainSelection = 0;
    uint8_t subSelection = 0;

    // Parameters
    uint8_t sizeWidth = 16;
    uint8_t sizeHeight = 16;
    uint8_t frameCurrent = 0;
    uint8_t frameTotal = 4;
    uint8_t frameCopyTo = 0;
    uint8_t frameSwapWith = 0;
    uint8_t modifyTransform = 0;
    uint8_t modifyDirection = 0;
    
    // Menu selections values
    static const uint8_t mainDraw = 0;  
    static const uint8_t mainFrame = 1;  
    static const uint8_t mainModify = 2;  
    static const uint8_t mainPreview = 3;  
    static const uint8_t mainSize = 4;  
    static const uint8_t mainClear = 5;  
    static const uint8_t mainLoad = 6;  
    static const uint8_t mainSave = 7;  
    static const uint8_t mainInfo = 8;

    static const uint8_t subFrameCurrent = 0;
    static const uint8_t subFrameCopyTo = 1;
    static const uint8_t subFrameSwapWith = 2;
    static const uint8_t subFrameTotal = 3;

    static const uint8_t subClearThis = 0;
    static const uint8_t subClearAll = 1;

    static const uint8_t subSizeWidth = 0;
    static const uint8_t subSizeHeight = 1;

    static const uint8_t subModifyTransform = 0;
    static const uint8_t subModifyDirection = 1;

    static const uint8_t modifyTransformFlip = 0;
    static const uint8_t modifyTransformInsert = 1;
    static const uint8_t modifyTransformDelete = 2;

    static const uint8_t modifyDirectionUp = 0;
    static const uint8_t modifyDirectionRight = 1;
    static const uint8_t modifyDirectionDown = 2;
    static const uint8_t modifyDirectionLeft = 3;

  private:
    // Constants
    static const uint8_t highlightYSpacing = 7; 
    static const uint8_t mainMenuLeft = 0; 
    static const uint8_t mainMenuRight = mainMenuLeft+30; 
    static const uint8_t subMenuLeft = mainMenuRight+2; 
    static const uint8_t subMenuRight = subMenuLeft+44;
    static const uint8_t subMenuRightShort = subMenuLeft+30;

    static const uint8_t mainMenuItems = 9;
    static const uint8_t frameMenuItems = 4; 
    static const uint8_t sizeMenuItems = 2; 
    static const uint8_t clearMenuItems = 2; 
    static const uint8_t modifyMenuItems = 3; 

    void highlight(uint8_t selection, uint8_t top, uint8_t left, uint8_t right, bool blink);
    bool readSelectButtons(uint8_t &selection, uint8_t selectionMax);
    bool readNumericButtons(uint8_t &value, uint8_t minValue, uint8_t maxValue);

};
