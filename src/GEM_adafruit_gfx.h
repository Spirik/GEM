/*
  GEM (a.k.a. Good Enough Menu) - Arduino library for creation of graphic multi-level menu with
  editable menu items, such as variables (supports int, byte, float, double, boolean, char[17] data types)
  and option selects. User-defined callback function can be specified to invoke when menu item is saved.
  
  Supports buttons that can invoke user-defined actions and create action-specific
  context, which can have its own enter (setup) and exit callbacks as well as loop function.

  Supports:
  - AltSerialGraphicLCD library by Jon Green (http://www.jasspa.com/serialGLCD.html);
  - U8g2 library by olikraus (https://github.com/olikraus/U8g2_Arduino);
  - Adafruit GFX library by Adafruit (https://github.com/adafruit/Adafruit-GFX-Library).

  For documentation visit:
  https://github.com/Spirik/GEM

  Copyright (c) 2018-2022 Alexander 'Spirik' Spiridonov

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

#ifndef HEADER_GEM_ADAFRUIT_GFX
#define HEADER_GEM_ADAFRUIT_GFX

#include "config.h"

#ifdef GEM_ENABLE_ADAFRUIT_GFX_VERSION

#include <Adafruit_GFX.h>
#include "fonts/TomThumbMono.h"
#include "fonts/Fixed6x12.h"
#include "GEMPage.h"
#include "GEMSelect.h"
#include "constants.h"

// Macro constants (aliases) for Adafruit GFX font families used to draw menu
#define GEM_FONT_BIG       &Fixed6x12
#define GEM_FONT_SMALL     &TomThumbMono

// Macro constants (aliases) for the keys (buttons) used to navigate and interact with menu
#define GEM_KEY_NONE 0    // No key presses are detected
#define GEM_KEY_UP 1      // Up key is pressed (navigate up through the menu items list, select next value of the digit/char of editable variable, or previous option in select)
#define GEM_KEY_RIGHT 2   // Right key is pressed (navigate through the link to another (child) menu page, select next digit/char of editable variable, execute code associated with button)
#define GEM_KEY_DOWN 3    // Down key is pressed (navigate down through the menu items list, select previous value of the digit/char of editable variable, or next option in select)
#define GEM_KEY_LEFT 4    // Left key is pressed (navigate through the Back button to the previous menu page, select previous digit/char of editable variable)
#define GEM_KEY_CANCEL 5  // Cancel key is pressed (navigate to the previous (parent) menu page, exit edit mode without saving the variable, exit context loop if allowed within context's settings)
#define GEM_KEY_OK 6      // Ok/Apply key is pressed (toggle boolean menu item, enter edit mode of the associated non-boolean variable, exit edit mode with saving the variable, execute code associated with button)

// Declaration of Splash type
struct Splash {
  byte width;             // Width of the splash lmage
  byte height;            // Height of the splash image
  const uint8_t *image;   // Pointer to bitmap image to be shown as splash
};

// Declaration of FontSizeAgfx type
struct FontSizeAgfx {
  byte width;           // Width of the character
  byte height;          // Height of the character
  byte baselineOffset;  // Baseline position relative to the top edge of the character box
};

// Declaration of FontFamiliesAGFX type
struct FontFamiliesAGFX {
  const GFXfont *big;    // Big font family (i.e., 6x12)
  const GFXfont *small;  // Small font family (i.e., 4x6)
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

// Declaration of GEM_adafruit_gfx class
class GEM_adafruit_gfx {
  public:
    /* 
      @param 'agfx_' - reference to an object created with Adafruit GFX library and used for communication with display
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
    GEM_adafruit_gfx(Adafruit_GFX& agfx_, byte menuPointerType_ = GEM_POINTER_ROW, byte menuItemsPerScreen_ = 5, byte menuItemHeight_ = 10, byte menuPageScreenTopOffset_ = 10, byte menuValuesLeftOffset_ = 86);

    /* INIT OPERATIONS */

    void setSplash(byte width, byte height, const uint8_t *image);  // Set custom bitmap image displayed as the splash screen when GEM is being initialized. Should be called before GEM_adafruit_gfx::init().
                                                                    // The following is the format of the bitmap as described in Adafruit GFX library documentation.
                                                                    // A contiguous block of bits, where each '1' bit sets the corresponding pixel to 'color,' while each '0' bit is skipped.
    void setSplashDelay(uint16_t value);                 // Set splash screen delay. Default value 1000ms, max value 65535ms. Setting to 0 will disable splash screen. Should be called before GEM::init().
    void hideVersion(boolean flag = true);               // Turn printing of the current GEM library version on splash screen off or back on. Should be called before GEM::init().
    void setForegroundColor(uint16_t color);             // Set foreground color of the menu (default is 0xFF)
    void setBackgroundColor(uint16_t color);             // Set background color of the menu (default is 0x00)
    void init();                                         // Init the menu (load necessary sprites into RAM of the SparkFun Graphic LCD Serial Backpack, display GEM splash screen, etc.)
    void reInit();                                       // Reinitialize the menu (apply GEM specific settings to AltSerialGraphicLCD library)
    void setMenuPageCurrent(GEMPage& menuPageCurrent);   // Set supplied menu page as current

    /* CONTEXT OPERATIONS */

    AppContext context;                                  // Currently set context
    void clearContext();                                 // Clear context

    /* DRAW OPERATIONS */

    void drawMenu();                                     // Draw menu on screen, with menu page set earlier in GEM::setMenuPageCurrent()

    /* KEY DETECTION */

    boolean readyForKey();                               // Check that menu is waiting for the key press
    void registerKeyPress(byte keyCode);                 // Register the key press and trigger corresponding action
                                                         // Accepts GEM_KEY_NONE, GEM_KEY_UP, GEM_KEY_RIGHT, GEM_KEY_DOWN, GEM_KEY_LEFT, GEM_KEY_CANCEL, GEM_KEY_OK values
  private:
    Adafruit_GFX& _agfx;
    byte _menuPointerType;
    byte _menuItemsPerScreen;
    byte _menuItemHeight;
    byte _menuPageScreenTopOffset;
    byte _menuValuesLeftOffset;
    byte _menuItemFontSize;
    FontSizeAgfx _menuItemFont[2] = {{6,12,10},{4,6,6}};
    FontFamiliesAGFX _fontFamilies = {GEM_FONT_BIG, GEM_FONT_SMALL};
    byte _menuItemInsetOffset;
    byte _menuItemTitleLength;
    byte _menuItemValueLength;
    Splash _splash;
    uint16_t _splashDelay = 1000;
    boolean _enableVersion = true;
    uint16_t _menuForegroundColor = 0xFFFF;
    uint16_t _menuBackgroundColor = 0x0000;

    /* DRAW OPERATIONS */

    GEMPage* _menuPageCurrent = nullptr;
    void drawTitleBar();
    void printMenuItemString(const char* str, byte num, byte startPos = 0);
    void printMenuItemTitle(const char* str, int offset = 0);
    void printMenuItemValue(const char* str, int offset = 0, byte startPos = 0);
    void printMenuItemFull(const char* str, int offset = 0);
    byte getMenuItemInsetOffset(boolean forSprite = false);
    byte getCurrentItemTopOffset(boolean withInsetOffset = true, boolean forSprite = false);
    void printMenuItem(GEMItem* menuItemTmp, byte yText, byte yDraw, uint16_t color);
    void printMenuItems();
    void drawMenuPointer(boolean clear = false);
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
    void drawEditValueCursor(boolean clear = false);
    void nextEditValueDigit();
    void prevEditValueDigit();
    void drawEditValueDigit(byte code, boolean clear = false);
    void nextEditValueSelect();
    void prevEditValueSelect();
    void drawEditValueSelect();
    void saveEditValue();
    void cancelEditValue();
    void exitEditValue(boolean redrawMenu = true);
    char* trimString(char* str);

    /* KEY DETECTION */

    byte _currentKey;
    void dispatchKeyPress();
};

#endif

#endif
