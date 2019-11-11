# ASE - Arduboy Sprite Editor

## Overview
ASE lets you edit sprites for use in Arduboy applications.  

## Layout
- The screen is divided into 4 parts:

  - **Main Menu Pane**: on the left.
  - **Draw / Sub-Menu Pane**: in the middle
  - **Preview Pane**: in the upper right
  - **Info Pane**: in the lower right

## Usage

- **Main Menu Mode**:
The application starts out in the *Main Menu Mode*, where the UP and DOWN buttons will move the cursor to choose a menu item and the B button will select then item. If the DRAW menu item is selected, then the application will be in *Draw Mode*, if a different main menu item is selected then the application will be in *Sub Menu Mode*.
- **Draw Mode**:
In *Draw Mode*, the UP, DOWN, RIGHT, and LEFT button will move the cursor, the A button will return to *Main Menu Mode*, and the B button will alter the color of the pixel under the cursor.  The pixel can be in 3 colors: Transparent, White or Black.  Pressing the B button will cycle through the 3 colors.  If the B button is held down, the last color set will continue to set any other pixel the cursor is moved to.  This is an easy way to color a block of pixels the same. If a directional button is held down, after a short delay the cursor will repeatedly move.
- **Sub Menu Mode**: In *Sub Menu Mode*, the UP and DOWN buttons will move the cursor to choose an item. The A button will return to the *Main Menu Mode*. For items with a value, the LEFT and RIGHT button will change the value. Certain items will perform an action when the B button is pressed.
  - **Frame**: 
    - **Current**: lets you pick the current frame to edit.
    - **Copy To**: when activated will copy the current frame to the *Copy To* frame.  Be careful as this will overwrite any previous drawing on the *Copy To* frame.
    - **Swap With**: when activated will switch the current frame and the *Swap With* frame.
    - **Total**: sets the total number of frames up to a limit imposed by the buffer size.  Note that if the total frames is reduced, the upper frames still exist in the buffer and can be re-accessed by increasing the total.  The total can be set to tune the number of frames in an animation and to be saved.
  - **Modify**:
  - **Preview**:
  - **Size**: 
  - **Clear**:
  - **Load**:
  - **Save**:
  - **Info**: Shows some information about the application.  The A button will return to the Main Menu.
 
