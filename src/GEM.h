/*
  GEM (a.k.a. Good Enough Menu) - Arduino library for creation of graphic multi-level menu with
  editable menu items, such as variables (supports int, byte, boolean, char[17] data types) and
  option selects. User-defined callback function can be specified to invoke when menu item is saved.
  
  Supports buttons that can invoke user-defined actions and create action-specific
  context, which can have its own enter (setup) and exit callbacks as well as loop function.

  Requires AltSerialGraphicLCD library by Jon Green (http://www.jasspa.com/serialGLCD.html).
  LCD screen must be equipped with SparkFun Graphic LCD Serial Backpack and properly set up
  to operate using firmware provided with aforementioned library.

  For documentation visit:
  https://github.com/Spirik/GEM

  Created by Alexander 'Spirik' Spiridonov, 16 Nov 2017
  
  This is free software. You can redistribute it and/or modify it under
  the terms of Creative Commons Attribution-ShareAlike 4.0 International License.
  To view a copy of this license, visit https://creativecommons.org/licenses/by-sa/4.0/
*/

#ifndef HEADER_GEM
#define HEADER_GEM

#include <AltSerialGraphicLCD.h>
#include "GEMPage.h"
#include "GEMSelect.h"

// Macro constant (alias) for supported length of the string (character sequence) variable of type char[GEM_STR_LEN]
#define GEM_STR_LEN 17

// Macro constants (aliases) for menu pointer visual appearance
#define GEM_POINTER_DASH 0  // Current menu item is marked with pointer (filled square) to the left of its name
#define GEM_POINTER_ROW 1   // Current menu item is marked with filled background

// Macro constants (aliases) for supported types of associated with menu item variable
#define GEM_VAL_INTEGER 0  // Associated variable is of type int
#define GEM_VAL_BYTE 1     // Associated variable is of type byte
#define GEM_VAL_CHAR 2     // Associated variable is of type char[17]
#define GEM_VAL_BOOLEAN 3  // Associated variable is of type boolean
#define GEM_VAL_SELECT 4   // Associated variable is either of type int, byte or char[] with option select used to pick a predefined value from the list
                           // (note that char[] array should be big enough to hold select option with the longest value)

// Macro constants (aliases) for the keys (buttons) used to navigate and interact with menu
#define GEM_KEY_NONE 0    // No key presses are detected
#define GEM_KEY_UP 1      // Up key is pressed (navigate up through the menu items list, select next value of the digit/char of editable variable, or previous option in select)
#define GEM_KEY_RIGHT 2   // Right key is pressed (navigate through the link to another (child) menu page, select next digit/char of editable variable, execute code associated with button)
#define GEM_KEY_DOWN 3    // Down key is pressed (navigate down through the menu items list, select previous value of the digit/char of editable variable, or next option in select)
#define GEM_KEY_LEFT 4    // Left key is pressed (navigate through the Back button to the previous menu page, select previous digit/char of editable variable)
#define GEM_KEY_CANCEL 5  // Cancel key is pressed (navigate to the previous (parent) menu page, exit edit mode without saving the variable, exit context loop if allowed within context's settings)
#define GEM_KEY_OK 6      // Ok/Apply key is pressed (toggle boolean menu item, enter edit mode of the associated non-boolean variable, exit edit mode with saving the variable, execute code associated with button)

// Declaration of FontSize type
struct FontSize {
  byte width;   // Width of the character
  byte height;  // Height of the character
};

// Declaration of AppContext type
struct AppContext {
  void (*loop)();   // Pointer to loop() function of current context (similar to regular loop() function, executed if context is defined each regular loop() iteration),
                    // usually contains code of user-defined action that is run when menu Button is pressed
  void (*enter)();  // Pointer to enter() function of current context (similar to regular setup() function, called manually, generally once before context's loop() function, optional),
                    // usually contains some additional set up required by the user-defined action pointed to by context's loop()
  void (*exit)();   // Pointer to exit() function of current context (executed when user exits currently running context, optional),
                    // usually contains instructions to do some cleanup after context's loop() and to draw menu on screen again,
                    // if no user-defined function specified, default action will take place that consists of call to drawMenu() and clearContext() methods
  boolean allowExit = true;  // Setting to false will require manually exit the context's loop() from within the loop itself (all necessary key detection should be done in context's loop() accordingly),
                             // otherwise exit is handled automatically by pressing GEM_KEY_CANCEL key (default is true)
};

// Forward declaration of necessary classes
class GEMItem;

// Declaration of GEM class
class GEM {
  public:
    /* 
      @param 'glcd_' - reference to the instance of the GLCD class created with AltSerialGraphicLCD library
      @param 'menuPointerType_' (optional) - type of menu pointer visual appearance
      values GEM_POINTER_ROW, GEM_POINTER_DASH
      default GEM_POINTER_ROW
      @param 'menuItemsPerScreen_' (optional) - count of the menu items per screen
      default 5 (suitable for 128x64 screen with other variables at their default values)
      @param 'menuItemHeight_' (optional) - height of the menu item
      default 10 (suitable for 128x64 screen with other variables at their default values)
      @param 'menuPageScreenTopOffset_' (optional) - offset from the top of the screen to accommodate title of the menu page
      default 10 (suitable for 128x64 screen with other variables at their default values)
      @param 'menuValuesLeftOffset_' (optional) - offset from the left of the screen to the value of the associated with menu item variable (effectively the space left for the title of the menu item to be printed on screen)
      default 86 (suitable for 128x64 screen with other variables at their default values; 86 - maximum value for 128x64 screen)
    */
    GEM(GLCD& glcd_, byte menuPointerType_ = GEM_POINTER_ROW, byte menuItemsPerScreen_ = 5, byte menuItemHeight_ = 10, byte menuPageScreenTopOffset_ = 10, byte menuValuesLeftOffset_ = 86);
    void setSplash(uint8_t PROGMEM *sprite);             // Set custom sprite displayed as the splash screen when GEM is being initialized. Should be called before GEM::init().
                                                         // The following is the format of the sprite as described in AltSerialGraphicLCD library documentation.
                                                         // The sprite commences with two bytes which are the width and height of the image in pixels.
                                                         // The pixel data is organised as rows of 8 vertical pixels per byte where the least significant bit (LSB)
                                                         // is the top-left pixel and the most significant bit (MSB) tends towards the bottom-left pixel.
                                                         // A complete row of 8 vertical pixels across the image width comprises the first row, this is then followed
                                                         // by the next row of 8 vertical pixels and so on.
                                                         // Where the image height is not an exact multiple of 8 bits then any unused bits are typically set to zero
                                                         // (although this does not matter).
    void hideVersion(boolean flag = true);               // Turn printing of the current GEM library version on splash screen off or back on. Should be called before GEM::init().
    void init();                                         // Init the menu (load necessary sprites into RAM of the SparkFun Graphic LCD Serial Backpack, display GEM splash screen, etc.)
    void setMenuPageCurrent(GEMPage& menuPageCurrent);   // Set supplied menu page as current
    void drawMenu();                                     // Draw menu on screen, with menu page set earlier in GEM::setMenuPageCurrent()
    boolean readyForKey();                               // Check that menu is waiting for the key press
    void registerKeyPress(byte keyCode);                 // Register the key press and trigger corresponding action
                                                         // Accepts GEM_KEY_NONE, GEM_KEY_UP, GEM_KEY_RIGHT, GEM_KEY_DOWN, GEM_KEY_LEFT, GEM_KEY_CANCEL, GEM_KEY_OK values
    AppContext context;                                  // Currently set context
    void clearContext();                                 // Clear context
  private:
    GLCD& _glcd;
    byte _menuPointerType;
    byte _menuItemsPerScreen;
    byte _menuItemHeight;
    byte _menuPageScreenTopOffset;
    byte _menuValuesLeftOffset;
    byte _menuItemFontSize;
    FontSize _menuItemFont[2] = {{6,8},{4,6}};
    byte _menuItemInsetOffset;
    byte _menuItemTitleLength;
    byte _menuItemValueLength;
    uint8_t PROGMEM *_splash;
    boolean _enableVersion = true;

    /* DRAW OPERATIONS */

    GEMPage* _menuPageCurrent;
    GEMItem* _menuItemCurrent;
    void drawTitleBar();
    void printMenuItemString(char* str, byte num, byte startPos = 0);
    void printMenuItemTitle(char* str, int offset = 0);
    void printMenuItemValue(char* str, int offset = 0, byte startPos = 0);
    void printMenuItemFull(char* str, int offset = 0);
    byte getMenuItemInsetOffset(boolean forSprite = false);
    byte getCurrentItemTopOffset(boolean withInsetOffset = true, boolean forSprite = false);
    void printMenuItems();
    void drawMenuPointer();
    void drawScrollbar();

    /* MENU ITEMS NAVIGATION */

    void nextMenuItem();
    void prevMenuItem();
    void menuItemSelect();

    /* VALUE EDIT */

    boolean _editValueMode;
    byte _editValueType;
    byte _editValueLength;
    byte _editValueCursorPosition;
    byte _editValueVirtualCursorPosition;
    char _valueString[GEM_STR_LEN];
    int _valueSelectNum;
    void enterEditValueMode();
    void checkboxToggle();
    void clearValueVisibleRange();
    void initEditValueCursor();
    void nextEditValueCursorPosition();
    void prevEditValueCursorPosition();
    void drawEditValueCursor();
    void nextEditValueDigit();
    void prevEditValueDigit();
    void drawEditValueDigit(byte code);
    void nextEditValueSelect();
    void prevEditValueSelect();
    void drawEditValueSelect();
    void saveEditValue();
    void cancelEditValue();
    void exitEditValue();
    void editValueLoop();
    char* trimString(char* str);

    /* KEY DETECTION */

    byte _currentKey;
    void dispatchKeyPress();
};

#endif
