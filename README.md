# ASE - Arduboy Sprite Editor

## Overview
ASE lets you edit sprites for use in Arduboy applications.  

## Layout
- The screen is divided into 4 parts:

  - **Main Menu Pane**: on the left
  - **Draw / Sub-Menu Pane**: in the middle
  - **Preview Pane**: in the upper right
  - **Info Pane**: in the lower right

## Usage

- **Main Menu Mode**:
The application starts out in the *Main Menu Mode*, where the UP and DOWN buttons will move the cursor to choose a menu item and the B button will select then item. If the DRAW menu item is selected, then the application will be in *Draw Mode*, if another main menu item is selected then the application will be in *Sub Menu Mode*.
- **Draw Mode**:
In *Draw Mode*, the UP, DOWN, RIGHT, and LEFT button will move the cursor, the A button will return to *Main Menu Mode*, and the B button will alter the color of the pixel under the cursor.  The pixel can be in one of 3 colors: Transparent, White or Black.  Pressing the B button will cycle through the 3 colors.  If the B button is held down, the last color set will continue to set any other pixel the cursor is moved to.  This is an easy way to color a block of pixels the same. If a directional button is held down, after a short delay the cursor will repeatedly move.
- **Sub Menu Mode**: In *Sub Menu Mode*, the UP and DOWN buttons will move the cursor to choose an item. The A button will return to the *Main Menu Mode*. For items with a value, the LEFT and RIGHT button will change the value. Certain items will perform an action when the B button is pressed.
  - **Frame**: 
    - **Current**: lets you pick the current frame to edit.
    - **Copy To**: when activated by pressing the B button will copy the current frame to the *Copy To* frame.  Be careful as this will overwrite any previous drawing on the *Copy To* frame.
    - **Swap With**: when activated by pressing the B button will switch the current frame and the *Swap With* frame.
    - **Total**: sets the total number of frames up to a limit imposed by the buffer size.  Note that if the total frames is reduced, the upper frames still exist in the buffer and can be re-accessed by increasing the total.  The total can be set to tune the number of frames in an animation and to be saved.
  - **Modify**: the *Modify* sub menu lets you change the current frame of the sprite based on the cursor position and a set direction. The cursor psotion is set in the *Draw Mode*. The cursor position is shown in the *Preview Pane* as a horizontal line if the direction is up or down and as a vertical line if the direction is left or right.
    - **Mode**:
      - **Flip**: if the direction is up or down, will flip the frame vertically.  If the direction is right or left, will flip the frame horizontally.
      - **Insert**: Inserts a copy of the pixels on the cusor line show in the *Preview Pane* and moves the existing lines in the set direction.  This can be used to shift a sprite position if the cursor is outside of the image, or to grow a sprite in one dimension if the cursor is inside the image.
      - **Delete**: Deletes the pixels on the cursor line show in the *Preview Pane* and moves the existing lines in the set direction, filling in a line of transparent pixels on the outer edge.  This can be used to shift a sprite position or shrink a sprite in one dimension.
    - **Direction**: pick a direction for the modification: up, down, right, or left.
    - **Do It!**: This selection will perform the selected action when the B button is pressed.
  - **Preview**:
    - **BG**: Background setting.  Choose the background for the *Preview Pane* which is a good way to test the transparency of a sprite.  Choices are black, white or checker (alternating black/white pixels in a checkerboard pattern).
    - **Tile**: Chooses if the *Preview Pane* should just show one copy of the sprite or tile as many copies as will fit.  Can be used to make sure the edges of the sprite lines up when used in a tiling pattern.
    - **Anim.**: Set the sprite animation setting.
      - 0 = only show the current frame
      - 1 or more = increment through all the frames based on the *Total Frames* setting in the *Frame* sub-menu. A larger value will increase the animation speed.
  - **Size**: 
    - **Width**: Sets the width of the sprite in pixels.  Can range in value from 1 to 32.
    - **Height**: Sets the height of the sprite in pixels.  Can range in value from 1 to 32.  Values not divisible by 8 may not produce good results depending on the library being used to draw the sprite.
  - **Clear**:
    - **This Frame**: When activated by pressing the B button will set all the pixels in the current frame to transparent.
    - **All Frames**: When activated by pressing the B button will set all the pixels in all frames to transparent.  The number of frames is based on the *Total Frames* setting in the *Frame* sub-menu.
  - **Load**: Either loads an example or load a design over USB using a serial interface.  The parser for the serial interface is pretty dumb, so can only handle decimal integers separated by commas.  No other text should be included.
    - **With Mask**: when selected with the B button will read a design that specifies both the pixel on/off data and the transparency mask data.  Should be used when a design was saved with *With Mask*.
    - **No Mask**: when selected with the B button will read a design that specifies just pixel on/off data.  Should be used when a design was saved with *With Mask*. In this mode, all pixels will either be white or transparent.
    - **Example**: loads a built in example image that can be animated.
  - **Save**: Saves a design over USB using a serial interface.  The design can be used directly in a Arduboy game or can be loaded back into the editor.  It is suggested that you save often when designing a sprite so you have a backup copy in case of future mistakes.
    - **With Mask**: when selected with the B button will write a design that specifies both the pixel on/off data and the transparency mask data.
    - **No Mask**: when selected with the B button will write a design that specifies just pixel on/off data.
  - **Info**: Shows some information about the application.  The A button will return to the Main Menu.
 
