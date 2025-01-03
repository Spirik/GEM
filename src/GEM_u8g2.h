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

#ifndef HEADER_GEM_U8G2
#define HEADER_GEM_U8G2

#include "config.h"

#ifdef GEM_ENABLE_U8G2_VERSION

#include <U8g2lib.h>
#include "GEMAppearance.h"
#include "GEMContext.h"
#include "GEMPage.h"
#include "GEMSelect.h"
#ifdef GEM_SUPPORT_SPINNER
#include "GEMSpinner.h"
#endif
#include "constants.h"

// Macro constants (aliases) for u8g2 font families used to draw menu
#define GEM_FONT_BIG        u8g2_font_6x12_tr
#define GEM_FONT_SMALL      u8g2_font_tom_thumb_4x6_tr
#define GEM_FONT_BIG_CYR    u8g2_font_6x12_t_cyrillic
#define GEM_FONT_SMALL_CYR  u8g2_font_4x6_t_cyrillic

// Macro constants (aliases) for the keys (buttons) used to navigate and interact with menu (mapped to corresponsding u8g2 constants)
#define GEM_KEY_NONE    0                         // No key presses are detected
#define GEM_KEY_UP      U8X8_MSG_GPIO_MENU_UP     // Up key is pressed (navigate up through the menu items list, select next value of the digit/char of editable variable, or previous option in select)
#define GEM_KEY_RIGHT   U8X8_MSG_GPIO_MENU_NEXT   // Right key is pressed (navigate through the link to another (child) menu page, select next digit/char of editable variable, execute code associated with button)
#define GEM_KEY_DOWN    U8X8_MSG_GPIO_MENU_DOWN   // Down key is pressed (navigate down through the menu items list, select previous value of the digit/char of editable variable, or next option in select)
#define GEM_KEY_LEFT    U8X8_MSG_GPIO_MENU_PREV   // Left key is pressed (navigate through the Back button to the previous menu page, select previous digit/char of editable variable)
#define GEM_KEY_CANCEL  U8X8_MSG_GPIO_MENU_HOME   // Cancel key is pressed (navigate to the previous (parent) menu page, exit edit mode without saving the variable, exit context loop if allowed within context's settings)
#define GEM_KEY_OK      U8X8_MSG_GPIO_MENU_SELECT // Ok/Apply key is pressed (toggle bool menu item, enter edit mode of the associated non-bool variable, exit edit mode with saving the variable, execute code associated with button)

// Declaration of Splash type
struct Splash {
  byte width;                         // Width of the splash lmage
  byte height;                        // Height of the splash image
  const uint8_t *image;  // Pointer to XBM image to be shown as splash
};

// Declaration of FontSize type
struct FontSize {
  byte width;   // Width of the character
  byte height;  // Height of the character
};

// Declaration of FontFamiliesU8g2 type
struct FontFamiliesU8g2 {
  const uint8_t *big;    // Big font family (i.e., 6x12)
  const uint8_t *small;  // Small font family (i.e., 4x6)
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
      values GEM_ITEMS_COUNT_AUTO, number
      default 5 (suitable for 128x64 screen with other variables at their default values); setting to GEM_ITEMS_COUNT_AUTO will enable auto count based on screen height
      @param 'menuItemHeight_' (optional) - height of the menu item
      default 10 (suitable for 128x64 screen with other variables at their default values)
      @param 'menuPageScreenTopOffset_' (optional) - offset from the top of the screen to accommodate title of the menu page
      default 10 (suitable for 128x64 screen with other variables at their default values)
      @param 'menuValuesLeftOffset_' (optional) - offset from the left of the screen to the value of the associated with menu item variable (effectively the space left for the title of the menu item to be printed on screen)
      default 86 (suitable for 128x64 screen with other variables at their default values)
    */
    GEM_u8g2(U8G2& u8g2_, byte menuPointerType_ = GEM_POINTER_ROW, byte menuItemsPerScreen_ = 5, byte menuItemHeight_ = 10, byte menuPageScreenTopOffset_ = 10, byte menuValuesLeftOffset_ = 86);
    /*
      @param 'u8g2_' - reference to an object created with U8g2 library and used for communication with LCD
      @param 'appearance_' - object of type GEMAppearance
    */
    GEM_u8g2(U8G2& u8g2_, GEMAppearance appearance_);

    /* APPEARANCE OPERATIONS */

    GEM_u8g2& setAppearance(GEMAppearance appearance);          // Set appearance of the menu (can be overridden in GEMPage on per page basis)
    GEMAppearance* getCurrentAppearance();                      // Get appearance (as a pointer to GEMAppearance) applied to current menu page (or general if menu page has none of its own)
    
    /* INIT OPERATIONS */

    GEM_u8g2& setSplash(byte width, byte height, const unsigned char *image); // Set custom XBM image displayed as the splash screen when GEM is being initialized. Should be called before GEM_u8g2::init().
    GEM_u8g2& setSplashDelay(uint16_t value);                   // Set splash screen delay. Default value 1000ms, max value 65535ms. Setting to 0 will disable splash screen. Should be called before GEM_u8g2::init().
    GEM_u8g2& hideVersion(bool flag = true);                    // Turn printing of the current GEM library version on splash screen off or back on. Should be called before GEM_u8g2::init().
    GEM_u8g2& enableUTF8(bool flag = true);                     // Enable UTF8 fonts support. Generally should be called before GEM_u8g2::init(). To disable UTF8 fonts support pass false: enableUTF8(false).
    GEM_u8g2& enableCyrillic(bool flag = true);                 // Enable default Cyrillic set of fonts with GEM. Generally should be called before GEM_u8g2::init(). To revert to non-Cyrillic fonts pass false: enableCyrillic(false).
    GEM_u8g2& setFontBig(const uint8_t* font, uint8_t width = 6, uint8_t height = 8);   // Set big font
    GEM_u8g2& setFontBig();                                     // Revert big font to default value (with respect to _UTF8Enabled flag)
    GEM_u8g2& setFontSmall(const uint8_t* font, uint8_t width = 4, uint8_t height = 6); // Set small font
    GEM_u8g2& setFontSmall();                                   // Revert small font to default value (with respect to _UTF8Enabled flag)
    GEM_u8g2& invertKeysDuringEdit(bool invert = true);         // Turn inverted order of characters during edit mode on or off
    GEM_VIRTUAL GEM_u8g2& init();                               // Init the menu (set necessary settings, display GEM splash screen, etc.)
    GEM_VIRTUAL GEM_u8g2& reInit();                             // Reinitialize the menu (call U8g2::initDisplay() and then reapply GEM specific settings)
    GEM_u8g2& setMenuPageCurrent(GEMPage& menuPageCurrent);     // Set supplied menu page as current
    GEMPage* getCurrentMenuPage();                              // Get pointer to current menu page

    /* CONTEXT OPERATIONS */

    GEMContext context;                                         // Currently set context
    GEM_u8g2& clearContext();                                   // Clear context

    /* DRAW OPERATIONS */

    GEM_VIRTUAL GEM_u8g2& drawMenu();                           // Draw menu on screen, with menu page set earlier in GEM_u8g2::setMenuPageCurrent()
    GEM_u8g2& setDrawMenuCallback(void (*drawMenuCallback_)()); // Set callback that will be called at the end of GEM_u8g2::drawMenu()
    GEM_u8g2& removeDrawMenuCallback();                         // Remove callback that was called at the end of GEM_u8g2::drawMenu()

    /* VALUE EDIT */

    bool isEditMode();                                          // Checks if menu is in edit mode

    /* KEY DETECTION */

    bool readyForKey();                                         // Checks that menu is waiting for the key press
    GEM_u8g2& registerKeyPress(byte keyCode);                   // Register the key press and trigger corresponding action
                                                                // Accepts GEM_KEY_NONE, GEM_KEY_UP, GEM_KEY_RIGHT, GEM_KEY_DOWN, GEM_KEY_LEFT, GEM_KEY_CANCEL, GEM_KEY_OK values
  protected:
    U8G2& _u8g2;
    GEMAppearance* _appearanceCurrent = nullptr;
    GEMAppearance _appearance;
    byte getMenuItemsPerScreen();
    byte getMenuItemFontSize();
    FontSize _menuItemFont[2] = {{6,8},{4,6}};
    FontFamiliesU8g2 _fontFamilies = {GEM_FONT_BIG, GEM_FONT_SMALL};
    bool _UTF8Enabled = false;
    bool _invertKeysDuringEdit = false;
    GEM_VIRTUAL byte getMenuItemTitleLength();
    GEM_VIRTUAL byte getMenuItemValueLength();
    Splash _splash;
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
