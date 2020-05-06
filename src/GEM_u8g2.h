/*
  GEM (a.k.a. Good Enough Menu) - Arduino library for creation of graphic multi-level menu with
  editable menu items, such as variables (supports int, byte, boolean, char[17] data types) and
  option selects. User-defined callback function can be specified to invoke when menu item is saved.
  
  Supports buttons that can invoke user-defined actions and create action-specific
  context, which can have its own enter (setup) and exit callbacks as well as loop function.

  Requires U8g2 library by olikraus (https://github.com/olikraus/U8g2_Arduino).

  For documentation visit:
  https://github.com/Spirik/GEM

  Copyright (c) 2020 Alexander 'Spirik' Spiridonov

  This file is part of GEM library.

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 3 of the License, or (at your option) any later version.
  
  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
  Lesser General Public License for more details.
  
  You should have received a copy of the GNU Lesser General Public License
  along with this library.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef HEADER_GEM
#define HEADER_GEM

#include <U8g2lib.h>
#include "GEMPage.h"
#include "GEMSelect.h"

// Macro constants (aliases) for u8g2 font families used to draw menu
#define GEM_FONT_BIG        u8g2_font_6x12_tr
#define GEM_FONT_SMALL      u8g2_font_tom_thumb_4x6_tr
#define GEM_FONT_BIG_CYR    u8g2_font_6x12_t_cyrillic
#define GEM_FONT_SMALL_CYR  u8g2_font_4x6_t_cyrillic

// Macro constant (alias) for supported length of the string (character sequence) variable of type char[GEM_STR_LEN]
#define GEM_STR_LEN 17

// Macro constants (aliases) for menu pointer visual appearance
#define GEM_POINTER_DASH 0  // Current menu item is marked with pointer (filled square) to the left of its name
#define GEM_POINTER_ROW 1   // Current menu item is marked with filled background

// Macro constants (aliases) for supported types of associated with menu item variable
#define GEM_VAL_INTEGER 0  // Associated variable is of type int
#define GEM_VAL_BYTE 1     // Associated variable is of type byte
#define GEM_VAL_CHAR 2     // Associated variable is of type char[GEM_STR_LEN]
#define GEM_VAL_BOOLEAN 3  // Associated variable is of type boolean
#define GEM_VAL_SELECT 4   // Associated variable is either of type int, byte or char[] with option select used to pick a predefined value from the list
                           // (note that char[] array should be big enough to hold select option with the longest value)

// Macro constants (aliases) for the keys (buttons) used to navigate and interact with menu (mapped to corresponsding u8g2 constants)
#define GEM_KEY_NONE    0                         // No key presses are detected
#define GEM_KEY_UP      U8X8_MSG_GPIO_MENU_UP     // Up key is pressed (navigate up through the menu items list, select next value of the digit/char of editable variable, or previous option in select)
#define GEM_KEY_RIGHT   U8X8_MSG_GPIO_MENU_NEXT   // Right key is pressed (navigate through the link to another (child) menu page, select next digit/char of editable variable, execute code associated with button)
#define GEM_KEY_DOWN    U8X8_MSG_GPIO_MENU_DOWN   // Down key is pressed (navigate down through the menu items list, select previous value of the digit/char of editable variable, or next option in select)
#define GEM_KEY_LEFT    U8X8_MSG_GPIO_MENU_PREV   // Left key is pressed (navigate through the Back button to the previous menu page, select previous digit/char of editable variable)
#define GEM_KEY_CANCEL  U8X8_MSG_GPIO_MENU_HOME   // Cancel key is pressed (navigate to the previous (parent) menu page, exit edit mode without saving the variable, exit context loop if allowed within context's settings)
#define GEM_KEY_OK      U8X8_MSG_GPIO_MENU_SELECT // Ok/Apply key is pressed (toggle boolean menu item, enter edit mode of the associated non-boolean variable, exit edit mode with saving the variable, execute code associated with button)

// Declaration of Splash type
struct Splash {
  byte width;                         // Width of the splash lmage
  byte height;                        // Height of the splash image
  const uint8_t U8X8_PROGMEM *image;  // Pointer to XBM image to be shown as splash
};

// Declaration of FontSize type
struct FontSize {
  byte width;   // Width of the character
  byte height;  // Height of the character
};

// Declaration of FontFamilies type
struct FontFamilies {
  const uint8_t * big;    // Big font family (i.e., 5x8)
  const uint8_t * small;  // Small font family (i.e., 4x6)
};

// Declaration of AppContext type
struct AppContext {
  void (*loop)();   // Pointer to loop() function of current context (similar to regular loop() function: if context is defined, executed each regular loop() iteration),
                    // usually contains code of user-defined action that is run when menu Button is pressed
  void (*enter)();  // Pointer to enter() function of current context (similar to regular setup() function, called manually, generally once before context's loop() function, optional),
                    // usually contains some additional set up required by the user-defined action pointed to by context's loop()
  void (*exit)();   // Pointer to exit() function of current context (executed when user exits currently running context, optional),
                    // usually contains instructions to do some cleanup after context's loop() and to draw menu on screen again,
                    // if no user-defined function specified, default action will take place that consists of call to reInit(), drawMenu() and clearContext() methods
  boolean allowExit = true;  // Setting to false will require manually exit the context's loop() from within the loop itself (all necessary key detection should be done in context's loop() accordingly),
                             // otherwise exit is handled automatically by pressing GEM_KEY_CANCEL key (default is true)
};

// Forward declaration of necessary classes
class GEMItem;

// Declaration of GEM_u8g2 class
class GEM_u8g2 {
  public:
    /* 
      @param 'u8g2_' - reference to an object created with U8g2 library and used for communication with LCD
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
      default 86 (suitable for 128x64 screen with other variables at their default values)
    */
    GEM_u8g2(U8G2& u8g2_, byte menuPointerType_ = GEM_POINTER_ROW, byte menuItemsPerScreen_ = 5, byte menuItemHeight_ = 10, byte menuPageScreenTopOffset_ = 10, byte menuValuesLeftOffset_ = 86);

    /* INIT OPERATIONS */

    void setSplash(byte width, byte height, const unsigned char U8X8_PROGMEM *image); // Set custom XBM image displayed as the splash screen when GEM is being initialized. Should be called before GEM_u8g2::init().
    void hideVersion(boolean flag = true);               // Turn printing of the current GEM library version on splash screen off or back on. Should be called before GEM_u8g2::init().
    void enableCyrillic(boolean flag = true);            // Enable Cyrillic set of fonts. Generally should be called before GEM_u8g2::init(). To revert to non-Cyrillic fonts pass false: enableCyrillic(false).
    void init();                                         // Init the menu (set necessary settings, display GEM splash screen, etc.)
    void reInit();                                       // Reinitialize the menu (call U8g2::initDisplay() and then reapply GEM specific settings)
    void setMenuPageCurrent(GEMPage& menuPageCurrent);   // Set supplied menu page as current

    /* CONTEXT OPERATIONS */

    AppContext context;                                  // Currently set context
    void clearContext();                                 // Clear context

    /* DRAW OPERATIONS */

    void drawMenu();                                     // Draw menu on screen, with menu page set earlier in GEM_u8g2::setMenuPageCurrent()

    /* KEY DETECTION */

    boolean readyForKey();                               // Check that menu is waiting for the key press
    void registerKeyPress(byte keyCode);                 // Register the key press and trigger corresponding action
                                                         // Accepts GEM_KEY_NONE, GEM_KEY_UP, GEM_KEY_RIGHT, GEM_KEY_DOWN, GEM_KEY_LEFT, GEM_KEY_CANCEL, GEM_KEY_OK values
  private:
    U8G2& _u8g2;
    byte _menuPointerType;
    byte _menuItemsPerScreen;
    byte _menuItemHeight;
    byte _menuPageScreenTopOffset;
    byte _menuValuesLeftOffset;
    byte _menuItemFontSize;
    FontSize _menuItemFont[2] = {{6,8},{4,6}};
    FontFamilies _fontFamilies = {GEM_FONT_BIG, GEM_FONT_SMALL};
    bool _cyrillicEnabled = false;
    byte _menuItemInsetOffset;
    byte _menuItemTitleLength;
    byte _menuItemValueLength;
    Splash _splash;
    boolean _enableVersion = true;

    /* DRAW OPERATIONS */

    GEMPage* _menuPageCurrent;
    GEMItem* _menuItemCurrent;
    void layoutMenu();
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
    void initEditValueCursor();
    void nextEditValueCursorPosition();
    void prevEditValueCursorPosition();
    void drawEditValueCursor();
    void nextEditValueDigit();
    void prevEditValueDigit();
    void drawEditValueDigit(byte code);
    void nextEditValueSelect();
    void prevEditValueSelect();
    void saveEditValue();
    void cancelEditValue();
    void exitEditValue();
    char* trimString(char* str);

    /* KEY DETECTION */

    byte _currentKey;
    void dispatchKeyPress();
};

#endif
