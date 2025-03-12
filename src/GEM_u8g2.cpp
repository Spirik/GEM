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

#include <Arduino.h>
#include "GEM_u8g2.h"

#ifdef GEM_ENABLE_U8G2_VERSION

// AVR-based Arduinos have suppoort for dtostrf, some others may require manual inclusion (e.g. SAMD),
// see https://github.com/plotly/arduino-api/issues/38#issuecomment-108987647
#if defined(GEM_SUPPORT_FLOAT_EDIT) && (defined(ARDUINO_ARCH_SAMD) || defined(ARDUINO_ARCH_SAM) || defined(ARDUINO_ARCH_RP2040) || defined(ARDUINO_ARCH_NRF52840))
#include <avr/dtostrf.h>
#endif

// Macro constants (aliases) for some of the ASCII character codes
#define GEM_CHAR_CODE_9 57
#define GEM_CHAR_CODE_0 48
#define GEM_CHAR_CODE_MINUS 45
#define GEM_CHAR_CODE_DOT 46
#define GEM_CHAR_CODE_SPACE 32
#define GEM_CHAR_CODE_UNDERSCORE 95
#define GEM_CHAR_CODE_TILDA 126
#define GEM_CHAR_CODE_BANG 33
#define GEM_CHAR_CODE_a 97
#define GEM_CHAR_CODE_ACCENT 96
/*
// WIP for Cyrillic values support
#define GEM_CHAR_CODE_CYR_YO 1025
#define GEM_CHAR_CODE_CYR_A 1040
#define GEM_CHAR_CODE_CYR_E 1045
#define GEM_CHAR_CODE_CYR_E_SM 1077
#define GEM_CHAR_CODE_CYR_YA_SM 1103
#define GEM_CHAR_CODE_CYR_YO_SM 1105
*/

// XBM image of the default GEM _splash screen (GEM logo v1)
/*
#define logo_width  12
#define logo_height 12
static const unsigned char logo_bits [] U8X8_PROGMEM = {
  0xc0,0xf7,0x60,0xfe,0x30,0xfe,0x18,0xff,0x8c,0xff,0xc6,0xff,
  0xe3,0xff,0xf1,0xff,0x19,0xff,0x7f,0xfc,0xff,0xfd,0xfe,0xf7
};
*/

// XBM image of the default GEM _splash screen (GEM logo v2)
#define logo_width  20
#define logo_height 8
static const unsigned char logo_bits [] U8X8_PROGMEM = {
  0x8f,0x4f,0xf4,0x00,0x40,0xf4,0x00,0x40,0xf5,0x98,0x47,0xf5,
  0x00,0x40,0xf4,0x00,0x40,0xf4,0x9f,0x4f,0xf4,0x00,0x00,0xf0
};

// Sprites of the UI elements used to draw menu

#define sprite_height  8

#define arrowRight_width  6
#define arrowRight_height 8
static const unsigned char arrowRight_bits [] U8X8_PROGMEM = {
   0xc0,0xc4,0xcc,0xdc,0xcc,0xc4,0xc0,0xc0
};

#define arrowLeft_width  6
#define arrowLeft_height 8
static const unsigned char arrowLeft_bits [] U8X8_PROGMEM = {
   0xc0,0xc4,0xc6,0xc7,0xc6,0xc4,0xc0,0xc0
};

#define arrowBtn_width  6
#define arrowBtn_height 8
static const unsigned char arrowBtn_bits [] U8X8_PROGMEM = {
  0xc0,0xc3,0xc5,0xc9,0xc5,0xc3,0xc0,0xc0
};

#define checkboxUnchecked_width  7
#define checkboxUnchecked_height 8
static const unsigned char checkboxUnchecked_bits [] U8X8_PROGMEM = {
   0x80,0xbf,0xa1,0xa1,0xa1,0xa1,0xbf,0x80
};

#define checkboxChecked_width  7
#define checkboxChecked_height 8
static const unsigned char checkboxChecked_bits [] U8X8_PROGMEM = {
   0xc0,0xaf,0xb1,0xab,0xa5,0xa1,0xbf,0x80
};

#define selectArrows_width  6
#define selectArrows_height 8
static const unsigned char selectArrows_bits [] U8X8_PROGMEM = {
   0xc0,0xc4,0xce,0xc0,0xce,0xc4,0xc0,0xc0
};

GEM_u8g2::GEM_u8g2(U8G2& u8g2_, byte menuPointerType_, byte menuItemsPerScreen_, byte menuItemHeight_, byte menuPageScreenTopOffset_, byte menuValuesLeftOffset_)
  : _u8g2(u8g2_)
{
  _appearance.menuPointerType = menuPointerType_;
  _appearance.menuItemsPerScreen = menuItemsPerScreen_;
  _appearance.menuItemHeight = menuItemHeight_;
  _appearance.menuPageScreenTopOffset = menuPageScreenTopOffset_;
  _appearance.menuValuesLeftOffset = menuValuesLeftOffset_;
  _appearanceCurrent = &_appearance;
  _splash = {logo_width, logo_height, logo_bits};
  clearContext();
  _editValueMode = false;
  _editValueCursorPosition = 0;
  memset(_valueString, '\0', GEM_STR_LEN - 1);
  _valueSelectNum = -1;
}

GEM_u8g2::GEM_u8g2(U8G2& u8g2_, GEMAppearance appearance_)
  : _u8g2(u8g2_)
  , _appearance(appearance_)
{
  _appearanceCurrent = &_appearance;
  _splash = {logo_width, logo_height, logo_bits};
  clearContext();
  _editValueMode = false;
  _editValueCursorPosition = 0;
  memset(_valueString, '\0', GEM_STR_LEN - 1);
  _valueSelectNum = -1;
}

//====================== APPEARANCE OPERATIONS

GEM_u8g2& GEM_u8g2::setAppearance(GEMAppearance appearance) {
  _appearance = appearance;
  return *this;
}

GEMAppearance* GEM_u8g2::getCurrentAppearance() {
  return (_menuPageCurrent != nullptr && _menuPageCurrent->_appearance != nullptr) ? _menuPageCurrent->_appearance : &_appearance;
}

byte GEM_u8g2::getMenuItemsPerScreen() {
  return getCurrentAppearance()->menuItemsPerScreen == GEM_ITEMS_COUNT_AUTO ? (_u8g2.getDisplayHeight() - getCurrentAppearance()->menuPageScreenTopOffset) / getCurrentAppearance()->menuItemHeight : getCurrentAppearance()->menuItemsPerScreen;
}

byte GEM_u8g2::getMenuItemFontSize() {
  return getCurrentAppearance()->menuItemHeight >= _menuItemFont[0].height ? 0 : 1;
}

byte GEM_u8g2::getMenuItemTitleLength() {
  return (getCurrentAppearance()->menuValuesLeftOffset - 5) / _menuItemFont[getMenuItemFontSize()].width;
}

byte GEM_u8g2::getMenuItemValueLength() {
  return (_u8g2.getDisplayWidth() - getCurrentAppearance()->menuValuesLeftOffset - 6) / _menuItemFont[getMenuItemFontSize()].width;
}

//====================== INIT OPERATIONS

GEM_u8g2& GEM_u8g2::setSplash(byte width, byte height, const unsigned char *image) {
  _splash = {width, height, image};
  return *this;
}

GEM_u8g2&  GEM_u8g2::setSplashDelay(uint16_t value) {
  _splashDelay = value;
  return *this;
}

GEM_u8g2& GEM_u8g2::hideVersion(bool flag) {
  _enableVersion = !flag;
  return *this;
}

GEM_u8g2& GEM_u8g2::enableUTF8(bool flag) {
  _UTF8Enabled = flag;
  if (_UTF8Enabled) {
    _u8g2.enableUTF8Print();
  } else {
    _u8g2.disableUTF8Print();
  }
  return *this;
}

GEM_u8g2& GEM_u8g2::enableCyrillic(bool flag) {
  enableUTF8(flag);
  if (_UTF8Enabled) {
    _fontFamilies = {(uint8_t *)GEM_FONT_BIG_CYR, (uint8_t *)GEM_FONT_SMALL_CYR};
  } else {
    _fontFamilies = {(uint8_t *)GEM_FONT_BIG, (uint8_t *)GEM_FONT_SMALL};
  }
  _menuItemFont[0] = {6, 8};
  _menuItemFont[1] = {4, 6};
  return *this;
}

GEM_u8g2& GEM_u8g2::setFontBig(const uint8_t* font, uint8_t width, uint8_t height) {
  _fontFamilies.big = font;
  _menuItemFont[0] = {width, height};
  return *this;
}

GEM_u8g2& GEM_u8g2::setFontBig() {
  _fontFamilies.big = _UTF8Enabled ? GEM_FONT_BIG_CYR : GEM_FONT_BIG;
  _menuItemFont[0] = {6, 8};
  return *this;
}

GEM_u8g2& GEM_u8g2::setFontSmall(const uint8_t* font, uint8_t width, uint8_t height) {
  _fontFamilies.small = font;
  _menuItemFont[1] = {width, height};
  return *this;
}

GEM_u8g2& GEM_u8g2::setFontSmall() {
  _fontFamilies.small = _UTF8Enabled ? GEM_FONT_SMALL_CYR : GEM_FONT_SMALL;
  _menuItemFont[1] = {4, 6};
  return *this;
}

GEM_u8g2& GEM_u8g2::invertKeysDuringEdit(bool invert) {
  _invertKeysDuringEdit = invert;
  return *this;
}

GEM_u8g2& GEM_u8g2::init() {
  _u8g2.clear();
  _u8g2.setDrawColor(1);
  _u8g2.setFontPosTop();

  if (_splashDelay > 0) {

    _u8g2.firstPage();
    do {
      _u8g2.drawXBMP((_u8g2.getDisplayWidth() - _splash.width) / 2, (_u8g2.getDisplayHeight() - _splash.height) / 2, _splash.width, _splash.height, _splash.image);
    } while (_u8g2.nextPage());

    if (_enableVersion) {
      delay(_splashDelay / 2);
      _u8g2.firstPage();
      do {
        _u8g2.drawXBMP((_u8g2.getDisplayWidth() - _splash.width) / 2, (_u8g2.getDisplayHeight() - _splash.height) / 2, _splash.width, _splash.height, _splash.image);
        _u8g2.setFont(_fontFamilies.small);
        byte x = _u8g2.getDisplayWidth() - strlen(GEM_VER)*4;
        byte y = _u8g2.getDisplayHeight() - 7;
        if (_splash.image != logo_bits) {
          _u8g2.setCursor(x - 12, y);
          _u8g2.print("GEM");
        } else {
          _u8g2.setCursor(x, y);
        }
        _u8g2.print(GEM_VER);
      } while (_u8g2.nextPage());
      delay(_splashDelay / 2);
    } else {
      delay(_splashDelay);
    }

    _u8g2.clear();

  }

  return *this;
}

GEM_u8g2& GEM_u8g2::reInit() {
  _u8g2.initDisplay();
  _u8g2.setPowerSave(0);
  _u8g2.clear();
  _u8g2.setDrawColor(1);
  _u8g2.setFontPosTop();
  if (_UTF8Enabled) {
    _u8g2.enableUTF8Print();
  } else {
    _u8g2.disableUTF8Print();
  }
  return *this;
}

GEM_u8g2& GEM_u8g2::setMenuPageCurrent(GEMPage& menuPageCurrent) {
  _menuPageCurrent = &menuPageCurrent;
  return *this;
}

GEMPage* GEM_u8g2::getCurrentMenuPage() {
  return _menuPageCurrent;
}

//====================== CONTEXT OPERATIONS

GEM_u8g2& GEM_u8g2::clearContext() {
  context.loop = nullptr;
  context.enter = nullptr;
  context.exit = nullptr;
  context.allowExit = true;
  return *this;
}

//====================== DRAW OPERATIONS

GEM_u8g2& GEM_u8g2::drawMenu() {
  // _u8g2.clear(); // Not clearing for better performance
  _u8g2.firstPage();
  do {
    drawTitleBar();
    printMenuItems();
    drawMenuPointer();
    drawScrollbar();
    if (drawMenuCallback != nullptr) {
      drawMenuCallback();
    }
  } while (_u8g2.nextPage());
  return *this;
}

GEM_u8g2& GEM_u8g2::setDrawMenuCallback(void (*drawMenuCallback_)()) {
  drawMenuCallback = drawMenuCallback_;
  return *this;
}

GEM_u8g2& GEM_u8g2::removeDrawMenuCallback() {
  drawMenuCallback = nullptr;
  return *this;
}

void GEM_u8g2::drawTitleBar() {
 _u8g2.setFont(_fontFamilies.small);
 _u8g2.setCursor(5, 0);
 _u8g2.print(_menuPageCurrent->title);
 _u8g2.setFont(getMenuItemFontSize() ? _fontFamilies.small : _fontFamilies.big);
}

void GEM_u8g2::printMenuItemString(const char* str, byte num, byte startPos) {
  if (_UTF8Enabled) {

    byte j = 0;
    byte p = 0;
    while ((j < startPos || ((byte)str[p] >= 128 && (byte)str[p] <= 191)) && str[p] != '\0') {
      if ((byte)str[p] <= 127 || (byte)str[p] >= 194) {
        j++;
      }
      p++;
    }
    byte startPosReal = p;

    byte i = j;
    byte k = startPosReal;
    while ((i < num + j || ((byte)str[k] >= 128 && (byte)str[k] <= 191)) && str[k] != '\0') {
      _u8g2.print(str[k]);
      if ((byte)str[k] <= 127 || (byte)str[k] >= 194) {
        i++;
      }
      k++;
    }

  } else {

    byte i = startPos;
    while (i < num + startPos && str[i] != '\0') {
      _u8g2.print(str[i]);
      i++;
    }

  }
}

void GEM_u8g2::printMenuItemTitle(const char* str, int offset) {
  printMenuItemString(str, getMenuItemTitleLength() + offset);
}

void GEM_u8g2::printMenuItemValue(const char* str, int offset, byte startPos) {
  printMenuItemString(str, getMenuItemValueLength() + offset, startPos);
}

void GEM_u8g2::printMenuItemFull(const char* str, int offset) {
  printMenuItemString(str, getMenuItemTitleLength() + getMenuItemValueLength() + offset);
}

byte GEM_u8g2::getMenuItemInsetOffset(bool forSprite) {
  byte menuItemFontSize = getMenuItemFontSize();
  byte menuItemInsetOffset = (getCurrentAppearance()->menuItemHeight - _menuItemFont[menuItemFontSize].height) / 2;
  return menuItemInsetOffset + (forSprite ? (_menuItemFont[menuItemFontSize].height - sprite_height) / 2 : -1); // With additional offset for sprites and text for better visual alignment
}

byte GEM_u8g2::getCurrentItemTopOffset(bool withInsetOffset, bool forSprite) {
  return (_menuPageCurrent->currentItemNum % getMenuItemsPerScreen()) * getCurrentAppearance()->menuItemHeight + getCurrentAppearance()->menuPageScreenTopOffset + (withInsetOffset ? getMenuItemInsetOffset(forSprite) : 0);
}

void GEM_u8g2::printMenuItems() {
  byte menuItemsPerScreen = getMenuItemsPerScreen();
  byte currentPageScreenNum = _menuPageCurrent->currentItemNum / menuItemsPerScreen;
  GEMItem* menuItemTmp = _menuPageCurrent->getMenuItem(currentPageScreenNum * menuItemsPerScreen);
  byte y = getCurrentAppearance()->menuPageScreenTopOffset;
  byte i = 0;
  char valueStringTmp[GEM_STR_LEN];
  while (menuItemTmp != nullptr && i < menuItemsPerScreen) {
    byte yText = y + getMenuItemInsetOffset();
    byte yDraw = y + getMenuItemInsetOffset(true);
    switch (menuItemTmp->type) {
      case GEM_ITEM_VAL:
        {
          _u8g2.setCursor(5, yText);
          if (menuItemTmp->readonly) {
            printMenuItemTitle(menuItemTmp->title, -1);
            _u8g2.print("^");
          } else {
            printMenuItemTitle(menuItemTmp->title);
          }

          byte menuValuesLeftOffset = getCurrentAppearance()->menuValuesLeftOffset;
          _u8g2.setCursor(menuValuesLeftOffset, yText);
          switch (menuItemTmp->linkedType) {
            case GEM_VAL_INTEGER:
              if (_editValueMode && menuItemTmp == _menuPageCurrent->getCurrentMenuItem()) {
                printMenuItemValue(_valueString, 0, _editValueVirtualCursorPosition - _editValueCursorPosition);
                drawEditValueCursor();
              } else {
                itoa(*(int*)menuItemTmp->linkedVariable, valueStringTmp, 10);
                printMenuItemValue(valueStringTmp);
              }
              break;
            case GEM_VAL_BYTE:
              if (_editValueMode && menuItemTmp == _menuPageCurrent->getCurrentMenuItem()) {
                printMenuItemValue(_valueString, 0, _editValueVirtualCursorPosition - _editValueCursorPosition);
                drawEditValueCursor();
              } else {
                itoa(*(byte*)menuItemTmp->linkedVariable, valueStringTmp, 10);
                printMenuItemValue(valueStringTmp);
              }
              break;
            case GEM_VAL_CHAR:
              if (_editValueMode && menuItemTmp == _menuPageCurrent->getCurrentMenuItem()) {
                printMenuItemValue(_valueString, 0, _editValueVirtualCursorPosition - _editValueCursorPosition);
                drawEditValueCursor();
              } else {
                printMenuItemValue((char*)menuItemTmp->linkedVariable);
              }
              break;
            case GEM_VAL_BOOL:
              if (*(bool*)menuItemTmp->linkedVariable) {
                _u8g2.drawXBMP(menuValuesLeftOffset, yDraw, checkboxChecked_width, checkboxChecked_height, checkboxChecked_bits);
              } else {
                _u8g2.drawXBMP(menuValuesLeftOffset, yDraw, checkboxUnchecked_width, checkboxUnchecked_height, checkboxUnchecked_bits);
              }
              break;
            case GEM_VAL_SELECT:
              {
                GEMSelect* select = menuItemTmp->select;
                if (_editValueMode && menuItemTmp == _menuPageCurrent->getCurrentMenuItem()) {
                  printMenuItemValue(select->getOptionNameByIndex(_valueSelectNum));
                  _u8g2.drawXBMP(_u8g2.getDisplayWidth() - 7, yDraw, selectArrows_width, selectArrows_height, selectArrows_bits);
                  drawEditValueCursor();
                } else {
                  printMenuItemValue(select->getSelectedOptionName(menuItemTmp->linkedVariable));
                  _u8g2.drawXBMP(_u8g2.getDisplayWidth() - 7, yDraw, selectArrows_width, selectArrows_height, selectArrows_bits);
                }
              }
              break;
            #ifdef GEM_SUPPORT_SPINNER
            case GEM_VAL_SPINNER:
              {
                GEMSpinner* spinner = menuItemTmp->spinner;
                if (_editValueMode && menuItemTmp == _menuPageCurrent->getCurrentMenuItem()) {
                  GEMSpinnerValue valueTmp = spinner->getOptionNameByIndex(menuItemTmp->linkedVariable, _valueSelectNum);
                  switch (spinner->getType()) {
                    case GEM_VAL_BYTE:
                      itoa(valueTmp.valByte, valueStringTmp, 10);
                      break;
                    case GEM_VAL_INTEGER:
                      itoa(valueTmp.valInt, valueStringTmp, 10);
                      break;
                    #ifdef GEM_SUPPORT_FLOAT_EDIT
                    case GEM_VAL_FLOAT:
                      dtostrf(valueTmp.valFloat, menuItemTmp->precision + 1, menuItemTmp->precision, valueStringTmp);
                      break;
                    case GEM_VAL_DOUBLE:
                      dtostrf(valueTmp.valDouble, menuItemTmp->precision + 1, menuItemTmp->precision, valueStringTmp);
                      break;
                    #endif
                  }
                  printMenuItemValue(valueStringTmp);
                  _u8g2.drawXBMP(_u8g2.getDisplayWidth() - 7, yDraw, selectArrows_width, selectArrows_height, selectArrows_bits);
                  drawEditValueCursor();
                } else {
                  switch (spinner->getType()) {
                    case GEM_VAL_BYTE:
                      itoa(*(byte*)menuItemTmp->linkedVariable, valueStringTmp, 10);
                      break;
                    case GEM_VAL_INTEGER:
                      itoa(*(int*)menuItemTmp->linkedVariable, valueStringTmp, 10);
                      break;
                    #ifdef GEM_SUPPORT_FLOAT_EDIT
                    case GEM_VAL_FLOAT:
                      dtostrf(*(float*)menuItemTmp->linkedVariable, menuItemTmp->precision + 1, menuItemTmp->precision, valueStringTmp);
                      break;
                    case GEM_VAL_DOUBLE:
                      dtostrf(*(double*)menuItemTmp->linkedVariable, menuItemTmp->precision + 1, menuItemTmp->precision, valueStringTmp);
                      break;
                    #endif
                  }
                  printMenuItemValue(valueStringTmp);
                  _u8g2.drawXBMP(_u8g2.getDisplayWidth() - 7, yDraw, selectArrows_width, selectArrows_height, selectArrows_bits);
                }
              }
              break;
            #endif
            #ifdef GEM_SUPPORT_FLOAT_EDIT
            case GEM_VAL_FLOAT:
              if (_editValueMode && menuItemTmp == _menuPageCurrent->getCurrentMenuItem()) {
                printMenuItemValue(_valueString, 0, _editValueVirtualCursorPosition - _editValueCursorPosition);
                drawEditValueCursor();
              } else {
                // sprintf(valueStringTmp,"%.6f", *(float*)menuItemTmp->linkedVariable); // May work for non-AVR boards
                dtostrf(*(float*)menuItemTmp->linkedVariable, menuItemTmp->precision + 1, menuItemTmp->precision, valueStringTmp);
                printMenuItemValue(valueStringTmp);
              }
              break;
            case GEM_VAL_DOUBLE:
              if (_editValueMode && menuItemTmp == _menuPageCurrent->getCurrentMenuItem()) {
                printMenuItemValue(_valueString, 0, _editValueVirtualCursorPosition - _editValueCursorPosition);
                drawEditValueCursor();
              } else {
                // sprintf(valueStringTmp,"%.6f", *(double*)menuItemTmp->linkedVariable); // May work for non-AVR boards
                dtostrf(*(double*)menuItemTmp->linkedVariable, menuItemTmp->precision + 1, menuItemTmp->precision, valueStringTmp);
                printMenuItemValue(valueStringTmp);
              }
              break;
            #endif
          }
          break;
        }
      case GEM_ITEM_LINK:
        _u8g2.setCursor(5, yText);
        if (menuItemTmp->readonly) {
          printMenuItemFull(menuItemTmp->title, -1);
          _u8g2.print("^");
        } else {
          printMenuItemFull(menuItemTmp->title);
        }
        _u8g2.drawXBMP(_u8g2.getDisplayWidth() - 8, yDraw, arrowRight_width, arrowRight_height, arrowRight_bits);
        break;
      case GEM_ITEM_BACK:
        _u8g2.drawXBMP(5, yDraw, arrowLeft_width, arrowLeft_height, arrowLeft_bits);
        break;
      case GEM_ITEM_BUTTON:
        _u8g2.setCursor(11, yText);
        if (menuItemTmp->readonly) {
          printMenuItemFull(menuItemTmp->title, -1);
          _u8g2.print("^");
        } else {
          printMenuItemFull(menuItemTmp->title);
        }
        _u8g2.drawXBMP(5, yDraw, arrowBtn_width, arrowBtn_height, arrowBtn_bits);
        break;
      case GEM_ITEM_LABEL:
        _u8g2.setCursor(5, yText);
        printMenuItemFull(menuItemTmp->title);
        break;
    }
    menuItemTmp = menuItemTmp->getMenuItemNext();
    y += getCurrentAppearance()->menuItemHeight;
    i++;
  }
  memset(valueStringTmp, '\0', GEM_STR_LEN - 1);
}

void GEM_u8g2::drawMenuPointer() {
  if (_menuPageCurrent->itemsCount > 0) {
    GEMItem* menuItemTmp = _menuPageCurrent->getCurrentMenuItem();
    int pointerPosition = getCurrentItemTopOffset(false);
    byte menuItemHeight = getCurrentAppearance()->menuItemHeight;
    if (getCurrentAppearance()->menuPointerType == GEM_POINTER_DASH) {
      if (menuItemTmp->readonly || menuItemTmp->type == GEM_ITEM_LABEL) {
        for (byte i = 0; i < (menuItemHeight - 1) / 2; i++) {
          _u8g2.drawPixel(0, pointerPosition + i * 2);
          _u8g2.drawPixel(1, pointerPosition + i * 2 + 1);
        }
      } else {
        _u8g2.drawBox(0, pointerPosition, 2, menuItemHeight - 1);
      }
    } else if (!_editValueMode) {
      _u8g2.setDrawColor(2);
      _u8g2.drawBox(0, pointerPosition - 1, _u8g2.getDisplayWidth() - 2, menuItemHeight + 1);
      _u8g2.setDrawColor(1);
      if (menuItemTmp->readonly || menuItemTmp->type == GEM_ITEM_LABEL) {
        _u8g2.setDrawColor(0);
        for (byte i = 0; i < (menuItemHeight + 2) / 2; i++) {
          _u8g2.drawPixel(0, pointerPosition + i * 2);
          _u8g2.drawPixel(1, pointerPosition + i * 2 - 1);
        }
        _u8g2.setDrawColor(1);
      }
    }
  }
}

void GEM_u8g2::drawScrollbar() {
  byte menuItemsPerScreen = getMenuItemsPerScreen();
  byte screensCount = (_menuPageCurrent->itemsCount % menuItemsPerScreen == 0) ? _menuPageCurrent->itemsCount / menuItemsPerScreen : _menuPageCurrent->itemsCount / menuItemsPerScreen + 1;
  if (screensCount > 1) {
    byte currentScreenNum = _menuPageCurrent->currentItemNum / menuItemsPerScreen;
    byte menuPageScreenTopOffset = getCurrentAppearance()->menuPageScreenTopOffset;
    byte scrollbarHeight = (_u8g2.getDisplayHeight() - menuPageScreenTopOffset + 1) / screensCount;
    byte scrollbarPosition = currentScreenNum * scrollbarHeight + menuPageScreenTopOffset - 1;
    _u8g2.drawLine(_u8g2.getDisplayWidth() - 1, scrollbarPosition, _u8g2.getDisplayWidth() - 1, scrollbarPosition + scrollbarHeight);
  }
}

//====================== MENU ITEMS NAVIGATION

void GEM_u8g2::nextMenuItem() {
  if (_menuPageCurrent->currentItemNum == _menuPageCurrent->itemsCount-1) {
    _menuPageCurrent->currentItemNum = 0;
  } else {
    _menuPageCurrent->currentItemNum++;
  }
  drawMenu();
}

void GEM_u8g2::prevMenuItem() {
  if (_menuPageCurrent->currentItemNum == 0) {
    _menuPageCurrent->currentItemNum = _menuPageCurrent->itemsCount-1;
  } else {
    _menuPageCurrent->currentItemNum--;
  }
  drawMenu();
}

void GEM_u8g2::menuItemSelect() {
  GEMItem* menuItemTmp = _menuPageCurrent->getCurrentMenuItem();
  switch (menuItemTmp->type) {
    case GEM_ITEM_VAL:
      if (!menuItemTmp->readonly) {
        enterEditValueMode();
      }
      break;
    case GEM_ITEM_LINK:
      if (!menuItemTmp->readonly) {
        _menuPageCurrent = menuItemTmp->linkedPage;
        drawMenu();
      }
      break;
    case GEM_ITEM_BACK:
      _menuPageCurrent->currentItemNum = (_menuPageCurrent->itemsCount > 1) ? 1 : 0;
      _menuPageCurrent = menuItemTmp->linkedPage;
      drawMenu();
      break;
    case GEM_ITEM_BUTTON:
      if (!menuItemTmp->readonly) {
        if (menuItemTmp->callbackWithArgs) {
          menuItemTmp->callbackActionArg(menuItemTmp->callbackData);
        } else {
          menuItemTmp->callbackAction();
        }
      }
      break;
  }
}

//====================== VALUE EDIT

void GEM_u8g2::enterEditValueMode() {
  _editValueMode = true;
  
  GEMItem* menuItemTmp = _menuPageCurrent->getCurrentMenuItem();
  _editValueType = menuItemTmp->linkedType;
  switch (_editValueType) {
    case GEM_VAL_INTEGER:
      itoa(*(int*)menuItemTmp->linkedVariable, _valueString, 10);
      _editValueLength = 6;
      initEditValueCursor();
      break;
    case GEM_VAL_BYTE:
      itoa(*(byte*)menuItemTmp->linkedVariable, _valueString, 10);
      _editValueLength = 3;
      initEditValueCursor();
      break;
    case GEM_VAL_CHAR:
      strcpy(_valueString, (char*)menuItemTmp->linkedVariable);
      _editValueLength = GEM_STR_LEN - 1;
      initEditValueCursor();
      break;
    case GEM_VAL_BOOL:
      checkboxToggle();
      drawMenu();
      break;
    case GEM_VAL_SELECT:
      {
        GEMSelect* select = menuItemTmp->select;
        _valueSelectNum = select->getSelectedOptionNum(menuItemTmp->linkedVariable);
        initEditValueCursor();
      }
      break;
    #ifdef GEM_SUPPORT_SPINNER
    case GEM_VAL_SPINNER:
      {
        GEMSpinner* spinner = menuItemTmp->spinner;
        _valueSelectNum = spinner->getSelectedOptionNum(menuItemTmp->linkedVariable);
        initEditValueCursor();
      }
      break;
    #endif
    #ifdef GEM_SUPPORT_FLOAT_EDIT
    case GEM_VAL_FLOAT:
      // sprintf(_valueString,"%.6f", *(float*)menuItemTmp->linkedVariable); // May work for non-AVR boards
      dtostrf(*(float*)menuItemTmp->linkedVariable, menuItemTmp->precision + 1, menuItemTmp->precision, _valueString);
      _editValueLength = GEM_STR_LEN - 1;
      initEditValueCursor();
      break;
    case GEM_VAL_DOUBLE:
      // sprintf(_valueString,"%.6f", *(double*)menuItemTmp->linkedVariable); // May work for non-AVR boards
      dtostrf(*(double*)menuItemTmp->linkedVariable, menuItemTmp->precision + 1, menuItemTmp->precision, _valueString);
      _editValueLength = GEM_STR_LEN - 1;
      initEditValueCursor();
      break;
    #endif
  }
}

void GEM_u8g2::checkboxToggle() {
  GEMItem* menuItemTmp = _menuPageCurrent->getCurrentMenuItem();
  bool checkboxValue = *(bool*)menuItemTmp->linkedVariable;
  *(bool*)menuItemTmp->linkedVariable = !checkboxValue;
  if (menuItemTmp->callbackAction != nullptr) {
    resetEditValueState(); // Explicitly reset edit value state to be more predictable before user-defined callback is called
    if (menuItemTmp->callbackWithArgs) {
      menuItemTmp->callbackActionArg(menuItemTmp->callbackData);
    } else {
      menuItemTmp->callbackAction();
    }
    drawEditValueCursor();
    drawMenu();
  } else {
    _editValueMode = false;
  }
}

void GEM_u8g2::initEditValueCursor() {
  _editValueCursorPosition = 0;
  _editValueVirtualCursorPosition = 0;
  drawMenu();
}

void GEM_u8g2::nextEditValueCursorPosition() {
  if ((_editValueCursorPosition != getMenuItemValueLength() - 1) && (_editValueCursorPosition != _editValueLength - 1) && (_valueString[_editValueCursorPosition] != '\0')) {
    _editValueCursorPosition++;
  }
  if ((_editValueVirtualCursorPosition != _editValueLength - 1) && (_valueString[_editValueVirtualCursorPosition] != '\0')) {
    _editValueVirtualCursorPosition++;
  }
  drawMenu();
}

void GEM_u8g2::prevEditValueCursorPosition() {
  if (_editValueCursorPosition != 0) {
    _editValueCursorPosition--;
  }
  if (_editValueVirtualCursorPosition != 0) {
    _editValueVirtualCursorPosition--;
  }
  drawMenu();
}

void GEM_u8g2::drawEditValueCursor() {
  int pointerPosition = getCurrentItemTopOffset(false);
  byte menuItemFontSize = getMenuItemFontSize();
  byte cursorLeftOffset = getCurrentAppearance()->menuValuesLeftOffset + _editValueCursorPosition * _menuItemFont[menuItemFontSize].width;
  _u8g2.setDrawColor(2);
  if (_editValueType == GEM_VAL_SELECT || _editValueType == GEM_VAL_SPINNER) {
    _u8g2.drawBox(cursorLeftOffset - 1, pointerPosition - 1, _u8g2.getDisplayWidth() - cursorLeftOffset - 1, getCurrentAppearance()->menuItemHeight + 1);
  } else {
    _u8g2.drawBox(cursorLeftOffset - 1, pointerPosition - 1, _menuItemFont[menuItemFontSize].width + 1, getCurrentAppearance()->menuItemHeight + 1);
  }
  _u8g2.setDrawColor(1);
}

void GEM_u8g2::nextEditValueDigit() {
  GEMItem* menuItemTmp = _menuPageCurrent->getCurrentMenuItem();
  char chr = _valueString[_editValueVirtualCursorPosition];
  byte code = (byte)chr;
  if (_editValueType == GEM_VAL_CHAR) {
    if (menuItemTmp->adjustedAsciiOrder) {
      switch (code) {
        case 0:
          code = GEM_CHAR_CODE_a;
          break;
        case GEM_CHAR_CODE_SPACE:
          code = GEM_CHAR_CODE_a;
          break;
        case GEM_CHAR_CODE_ACCENT:
          code = GEM_CHAR_CODE_SPACE;
          break;
        case GEM_CHAR_CODE_TILDA:
          code = GEM_CHAR_CODE_BANG;
          break;
        default:
          code++;
          break;
      }
    } else {
      switch (code) {
        case 0:
          code = GEM_CHAR_CODE_SPACE;
          break;
        case GEM_CHAR_CODE_TILDA:
          code = GEM_CHAR_CODE_SPACE;
          break;
        /*
        // WIP for Cyrillic values support
        case GEM_CHAR_CODE_TILDA:
          code = _cyrillicEnabled ? GEM_CHAR_CODE_CYR_A : GEM_CHAR_CODE_SPACE;
          break;
        case GEM_CHAR_CODE_CYR_YA_SM:
          code = GEM_CHAR_CODE_SPACE;
          break;
        case GEM_CHAR_CODE_CYR_E:
          code = GEM_CHAR_CODE_CYR_YO;
          break;
        case GEM_CHAR_CODE_CYR_YO:
          code = GEM_CHAR_CODE_CYR_E + 1;
          break;
        case GEM_CHAR_CODE_CYR_E_SM:
          code = GEM_CHAR_CODE_CYR_YO_SM;
          break;
        case GEM_CHAR_CODE_CYR_YO_SM:
          code = GEM_CHAR_CODE_CYR_E_SM + 1;
          break;
        */
        default:
          code++;
          break;
      }
    }
  } else {
    switch (code) {
      case 0:
        code = GEM_CHAR_CODE_0;
        break;
      case GEM_CHAR_CODE_9:
        code = (_editValueCursorPosition == 0 && (_editValueType == GEM_VAL_INTEGER || _editValueType == GEM_VAL_FLOAT || _editValueType == GEM_VAL_DOUBLE)) ? GEM_CHAR_CODE_MINUS : GEM_CHAR_CODE_SPACE;
        break;
      case GEM_CHAR_CODE_MINUS:
        code = GEM_CHAR_CODE_SPACE;
        break;
      case GEM_CHAR_CODE_SPACE:
        code = (_editValueCursorPosition != 0 && (_editValueType == GEM_VAL_FLOAT || _editValueType == GEM_VAL_DOUBLE)) ? GEM_CHAR_CODE_DOT : GEM_CHAR_CODE_0;
        break;
      case GEM_CHAR_CODE_DOT:
        code = GEM_CHAR_CODE_0;
        break;
      default:
        code++;
        break;
    }
  }
  drawEditValueDigit(code);
}

void GEM_u8g2::prevEditValueDigit() {
  GEMItem* menuItemTmp = _menuPageCurrent->getCurrentMenuItem();
  char chr = _valueString[_editValueVirtualCursorPosition];
  byte code = (byte)chr;
  if (_editValueType == GEM_VAL_CHAR) {
    if (menuItemTmp->adjustedAsciiOrder) {
      switch (code) {
        case 0:
          code = GEM_CHAR_CODE_ACCENT;
          break;
        case GEM_CHAR_CODE_BANG:
          code = GEM_CHAR_CODE_TILDA;
          break;
        case GEM_CHAR_CODE_a:
          code = GEM_CHAR_CODE_SPACE;
          break;
        case GEM_CHAR_CODE_SPACE:
          code = GEM_CHAR_CODE_ACCENT;
          break;
        default:
          code--;
          break;
      }
    } else {
      switch (code) {
        case 0:
          code = GEM_CHAR_CODE_TILDA;
          break;
        case GEM_CHAR_CODE_SPACE:
          code = GEM_CHAR_CODE_TILDA;
          break;
        /*
        // WIP for Cyrillic values support
        case 0:
          code = _cyrillicEnabled ? GEM_CHAR_CODE_CYR_YA_SM : GEM_CHAR_CODE_TILDA;
          break;
        case GEM_CHAR_CODE_SPACE:
          code = _cyrillicEnabled ? GEM_CHAR_CODE_CYR_YA_SM : GEM_CHAR_CODE_TILDA;
          break;
        case GEM_CHAR_CODE_CYR_A:
          code = GEM_CHAR_CODE_TILDA;
          break;
        case GEM_CHAR_CODE_CYR_E + 1:
          code = GEM_CHAR_CODE_CYR_YO;
          break;
        case GEM_CHAR_CODE_CYR_YO:
          code = GEM_CHAR_CODE_CYR_E;
          break;
        case GEM_CHAR_CODE_CYR_E_SM + 1:
          code = GEM_CHAR_CODE_CYR_YO_SM;
          break;
        case GEM_CHAR_CODE_CYR_YO_SM:
          code = GEM_CHAR_CODE_CYR_E_SM;
          break;
        */
        default:
          code--;
          break;
      }
    }
  } else {
    switch (code) {
      case 0:
        code = (_editValueCursorPosition == 0 && (_editValueType == GEM_VAL_INTEGER || _editValueType == GEM_VAL_FLOAT || _editValueType == GEM_VAL_DOUBLE)) ? GEM_CHAR_CODE_MINUS : GEM_CHAR_CODE_9;
        break;
      case GEM_CHAR_CODE_MINUS:
        code = GEM_CHAR_CODE_9;
        break;
      case GEM_CHAR_CODE_0:
        code = (_editValueCursorPosition != 0 && (_editValueType == GEM_VAL_FLOAT || _editValueType == GEM_VAL_DOUBLE)) ? GEM_CHAR_CODE_DOT : GEM_CHAR_CODE_SPACE;
        break;
      case GEM_CHAR_CODE_SPACE:
        code = (_editValueCursorPosition == 0 && (_editValueType == GEM_VAL_INTEGER || _editValueType == GEM_VAL_FLOAT || _editValueType == GEM_VAL_DOUBLE)) ? GEM_CHAR_CODE_MINUS : GEM_CHAR_CODE_9;
        break;
      case GEM_CHAR_CODE_DOT:
        code = GEM_CHAR_CODE_SPACE;
        break;
      default:
        code--;
        break;
    }
  }
  drawEditValueDigit(code);
}

void GEM_u8g2::drawEditValueDigit(byte code) {
  char chrNew = (char)code;
  _valueString[_editValueVirtualCursorPosition] = chrNew;
  drawMenu();
}

void GEM_u8g2::nextEditValueSelect() {
  GEMItem* menuItemTmp = _menuPageCurrent->getCurrentMenuItem();
  GEMSelect* select = menuItemTmp->select;
  if (_valueSelectNum+1 < select->getLength()) {
    _valueSelectNum++;
  } else if (select->getLoop()) {
    _valueSelectNum = 0;
  }
  drawMenu();
}

void GEM_u8g2::prevEditValueSelect() {
  GEMItem* menuItemTmp = _menuPageCurrent->getCurrentMenuItem();
  GEMSelect* select = menuItemTmp->select;
  if (_valueSelectNum > 0) {
    _valueSelectNum--;
  } else if (select->getLoop()) {
    _valueSelectNum = select->getLength() - 1;
  }
  drawMenu();
}

#ifdef GEM_SUPPORT_SPINNER
void GEM_u8g2::nextEditValueSpinner() {
  GEMItem* menuItemTmp = _menuPageCurrent->getCurrentMenuItem();
  GEMSpinner* spinner = menuItemTmp->spinner;
  if (_valueSelectNum+1 < spinner->getLength()) {
    _valueSelectNum++;
  } else if (spinner->getLoop()) {
    _valueSelectNum = 0;
  }
  drawMenu();
}

void GEM_u8g2::prevEditValueSpinner() {
  GEMItem* menuItemTmp = _menuPageCurrent->getCurrentMenuItem();
  GEMSpinner* spinner = menuItemTmp->spinner;
  if (_valueSelectNum > 0) {
    _valueSelectNum--;
  } else if (spinner->getLoop()) {
    _valueSelectNum = spinner->getLength() - 1;
  }
  drawMenu();
}
#endif

void GEM_u8g2::saveEditValue() {
  GEMItem* menuItemTmp = _menuPageCurrent->getCurrentMenuItem();
  switch (menuItemTmp->linkedType) {
    case GEM_VAL_INTEGER:
      *(int*)menuItemTmp->linkedVariable = atoi(_valueString);
      break;
    case GEM_VAL_BYTE:
      *(byte*)menuItemTmp->linkedVariable = atoi(_valueString);
      break;
    case GEM_VAL_CHAR:
      strcpy((char*)menuItemTmp->linkedVariable, trimString(_valueString)); // Potential overflow if string length is not defined
      break;
    case GEM_VAL_SELECT:
      {
        GEMSelect* select = menuItemTmp->select;
        select->setValue(menuItemTmp->linkedVariable, _valueSelectNum);
      }
      break;
    #ifdef GEM_SUPPORT_SPINNER
    case GEM_VAL_SPINNER:
      {
        GEMSpinner* spinner = menuItemTmp->spinner;
        spinner->setValue(menuItemTmp->linkedVariable, _valueSelectNum);
      }
      break;
    #endif
    #ifdef GEM_SUPPORT_FLOAT_EDIT
    case GEM_VAL_FLOAT:
      *(float*)menuItemTmp->linkedVariable = atof(_valueString);
      break;
    case GEM_VAL_DOUBLE:
      *(double*)menuItemTmp->linkedVariable = atof(_valueString);
      break;
    #endif
  }
  if (menuItemTmp->callbackAction != nullptr) {
    resetEditValueState(); // Explicitly reset edit value state to be more predictable before user-defined callback is called
    if (menuItemTmp->callbackWithArgs) {
      menuItemTmp->callbackActionArg(menuItemTmp->callbackData);
    } else {
      menuItemTmp->callbackAction();
    }
    drawEditValueCursor();
    drawMenu();
  } else {
    exitEditValue();
  }
}

void GEM_u8g2::cancelEditValue() {
  exitEditValue();
}

void GEM_u8g2::resetEditValueState() {
  memset(_valueString, '\0', GEM_STR_LEN - 1);
  _valueSelectNum = -1;
  _editValueMode = false;
}

void GEM_u8g2::exitEditValue() {
  resetEditValueState();
  drawEditValueCursor();
  drawMenu();
}

bool GEM_u8g2::isEditMode() {
  return _editValueMode;
}

// Trim leading/trailing whitespaces
// Author: Adam Rosenfield, https://stackoverflow.com/a/122721
char* GEM_u8g2::trimString(char* str) {
  char *end;

  // Trim leading space
  while(isspace((unsigned char)*str)) str++;

  if(*str == 0)  // All spaces?
    return str;

  // Trim trailing space
  end = str + strlen(str) - 1;
  while(end > str && isspace((unsigned char)*end)) end--;

  // Write new null terminator
  *(end+1) = 0;
  
  return str;
}

//====================== KEY DETECTION

bool GEM_u8g2::readyForKey() {
  if ( (context.loop == nullptr) ||
      ((context.loop != nullptr) && (context.allowExit)) ) {
    return true;
  } else {
    registerKeyPress(GEM_KEY_NONE);
    return false;
  }

}

GEM_u8g2& GEM_u8g2::registerKeyPress(byte keyCode) {
  _currentKey = keyCode;
  dispatchKeyPress();
  return *this;
}

void GEM_u8g2::dispatchKeyPress() {

  if (context.loop != nullptr) {
    if ((context.allowExit) && (_currentKey == GEM_KEY_CANCEL)) {
      if (context.exit != nullptr) {
        context.exit();
      } else {
        reInit();
        drawMenu();
        clearContext();
      }
    } else {
      context.loop();
    }
  } else {
  
    if (_editValueMode) {
      switch (_currentKey) {
        case GEM_KEY_UP:
          if (_editValueType == GEM_VAL_SELECT) {
            prevEditValueSelect();
          #ifdef GEM_SUPPORT_SPINNER
          } else if (_editValueType == GEM_VAL_SPINNER) {
            if (_invertKeysDuringEdit) {
              prevEditValueSpinner();
            } else {
              nextEditValueSpinner();
            }
          #endif
          } else if (_invertKeysDuringEdit) {
            prevEditValueDigit();
          } else {
            nextEditValueDigit();
          }
          break;
        case GEM_KEY_RIGHT:
          if (_editValueType != GEM_VAL_SELECT && _editValueType != GEM_VAL_SPINNER) {
            nextEditValueCursorPosition();
          }
          break;
        case GEM_KEY_DOWN:
          if (_editValueType == GEM_VAL_SELECT) {
            nextEditValueSelect();
          #ifdef GEM_SUPPORT_SPINNER
          } else if (_editValueType == GEM_VAL_SPINNER) {
            if (_invertKeysDuringEdit) {
              nextEditValueSpinner();
            } else {
              prevEditValueSpinner();
            }
          #endif
          } else if (_invertKeysDuringEdit) {
            nextEditValueDigit();
          } else {
            prevEditValueDigit();
          }
          break;
        case GEM_KEY_LEFT:
          if (_editValueType != GEM_VAL_SELECT && _editValueType != GEM_VAL_SPINNER) {
            prevEditValueCursorPosition();
          }
          break;
        case GEM_KEY_CANCEL:
          cancelEditValue();
          break;
        case GEM_KEY_OK:
          saveEditValue();
          break;
      }
    } else {
      switch (_currentKey) {
        case GEM_KEY_UP:
          prevMenuItem();
          break;
        case GEM_KEY_RIGHT:
          if (_menuPageCurrent->getCurrentMenuItem()->type == GEM_ITEM_LINK ||
              _menuPageCurrent->getCurrentMenuItem()->type == GEM_ITEM_BUTTON) {
            menuItemSelect();
          }
          break;
        case GEM_KEY_DOWN:
          nextMenuItem();
          break;
        case GEM_KEY_LEFT:
          if (_menuPageCurrent->getCurrentMenuItem()->type == GEM_ITEM_BACK) {
            menuItemSelect();
          }
          break;
        case GEM_KEY_CANCEL:
          if (_menuPageCurrent->getMenuItem(0)->type == GEM_ITEM_BACK) {
            _menuPageCurrent->currentItemNum = 0;
            menuItemSelect();
          } else if (_menuPageCurrent->exitAction != nullptr) {
            _menuPageCurrent->currentItemNum = 0;
            _menuPageCurrent->exitAction();
          }
          break;
        case GEM_KEY_OK:
          menuItemSelect();
          break;
      }
    }

  }
}

#endif
