/*
  GEM (a.k.a. Good Enough Menu) - Arduino library for creation of graphic multi-level menu with
  editable menu items, such as variables (supports int, byte, float, double, bool, char[17] data types)
  and option selects. User-defined callback function can be specified to invoke when menu item is saved.
  
  Supports buttons that can invoke user-defined actions and create action-specific
  context, which can have its own enter (setup) and exit callbacks as well as loop function.

  Supports:
  - AltSerialGraphicLCD library by Jon Green (http://www.jasspa.com/serialGLCD.html);
  - U8g2 library by olikraus (https://github.com/olikraus/U8g2_Arduino);
  - Adafruit GFX library by Adafruit (https://github.com/adafruit/Adafruit-GFX-Library).

  For documentation visit:
  https://github.com/Spirik/GEM
  
  Copyright (c) 2018-2025 Alexander 'Spirik' Spiridonov

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

#include "config.h"

#ifdef GEM_ENABLE_GLCD_VERSION

#include <AltSerialGraphicLCD.h>
#include "GEMAppearance.h"
#include "GEMContext.h"
#include "GEMPage.h"
#include "GEMSelect.h"
#ifdef GEM_SUPPORT_SPINNER
#include "GEMSpinner.h"
#endif
#include "constants.h"

// Macro constants (aliases) for the keys (buttons) used to navigate and interact with menu
#define GEM_KEY_NONE 0    // No key presses are detected
#define GEM_KEY_UP 1      // Up key is pressed (navigate up through the menu items list, select next value of the digit/char of editable variable, or previous option in select)
#define GEM_KEY_RIGHT 2   // Right key is pressed (navigate through the link to another (child) menu page, select next digit/char of editable variable, execute code associated with button)
#define GEM_KEY_DOWN 3    // Down key is pressed (navigate down through the menu items list, select previous value of the digit/char of editable variable, or next option in select)
#define GEM_KEY_LEFT 4    // Left key is pressed (navigate through the Back button to the previous menu page, select previous digit/char of editable variable)
#define GEM_KEY_CANCEL 5  // Cancel key is pressed (navigate to the previous (parent) menu page, exit edit mode without saving the variable, exit context loop if allowed within context's settings)
#define GEM_KEY_OK 6      // Ok/Apply key is pressed (toggle bool menu item, enter edit mode of the associated non-bool variable, exit edit mode with saving the variable, execute code associated with button)

// Declaration of FontSize type
struct FontSize {
  byte width;   // Width of the character
  byte height;  // Height of the character
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
      values GEM_ITEMS_COUNT_AUTO, number
      default 5 (suitable for 128x64 screen with other variables at their default values); setting to GEM_ITEMS_COUNT_AUTO will enable auto count based on screen height
      @param 'menuItemHeight_' (optional) - height of the menu item
      default 10 (suitable for 128x64 screen with other variables at their default values)
      @param 'menuPageScreenTopOffset_' (optional) - offset from the top of the screen to accommodate title of the menu page
      default 10 (suitable for 128x64 screen with other variables at their default values)
      @param 'menuValuesLeftOffset_' (optional) - offset from the left of the screen to the value of the associated with menu item variable (effectively the space left for the title of the menu item to be printed on screen)
      default 86 (suitable for 128x64 screen with other variables at their default values)
    */
    GEM(GLCD& glcd_, byte menuPointerType_ = GEM_POINTER_ROW, byte menuItemsPerScreen_ = 5, byte menuItemHeight_ = 10, byte menuPageScreenTopOffset_ = 10, byte menuValuesLeftOffset_ = 86);
    /*
      @param 'glcd_' - reference to the instance of the GLCD class created with AltSerialGraphicLCD library
      @param 'appearance_' - object of type GEMAppearance
    */
    GEM(GLCD& glcd_, GEMAppearance appearance_);

    /* APPEARANCE OPERATIONS */

    GEM& setAppearance(GEMAppearance appearance);           // Set apperance of the menu (can be overridden in GEMPage on per page basis)
    GEMAppearance* getCurrentAppearance();                  // Get appearance (as a pointer to GEMAppearance) applied to current menu page (or general if menu page has none of its own)

    /* INIT OPERATIONS */

    GEM& setSplash(const uint8_t *sprite);                  // Set custom sprite displayed as the splash screen when GEM is being initialized. Should be called before GEM::init().
                                                            // The following is the format of the sprite as described in AltSerialGraphicLCD library documentation.
                                                            // The sprite commences with two bytes which are the width and height of the image in pixels.
                                                            // The pixel data is organised as rows of 8 vertical pixels per byte where the least significant bit (LSB)
                                                            // is the top-left pixel and the most significant bit (MSB) tends towards the bottom-left pixel.
                                                            // A complete row of 8 vertical pixels across the image width comprises the first row, this is then followed
                                                            // by the next row of 8 vertical pixels and so on.
                                                            // Where the image height is not an exact multiple of 8 bits then any unused bits are typically set to zero
                                                            // (although this does not matter).
    GEM& setSplashDelay(uint16_t value);                    // Set splash screen delay. Default value 1000ms, max value 65535ms. Setting to 0 will disable splash screen. Should be called before GEM::init().
    GEM& hideVersion(bool flag = true);                     // Turn printing of the current GEM library version on splash screen off or back on. Should be called before GEM::init().
    GEM& invertKeysDuringEdit(bool invert = true);          // Turn inverted order of characters during edit mode on or off
    GEM_VIRTUAL GEM& init();                                // Init the menu (load necessary sprites into RAM of the SparkFun Graphic LCD Serial Backpack, display GEM splash screen, etc.)
    GEM_VIRTUAL GEM& reInit();                              // Reinitialize the menu (apply GEM specific settings to AltSerialGraphicLCD library)
    GEM& setMenuPageCurrent(GEMPage& menuPageCurrent);      // Set supplied menu page as current
    GEMPage* getCurrentMenuPage();                          // Get pointer to current menu page

    /* CONTEXT OPERATIONS */

    GEMContext context;                                     // Currently set context
    GEM& clearContext();                                    // Clear context

    /* DRAW OPERATIONS */

    GEM_VIRTUAL GEM& drawMenu();                            // Draw menu on screen, with menu page set earlier in GEM::setMenuPageCurrent()
    GEM& setDrawMenuCallback(void (*drawMenuCallback_)());  // Set callback that will be called at the end of GEM::drawMenu()
    GEM& removeDrawMenuCallback();                          // Remove callback that was called at the end of GEM::drawMenu()

    /* VALUE EDIT */

    bool isEditMode();                                      // Checks if menu is in edit mode

    /* KEY DETECTION */

    bool readyForKey();                                     // Checks that menu is waiting for the key press
    GEM& registerKeyPress(byte keyCode);                    // Register the key press and trigger corresponding action
                                                            // Accepts GEM_KEY_NONE, GEM_KEY_UP, GEM_KEY_RIGHT, GEM_KEY_DOWN, GEM_KEY_LEFT, GEM_KEY_CANCEL, GEM_KEY_OK values
  protected:
    GLCD& _glcd;
    GEMAppearance* _appearanceCurrent = nullptr;
    GEMAppearance _appearance;
    byte getMenuItemsPerScreen();
    byte getMenuItemFontSize();
    FontSize _menuItemFont[2] = {{6,8},{4,6}};
    bool _invertKeysDuringEdit = false;
    GEM_VIRTUAL byte getMenuItemTitleLength();
    GEM_VIRTUAL byte getMenuItemValueLength();
    const uint8_t *_splash;
    uint16_t _splashDelay = 1000;
    bool _enableVersion = true;

    /* DRAW OPERATIONS */

    GEMPage* _menuPageCurrent = nullptr;
    void (*drawMenuCallback)() = nullptr;
    GEM_VIRTUAL void drawTitleBar();
    GEM_VIRTUAL void printMenuItemString(const char* str, byte num, byte startPos = 0);
    GEM_VIRTUAL void printMenuItemTitle(const char* str, int offset = 0);
    GEM_VIRTUAL void printMenuItemValue(const char* str, int offset = 0, byte startPos = 0);
    GEM_VIRTUAL void printMenuItemFull(const char* str, int offset = 0);
    GEM_VIRTUAL byte getMenuItemInsetOffset(bool forSprite = false);
    GEM_VIRTUAL byte getCurrentItemTopOffset(bool withInsetOffset = true, bool forSprite = false);
    GEM_VIRTUAL void printMenuItems();
    GEM_VIRTUAL void drawMenuPointer();
    GEM_VIRTUAL void drawScrollbar();

    /* MENU ITEMS NAVIGATION */

    GEM_VIRTUAL void nextMenuItem();
    GEM_VIRTUAL void prevMenuItem();
    GEM_VIRTUAL void menuItemSelect();

    /* VALUE EDIT */

    bool _editValueMode;
    byte _editValueType;
    byte _editValueLength;
    byte _editValueCursorPosition;
    byte _editValueVirtualCursorPosition;
    char _valueString[GEM_STR_LEN];
    int _valueSelectNum;
    GEM_VIRTUAL void enterEditValueMode();
    GEM_VIRTUAL void checkboxToggle();
    GEM_VIRTUAL void clearValueVisibleRange();
    GEM_VIRTUAL void initEditValueCursor();
    GEM_VIRTUAL void nextEditValueCursorPosition();
    GEM_VIRTUAL void prevEditValueCursorPosition();
    GEM_VIRTUAL void drawEditValueCursor();
    GEM_VIRTUAL void nextEditValueDigit();
    GEM_VIRTUAL void prevEditValueDigit();
    GEM_VIRTUAL void drawEditValueDigit(byte code);
    GEM_VIRTUAL void nextEditValueSelect();
    GEM_VIRTUAL void prevEditValueSelect();
    #ifdef GEM_SUPPORT_SPINNER
    GEM_VIRTUAL void nextEditValueSpinner();
    GEM_VIRTUAL void prevEditValueSpinner();
    #endif
    GEM_VIRTUAL void drawEditValueSelect();
    GEM_VIRTUAL void saveEditValue();
    GEM_VIRTUAL void cancelEditValue();
    GEM_VIRTUAL void resetEditValueState();
    GEM_VIRTUAL void exitEditValue();
    char* trimString(char* str);

    /* KEY DETECTION */

    byte _currentKey;
    void dispatchKeyPress();
};

#endif

#endif
