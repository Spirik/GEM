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

#ifndef HEADER_GEM_ADAFRUIT_GFX
#define HEADER_GEM_ADAFRUIT_GFX

#include "config.h"

#ifdef GEM_ENABLE_ADAFRUIT_GFX_VERSION

#include <Adafruit_GFX.h>
#include "fonts/TomThumbMono.h"
#include "fonts/Fixed6x12.h"
#include "GEMAppearance.h"
#include "GEMContext.h"
#include "GEMPage.h"
#include "GEMSelect.h"
#ifdef GEM_SUPPORT_SPINNER
#include "GEMSpinner.h"
#endif
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
#define GEM_KEY_OK 6      // Ok/Apply key is pressed (toggle bool menu item, enter edit mode of the associated non-bool variable, exit edit mode with saving the variable, execute code associated with button)

// Declaration of Splash type
struct Splash {
  byte width;             // Width of the splash lmage
  byte height;            // Height of the splash image
  const uint8_t *image;   // Pointer to bitmap image to be shown as splash
};

// Declaration of FontSizeAGFX type
struct FontSizeAGFX {
  byte width;           // Width of the character
  byte height;          // Height of the character
  byte baselineOffset;  // Baseline position relative to the top edge of the character box
};

// Declaration of FontFamiliesAGFX type
struct FontFamiliesAGFX {
  const GFXfont *big;    // Big font family (i.e., 6x12)
  const GFXfont *small;  // Small font family (i.e., 4x6)
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
    /*
      @param 'agfx_' - reference to an object created with Adafruit GFX library and used for communication with display
      @param 'appearance_' - object of type GEMAppearance
    */
    GEM_adafruit_gfx(Adafruit_GFX& agfx_, GEMAppearance appearance_);

    /* APPEARANCE OPERATIONS */

    GEM_adafruit_gfx& setAppearance(GEMAppearance appearance);          // Set appearance of the menu (can be overridden in GEMPage on per page basis)
    GEMAppearance* getCurrentAppearance();                              // Get appearance (as a pointer to GEMAppearance) applied to current menu page (or general if menu page has none of its own)

    /* INIT OPERATIONS */

    GEM_adafruit_gfx& setSplash(byte width, byte height, const uint8_t *image); // Set custom bitmap image displayed as the splash screen when GEM is being initialized. Should be called before GEM_adafruit_gfx::init().
                                                                                // The following is the format of the bitmap as described in Adafruit GFX library documentation.
                                                                                // A contiguous block of bits, where each '1' bit sets the corresponding pixel to 'color,' while each '0' bit is skipped.
    GEM_adafruit_gfx& setSplashDelay(uint16_t value);                   // Set splash screen delay. Default value 1000ms, max value 65535ms. Setting to 0 will disable splash screen. Should be called before GEM_adafruit_gfx::init().
    GEM_adafruit_gfx& hideVersion(bool flag = true);                    // Turn printing of the current GEM library version on splash screen off or back on. Should be called before GEM_adafruit_gfx::init().
    GEM_adafruit_gfx& setTextSize(uint8_t size);                        // Set text 'magnification' size (as per Adafruit GFX docs); sprites will be scaled maximum up to two times regardless of the supplied value (default is 1)
    GEM_adafruit_gfx& setSpriteSize(uint8_t size);                      // Set sprite scaling factor if it should be different from the 'magnification' size above; sprites will be scaled maximum up to two times regardless of the supplied value (default is 1)
    GEM_adafruit_gfx& setFontBig(const GFXfont* font = GEM_FONT_BIG, uint8_t width = 6, uint8_t height = 8, uint8_t baselineOffset = 8);      // Set big font
    GEM_adafruit_gfx& setFontSmall(const GFXfont* font = GEM_FONT_SMALL, uint8_t width = 4, uint8_t height = 6, uint8_t baselineOffset = 6);  // Set small font
    GEM_adafruit_gfx& setForegroundColor(uint16_t color);               // Set foreground color of the menu (default is 0xFF)
    GEM_adafruit_gfx& setBackgroundColor(uint16_t color);               // Set background color of the menu (default is 0x00)
    GEM_adafruit_gfx& invertKeysDuringEdit(bool invert = true);         // Turn inverted order of characters during edit mode on or off
    GEM_VIRTUAL GEM_adafruit_gfx& init();                               // Init the menu (load necessary sprites into RAM of the SparkFun Graphic LCD Serial Backpack, display GEM splash screen, etc.)
    GEM_VIRTUAL GEM_adafruit_gfx& reInit();                             // Reinitialize the menu (apply GEM specific settings to AltSerialGraphicLCD library)
    GEM_adafruit_gfx& setMenuPageCurrent(GEMPage& menuPageCurrent);     // Set supplied menu page as current
    GEMPage* getCurrentMenuPage();                                      // Get pointer to current menu page

    /* CONTEXT OPERATIONS */

    GEMContext context;                                                 // Currently set context
    GEM_adafruit_gfx& clearContext();                                   // Clear context

    /* DRAW OPERATIONS */

    GEM_VIRTUAL GEM_adafruit_gfx& drawMenu();                           // Draw menu on screen, with menu page set earlier in GEM_adafruit_gfx::setMenuPageCurrent()
    GEM_adafruit_gfx& setDrawMenuCallback(void (*drawMenuCallback_)()); // Set callback that will be called at the end of GEM_adafruit_gfx::drawMenu()
    GEM_adafruit_gfx& removeDrawMenuCallback();                         // Remove callback that was called at the end of GEM_adafruit_gfx::drawMenu()

    /* VALUE EDIT */

    bool isEditMode();                                                  // Checks if menu is in edit mode

    /* KEY DETECTION */

    bool readyForKey();                                                 // Checks that menu is waiting for the key press
    GEM_adafruit_gfx& registerKeyPress(byte keyCode);                   // Register the key press and trigger corresponding action
                                                                        // Accepts GEM_KEY_NONE, GEM_KEY_UP, GEM_KEY_RIGHT, GEM_KEY_DOWN, GEM_KEY_LEFT, GEM_KEY_CANCEL, GEM_KEY_OK values
  protected:
    Adafruit_GFX& _agfx;
    GEMAppearance* _appearanceCurrent = nullptr;
    GEMAppearance _appearance;
    byte getMenuItemsPerScreen();
    byte getMenuItemFontSize();
    FontSizeAGFX _menuItemFont[2] = {{6,8,8},{4,6,6}};
    FontFamiliesAGFX _fontFamilies = {GEM_FONT_BIG, GEM_FONT_SMALL};
    byte _textSize = 1;
    byte _spriteSize = 1;
    bool _invertKeysDuringEdit = false;
    GEM_VIRTUAL byte getMenuItemTitleLength();
    GEM_VIRTUAL byte getMenuItemValueLength();
    Splash _splash;
    uint16_t _splashDelay = 1000;
    bool _enableVersion = true;
    uint16_t _menuForegroundColor = 0xFFFF;
    uint16_t _menuBackgroundColor = 0x0000;

    /* DRAW OPERATIONS */

    GEMPage* _menuPageCurrent = nullptr;
    void (*drawMenuCallback)() = nullptr;
    GEM_VIRTUAL void drawTitleBar();
    GEM_VIRTUAL void drawSprite(int16_t x, int16_t y, const Splash sprite[], uint16_t color);
    GEM_VIRTUAL void printMenuItemString(const char* str, byte num, byte startPos = 0);
    GEM_VIRTUAL void printMenuItemTitle(const char* str, int offset = 0);
    GEM_VIRTUAL void printMenuItemValue(const char* str, int offset = 0, byte startPos = 0);
    GEM_VIRTUAL void printMenuItemFull(const char* str, int offset = 0);
    GEM_VIRTUAL byte getMenuItemInsetOffset(bool forSprite = false);
    GEM_VIRTUAL byte getCurrentItemTopOffset(bool withInsetOffset = true, bool forSprite = false);
    GEM_VIRTUAL byte calculateSpriteOverlap(const Splash sprite[]);
    GEM_VIRTUAL void printMenuItem(GEMItem* menuItemTmp, byte yText, byte yDraw, uint16_t color);
    GEM_VIRTUAL void printMenuItems();
    GEM_VIRTUAL void drawMenuPointer(bool clear = false);
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
    GEM_VIRTUAL void drawEditValueCursor(bool clear = false);
    GEM_VIRTUAL void nextEditValueDigit();
    GEM_VIRTUAL void prevEditValueDigit();
    GEM_VIRTUAL void drawEditValueDigit(byte code, bool clear = false);
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
    GEM_VIRTUAL void exitEditValue(bool redrawMenu = true);
    char* trimString(char* str);

    /* KEY DETECTION */

    byte _currentKey;
    void dispatchKeyPress();
};

#endif

#endif
