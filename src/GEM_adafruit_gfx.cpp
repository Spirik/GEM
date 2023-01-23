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

#include <Arduino.h>
#include "GEM_adafruit_gfx.h"

#ifdef GEM_ENABLE_ADAFRUIT_GFX_VERSION

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
#define GEM_CHAR_CODE_LINE 124
#define GEM_CHAR_CODE_TILDA 126

// Sprite of the default GEM _splash screen (GEM logo v1)
/*
#define logo_width  12
#define logo_height 12
static const uint8_t logo_bits [] PROGMEM = {
  0x03, 0xe0, 0x06, 0x70, 0x0c, 0x70, 0x18, 0xf0, 0x31, 0xf0, 0x63, 0xf0, 0xc7, 0xf0, 0x8f, 0xf0, 
	0x98, 0xf0, 0xfe, 0x30, 0xff, 0xb0, 0x7f, 0xe0
};
*/

// Sprite of the default GEM _splash screen (GEM logo v2)
#define logo_width  20
#define logo_height 8
static const uint8_t logo_bits [] PROGMEM = {
  0xf1, 0xf2, 0x20, 0x00, 0x02, 0x20, 0x00, 0x02, 0xa0, 0x19, 0xe2, 0xa0, 0x00, 0x02, 0x20, 0x00, 
	0x02, 0x20, 0xf9, 0xf2, 0x20, 0x00, 0x00, 0x00
};

// Sprites of the UI elements used to draw menu

#define arrowRight_width  6
#define arrowRight_height 8
static const uint8_t arrowRight_bits [] PROGMEM = {
  0x00, 0x20, 0x30, 0x38, 0x30, 0x20, 0x00, 0x00
};

#define arrowLeft_width  6
#define arrowLeft_height 8
static const uint8_t arrowLeft_bits [] PROGMEM = {
  0x00, 0x20, 0x60, 0xe0, 0x60, 0x20, 0x00, 0x00
};

#define arrowBtn_width  6
#define arrowBtn_height 8
static const uint8_t arrowBtn_bits [] PROGMEM = {
  0x00, 0xc0, 0xa0, 0x90, 0xa0, 0xc0, 0x00, 0x00
};

#define checkboxUnchecked_width  7
#define checkboxUnchecked_height 8
static const uint8_t checkboxUnchecked_bits [] PROGMEM = {
  0x00, 0xfc, 0x84, 0x84, 0x84, 0x84, 0xfc, 0x00
};

#define checkboxChecked_width  7
#define checkboxChecked_height 8
static const uint8_t checkboxChecked_bits [] PROGMEM = {
  0x02, 0xf4, 0x8c, 0xd4, 0xa4, 0x84, 0xfc, 0x00
};

#define selectArrows_width  6
#define selectArrows_height 8
static const uint8_t selectArrows_bits [] PROGMEM = {
  0x00, 0x20, 0x70, 0x00, 0x70, 0x20, 0x00, 0x00
};

GEM_adafruit_gfx::GEM_adafruit_gfx(Adafruit_GFX& agfx_, byte menuPointerType_, byte menuItemsPerScreen_, byte menuItemHeight_, byte menuPageScreenTopOffset_, byte menuValuesLeftOffset_)
  : _agfx(agfx_)
  , _menuPointerType(menuPointerType_)
  , _menuItemsPerScreen(menuItemsPerScreen_)
  , _menuItemHeight(menuItemHeight_)
  , _menuPageScreenTopOffset(menuPageScreenTopOffset_)
  , _menuValuesLeftOffset(menuValuesLeftOffset_)
{
  _menuItemFontSize = _menuItemHeight >= 8 ? 0 : 1;
  _menuItemInsetOffset = (_menuItemHeight - _menuItemFont[_menuItemFontSize].height) / 2;
  _splash = {logo_width, logo_height, logo_bits};
  clearContext();
  _editValueMode = false;
  _editValueCursorPosition = 0;
  memset(_valueString, '\0', GEM_STR_LEN - 1);
  _valueSelectNum = -1;
}

//====================== INIT OPERATIONS

void GEM_adafruit_gfx::setSplash(byte width, byte height, const uint8_t *image) {
  _splash = {width, height, image};
}

void GEM_adafruit_gfx::setSplashDelay(uint16_t value) {
  _splashDelay = value;
}

void GEM_adafruit_gfx::hideVersion(boolean flag) {
  _enableVersion = !flag;
}

void GEM_adafruit_gfx::setForegroundColor(uint16_t color) {
  _menuForegroundColor = color;
}

void GEM_adafruit_gfx::setBackgroundColor(uint16_t color) {
  _menuBackgroundColor = color;
}

void GEM_adafruit_gfx::init() {
  _agfx.setTextSize(1);
  _agfx.setTextWrap(false);
  _agfx.setTextColor(_menuForegroundColor);
  _agfx.fillScreen(_menuBackgroundColor);
  
  _menuItemTitleLength = (_menuValuesLeftOffset - 5) / _menuItemFont[_menuItemFontSize].width;
  _menuItemValueLength = (_agfx.width() - _menuValuesLeftOffset - 6) / _menuItemFont[_menuItemFontSize].width;
  if (_menuItemsPerScreen == GEM_ITEMS_COUNT_AUTO) {
    _menuItemsPerScreen = (_agfx.height() - _menuPageScreenTopOffset) / _menuItemHeight;
  }

  if (_splashDelay > 0) {

    _agfx.drawBitmap((_agfx.width() - _splash.width) / 2, (_agfx.height() - _splash.height) / 2, _splash.image, _splash.width, _splash.height, _menuForegroundColor);

    if (_enableVersion) {
      delay(_splashDelay / 2);
      _agfx.setFont(_fontFamilies.small);
      byte x = _agfx.width() - strlen(GEM_VER)*4;
      byte y = _agfx.height() - 1;
      if (_splash.image != logo_bits) {
        _agfx.setCursor(x - 12, y);
        _agfx.print("GEM");
      } else {
        _agfx.setCursor(x, y);
      }
      _agfx.print(GEM_VER);
      delay(_splashDelay / 2);
    } else {
      delay(_splashDelay);
    }

    _agfx.fillScreen(_menuBackgroundColor);

  }
}

void GEM_adafruit_gfx::reInit() {
  _agfx.setTextSize(1);
  _agfx.setTextWrap(false);
  _agfx.setTextColor(_menuForegroundColor);
  _agfx.fillScreen(_menuBackgroundColor);
}

void GEM_adafruit_gfx::setMenuPageCurrent(GEMPage& menuPageCurrent) {
  _menuPageCurrent = &menuPageCurrent;
}

//====================== CONTEXT OPERATIONS

void GEM_adafruit_gfx::clearContext() {
  context.loop = nullptr;
  context.enter = nullptr;
  context.exit = nullptr;
  context.allowExit = true;
}

//====================== DRAW OPERATIONS

void GEM_adafruit_gfx::drawMenu() {
  _agfx.fillScreen(_menuBackgroundColor);
  drawTitleBar();
  printMenuItems();
  drawMenuPointer();
  drawScrollbar();
}

void GEM_adafruit_gfx::drawTitleBar() {
  _agfx.setFont(_fontFamilies.small);
  _agfx.setTextWrap(true);
  _agfx.setTextColor(_menuForegroundColor);
  _agfx.setCursor(5, _menuItemFont[1].baselineOffset + 1);
  _agfx.print(_menuPageCurrent->title);
  _agfx.setTextWrap(false);
  _agfx.setFont(_menuItemFontSize ? _fontFamilies.small : _fontFamilies.big);
}

void GEM_adafruit_gfx::printMenuItemString(const char* str, byte num, byte startPos) {
  byte i = startPos;
  while (i < num + startPos && str[i] != '\0') {
    _agfx.print(str[i]);
    i++;
  }
}

void GEM_adafruit_gfx::printMenuItemTitle(const char* str, int offset) {
  printMenuItemString(str, _menuItemTitleLength + offset);
}

void GEM_adafruit_gfx::printMenuItemValue(const char* str, int offset, byte startPos) {
  printMenuItemString(str, _menuItemValueLength + offset, startPos);
}

void GEM_adafruit_gfx::printMenuItemFull(const char* str, int offset) {
  printMenuItemString(str, _menuItemTitleLength + _menuItemValueLength + offset);
}

byte GEM_adafruit_gfx::getMenuItemInsetOffset(boolean forSprite) {
  return _menuItemInsetOffset + (forSprite ? (_menuItemFontSize ? -1 : 2) : -1 ); // With additional offset for 6x8 sprites to compensate for smaller font size
}

byte GEM_adafruit_gfx::getCurrentItemTopOffset(boolean withInsetOffset, boolean forSprite) {
  return (_menuPageCurrent->currentItemNum % _menuItemsPerScreen) * _menuItemHeight + _menuPageScreenTopOffset + (withInsetOffset ? getMenuItemInsetOffset(forSprite) : 0);
}

void GEM_adafruit_gfx::printMenuItem(GEMItem* menuItemTmp, byte yText, byte yDraw, uint16_t color) {
  _agfx.setTextColor(color);
  switch (menuItemTmp->type) {
      case GEM_ITEM_VAL:
        _agfx.setCursor(5, yText);
        if (menuItemTmp->readonly) {
          printMenuItemTitle(menuItemTmp->title, -1);
          _agfx.print("^");
        } else {
          printMenuItemTitle(menuItemTmp->title);
        }

        _agfx.setCursor(_menuValuesLeftOffset, yText);
        switch (menuItemTmp->linkedType) {
          case GEM_VAL_INTEGER:
            itoa(*(int*)menuItemTmp->linkedVariable, _valueString, 10);
            printMenuItemValue(_valueString);
            break;
          case GEM_VAL_BYTE:
            itoa(*(byte*)menuItemTmp->linkedVariable, _valueString, 10);
            printMenuItemValue(_valueString);
            break;
          case GEM_VAL_CHAR:
            printMenuItemValue((char*)menuItemTmp->linkedVariable);
            break;
          case GEM_VAL_BOOLEAN:
            if (*(boolean*)menuItemTmp->linkedVariable) {
              _agfx.drawBitmap(_menuValuesLeftOffset, yDraw, checkboxChecked_bits, checkboxChecked_width, checkboxChecked_height, color);
            } else {
              _agfx.drawBitmap(_menuValuesLeftOffset, yDraw, checkboxUnchecked_bits, checkboxUnchecked_width, checkboxUnchecked_height, color);
            }
            break;
          case GEM_VAL_SELECT:
            {
              GEMSelect* select = menuItemTmp->select;
              printMenuItemValue(select->getSelectedOptionName(menuItemTmp->linkedVariable));
              _agfx.drawBitmap(_agfx.width() - 7, yDraw, selectArrows_bits, selectArrows_width, selectArrows_height, color);
            }
            break;
          #ifdef GEM_SUPPORT_FLOAT_EDIT
          case GEM_VAL_FLOAT:
            // sprintf(_valueString,"%.6f", *(float*)menuItemTmp->linkedVariable); // May work for non-AVR boards
            dtostrf(*(float*)menuItemTmp->linkedVariable, menuItemTmp->precision + 1, menuItemTmp->precision, _valueString);
            printMenuItemValue(_valueString);
            break;
          case GEM_VAL_DOUBLE:
            // sprintf(_valueString,"%.6f", *(double*)menuItemTmp->linkedVariable); // May work for non-AVR boards
            dtostrf(*(double*)menuItemTmp->linkedVariable, menuItemTmp->precision + 1, menuItemTmp->precision, _valueString);
            printMenuItemValue(_valueString);
            break;
          #endif
        }
        break;
      case GEM_ITEM_LINK:
        _agfx.setCursor(5, yText);
        if (menuItemTmp->readonly) {
          printMenuItemFull(menuItemTmp->title, -1);
          _agfx.print("^");
        } else {
          printMenuItemFull(menuItemTmp->title);
        }
        _agfx.drawBitmap(_agfx.width() - 8, yDraw, arrowRight_bits, arrowRight_width, arrowRight_height, color);
        break;
      case GEM_ITEM_BACK:
        _agfx.setCursor(11, yText);
        _agfx.drawBitmap(5, yDraw, arrowLeft_bits, arrowLeft_width, arrowLeft_height, color);
        break;
      case GEM_ITEM_BUTTON:
        _agfx.setCursor(11, yText);
        if (menuItemTmp->readonly) {
          printMenuItemFull(menuItemTmp->title, -1);
          _agfx.print("^");
        } else {
          printMenuItemFull(menuItemTmp->title);
        }
        _agfx.drawBitmap(5, yDraw, arrowBtn_bits, arrowBtn_width, arrowBtn_height, color);
        break;
    }
  _agfx.setTextColor(_menuForegroundColor);
}

void GEM_adafruit_gfx::printMenuItems() {
  byte currentPageScreenNum = _menuPageCurrent->currentItemNum / _menuItemsPerScreen;
  GEMItem* menuItemTmp = _menuPageCurrent->getMenuItem(currentPageScreenNum * _menuItemsPerScreen);
  byte y = _menuPageScreenTopOffset;
  byte i = 0;
  while (menuItemTmp != nullptr && i < _menuItemsPerScreen) {
    byte yText = y + getMenuItemInsetOffset() + _menuItemFont[_menuItemFontSize].baselineOffset;
    byte yDraw = y + getMenuItemInsetOffset(true);

    printMenuItem(menuItemTmp, yText, yDraw, _menuForegroundColor);

    menuItemTmp = menuItemTmp->getMenuItemNext();
    y += _menuItemHeight;
    i++;
  }
  memset(_valueString, '\0', GEM_STR_LEN - 1);
}

void GEM_adafruit_gfx::drawMenuPointer(boolean clear) {
  if (_menuPageCurrent->itemsCount > 0) {
    GEMItem* menuItemTmp = _menuPageCurrent->getCurrentMenuItem();
    int pointerPosition = getCurrentItemTopOffset(false);
    if (_menuPointerType == GEM_POINTER_DASH) {
      _agfx.fillRect(0, _menuPageScreenTopOffset, 2, _agfx.height() - _menuPageScreenTopOffset, _menuBackgroundColor);
      if (menuItemTmp->readonly) {
        for (byte i = 0; i < (_menuItemHeight - 1) / 2; i++) {
          _agfx.drawPixel(0, pointerPosition + i * 2, _menuForegroundColor);
          _agfx.drawPixel(1, pointerPosition + i * 2 + 1, _menuForegroundColor);
        }
      } else {
        _agfx.fillRect(0, pointerPosition, 2, _menuItemHeight - 1, _menuForegroundColor);
      }
      if (clear) {
        byte yText = pointerPosition + getMenuItemInsetOffset() + _menuItemFont[_menuItemFontSize].baselineOffset;
        byte yDraw = pointerPosition + getMenuItemInsetOffset(true);
        _agfx.fillRect(5, pointerPosition - 1, _agfx.width() - 2, _menuItemHeight + 1, _menuBackgroundColor);
        printMenuItem(menuItemTmp, yText, yDraw, _menuForegroundColor);
      }
    } else {
      byte yText = pointerPosition + getMenuItemInsetOffset() + _menuItemFont[_menuItemFontSize].baselineOffset;
      byte yDraw = pointerPosition + getMenuItemInsetOffset(true);
      byte screensCount = (_menuPageCurrent->itemsCount % _menuItemsPerScreen == 0) ? _menuPageCurrent->itemsCount / _menuItemsPerScreen : _menuPageCurrent->itemsCount / _menuItemsPerScreen + 1;
      _agfx.fillRect(0, pointerPosition - 1, _agfx.width() + (screensCount > 1 ? -2 : 0), _menuItemHeight + 1, clear ? _menuBackgroundColor : _menuForegroundColor);
      printMenuItem(menuItemTmp, yText, yDraw, clear ? _menuForegroundColor : _menuBackgroundColor);
      if (menuItemTmp->readonly) {
        for (byte i = 0; i < (_menuItemHeight + 2) / 2; i++) {
          _agfx.drawPixel(0, pointerPosition + i * 2, _menuBackgroundColor);
          _agfx.drawPixel(1, pointerPosition + i * 2 - 1, _menuBackgroundColor);
        }
      }
    }
  }
}

void GEM_adafruit_gfx::drawScrollbar() {
  byte screensCount = (_menuPageCurrent->itemsCount % _menuItemsPerScreen == 0) ? _menuPageCurrent->itemsCount / _menuItemsPerScreen : _menuPageCurrent->itemsCount / _menuItemsPerScreen + 1;
  if (screensCount > 1) {
    byte currentScreenNum = _menuPageCurrent->currentItemNum / _menuItemsPerScreen;
    byte scrollbarHeight = (_agfx.height() - _menuPageScreenTopOffset + 1) / screensCount;
    byte scrollbarPosition = currentScreenNum * scrollbarHeight + _menuPageScreenTopOffset - 1;
    _agfx.drawLine(_agfx.width() - 1, scrollbarPosition, _agfx.width() - 1, scrollbarPosition + scrollbarHeight, _menuForegroundColor);
  }
}

//====================== MENU ITEMS NAVIGATION

void GEM_adafruit_gfx::nextMenuItem() {
  if (_menuPointerType != GEM_POINTER_DASH) {
    drawMenuPointer(true);
  }
  if (_menuPageCurrent->currentItemNum == _menuPageCurrent->itemsCount-1) {
    _menuPageCurrent->currentItemNum = 0;
  } else {
    _menuPageCurrent->currentItemNum++;
  }
  boolean redrawMenu = (_menuPageCurrent->itemsCount > _menuItemsPerScreen && _menuPageCurrent->currentItemNum % _menuItemsPerScreen == 0);
  if (redrawMenu) {
    drawMenu();
  } else {
    drawMenuPointer();
  }
}

void GEM_adafruit_gfx::prevMenuItem() {
  if (_menuPointerType != GEM_POINTER_DASH) {
    drawMenuPointer(true);
  }
  boolean redrawMenu = (_menuPageCurrent->itemsCount > _menuItemsPerScreen && _menuPageCurrent->currentItemNum % _menuItemsPerScreen == 0);
  if (_menuPageCurrent->currentItemNum == 0) {
    _menuPageCurrent->currentItemNum = _menuPageCurrent->itemsCount-1;
  } else {
    _menuPageCurrent->currentItemNum--;
  }
  if (redrawMenu) {
    drawMenu();
  } else {
    drawMenuPointer();
  }
}

void GEM_adafruit_gfx::menuItemSelect() {
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

void GEM_adafruit_gfx::enterEditValueMode() {
  _editValueMode = true;
  memset(_valueString, '\0', GEM_STR_LEN - 1); // Not calling drawMenu() in exitEditValue() every time, so it's better to implement buffer cleanup here
  
  GEMItem* menuItemTmp = _menuPageCurrent->getCurrentMenuItem();
  _editValueType = menuItemTmp->linkedType;
  if ((_menuPointerType != GEM_POINTER_DASH) && (_editValueType != GEM_VAL_BOOLEAN)) {
    drawMenuPointer(true);
  }
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
    case GEM_VAL_BOOLEAN:
      checkboxToggle();
      break;
    case GEM_VAL_SELECT:
      {
        GEMSelect* select = menuItemTmp->select;
        _valueSelectNum = select->getSelectedOptionNum(menuItemTmp->linkedVariable);
        initEditValueCursor();
      }
      break;
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

void GEM_adafruit_gfx::checkboxToggle() {
  GEMItem* menuItemTmp = _menuPageCurrent->getCurrentMenuItem();
  int topOffset = getCurrentItemTopOffset(true, true);
  boolean checkboxValue = *(boolean*)menuItemTmp->linkedVariable;
  *(boolean*)menuItemTmp->linkedVariable = !checkboxValue;
  if (menuItemTmp->callbackAction != nullptr) {
    if (menuItemTmp->callbackWithArgs) {
      menuItemTmp->callbackActionArg(menuItemTmp->callbackData);
    } else {
      menuItemTmp->callbackAction();
    }
    exitEditValue();
  } else {
    uint16_t foreColor = (_menuPointerType == GEM_POINTER_DASH) ? _menuForegroundColor : _menuBackgroundColor;
    uint16_t backColor = (_menuPointerType == GEM_POINTER_DASH) ? _menuBackgroundColor : _menuForegroundColor;
    if (!checkboxValue) {
      _agfx.fillRect(_menuValuesLeftOffset, topOffset, checkboxChecked_width, checkboxChecked_height, backColor);
      _agfx.drawBitmap(_menuValuesLeftOffset, topOffset, checkboxChecked_bits, checkboxChecked_width, checkboxChecked_height, foreColor);
    } else {
      _agfx.fillRect(_menuValuesLeftOffset, topOffset, checkboxUnchecked_width, checkboxUnchecked_height, backColor);
      _agfx.drawBitmap(_menuValuesLeftOffset, topOffset, checkboxUnchecked_bits, checkboxUnchecked_width, checkboxUnchecked_height, foreColor);
    }
    _editValueMode = false;
  }
}

void GEM_adafruit_gfx::clearValueVisibleRange() {
  int pointerPosition = getCurrentItemTopOffset(false);
  byte cursorLeftOffset = _menuValuesLeftOffset;
  _agfx.fillRect(cursorLeftOffset - 1, pointerPosition - 1, _agfx.width() - cursorLeftOffset - 1, _menuItemHeight + 1, _menuBackgroundColor);
}

void GEM_adafruit_gfx::initEditValueCursor() {
  _editValueCursorPosition = 0;
  _editValueVirtualCursorPosition = 0;
  if (_editValueType == GEM_VAL_SELECT) {
    drawEditValueSelect();
  } else {
    char chr = _valueString[_editValueVirtualCursorPosition];
    drawEditValueDigit(chr);
  }
}

void GEM_adafruit_gfx::nextEditValueCursorPosition() {
  char chr = _valueString[_editValueVirtualCursorPosition];
  drawEditValueDigit(chr, true);
  if ((_editValueCursorPosition != _menuItemValueLength - 1) && (_editValueCursorPosition != _editValueLength - 1) && (_valueString[_editValueCursorPosition] != '\0')) {
    _editValueCursorPosition++;
  }
  if ((_editValueVirtualCursorPosition != _editValueLength - 1) && (_valueString[_editValueVirtualCursorPosition] != '\0')) {
    _editValueVirtualCursorPosition++;
    if (_editValueCursorPosition == _menuItemValueLength - 1) {
      clearValueVisibleRange();
      printMenuItemValue(_valueString, 0, _editValueVirtualCursorPosition - _editValueCursorPosition);
    }
  }
  chr = _valueString[_editValueVirtualCursorPosition];
  drawEditValueDigit(chr);
}

void GEM_adafruit_gfx::prevEditValueCursorPosition() {
  char chr = _valueString[_editValueVirtualCursorPosition];
  drawEditValueDigit(chr, true);
  if (_editValueCursorPosition != 0) {
    _editValueCursorPosition--;
  }
  if (_editValueVirtualCursorPosition != 0) {
    _editValueVirtualCursorPosition--;
    if (_editValueCursorPosition == 0) {
      clearValueVisibleRange();
      printMenuItemValue(_valueString, 0, _editValueVirtualCursorPosition);
    }
  }
  chr = _valueString[_editValueVirtualCursorPosition];
  drawEditValueDigit(chr);
}

void GEM_adafruit_gfx::drawEditValueCursor(boolean clear) {
  int pointerPosition = getCurrentItemTopOffset(false);
  byte cursorLeftOffset = _menuValuesLeftOffset + _editValueCursorPosition * _menuItemFont[_menuItemFontSize].width;
  if (_editValueType == GEM_VAL_SELECT) {
    _agfx.fillRect(cursorLeftOffset - 1, pointerPosition - 1, _agfx.width() - cursorLeftOffset - 1, _menuItemHeight + 1, clear ? _menuBackgroundColor : _menuForegroundColor);
  } else {
    _agfx.fillRect(cursorLeftOffset - 1, pointerPosition - 1, _menuItemFont[_menuItemFontSize].width + 1, _menuItemHeight + 1, clear ? _menuBackgroundColor : _menuForegroundColor);
    byte yText = pointerPosition + getMenuItemInsetOffset() + _menuItemFont[_menuItemFontSize].baselineOffset;
    _agfx.setCursor(_menuValuesLeftOffset, yText);
  }
}

void GEM_adafruit_gfx::nextEditValueDigit() {
  char chr = _valueString[_editValueVirtualCursorPosition];
  byte code = (byte)chr;
  if (_editValueType == GEM_VAL_CHAR) {
    switch (code) {
      case 0:
        code = GEM_CHAR_CODE_SPACE;
        break;
      case GEM_CHAR_CODE_TILDA:
        code = GEM_CHAR_CODE_SPACE;
        break;
      case GEM_CHAR_CODE_LINE - 1:
        code = GEM_CHAR_CODE_LINE + 1;
        break;
      default:
        code++;
        break;
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

void GEM_adafruit_gfx::prevEditValueDigit() {
  char chr = _valueString[_editValueVirtualCursorPosition];
  byte code = (byte)chr;
  if (_editValueType == GEM_VAL_CHAR) {
    switch (code) {
      case 0:
        code = GEM_CHAR_CODE_TILDA;
        break;
      case GEM_CHAR_CODE_SPACE:
        code = GEM_CHAR_CODE_TILDA;
        break;
      case GEM_CHAR_CODE_LINE + 1:
        code = GEM_CHAR_CODE_LINE - 1;
        break;
      default:
        code--;
        break;
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

void GEM_adafruit_gfx::drawEditValueDigit(byte code, boolean clear) {
  drawEditValueCursor(clear);
  uint16_t foreColor = (clear) ? _menuForegroundColor : _menuBackgroundColor;
  uint16_t backColor = (clear) ? _menuBackgroundColor : _menuForegroundColor;
  int pointerPosition = getCurrentItemTopOffset(false);
  byte xText = _menuValuesLeftOffset + _editValueCursorPosition * _menuItemFont[_menuItemFontSize].width;
  byte yText = pointerPosition + getMenuItemInsetOffset() + _menuItemFont[_menuItemFontSize].baselineOffset;
  char chrNew = (char)code;
  if (chrNew != '\0') {
    _valueString[_editValueVirtualCursorPosition] = chrNew;
    _agfx.drawChar(xText, yText, code, foreColor, backColor, 1);
  }
}

void GEM_adafruit_gfx::nextEditValueSelect() {
  GEMItem* menuItemTmp = _menuPageCurrent->getCurrentMenuItem();
  GEMSelect* select = menuItemTmp->select;
  if (_valueSelectNum+1 < select->getLength()) {
    _valueSelectNum++;
  }
  drawEditValueSelect();
}

void GEM_adafruit_gfx::prevEditValueSelect() {
  if (_valueSelectNum > 0) {
    _valueSelectNum--;
  }
  drawEditValueSelect();
}

void GEM_adafruit_gfx::drawEditValueSelect() {
  GEMItem* menuItemTmp = _menuPageCurrent->getCurrentMenuItem();
  GEMSelect* select = menuItemTmp->select;
  drawEditValueCursor();
  _agfx.setTextColor(_menuBackgroundColor);
  
  int pointerPosition = getCurrentItemTopOffset(false);
  byte yText = pointerPosition + getMenuItemInsetOffset() + _menuItemFont[_menuItemFontSize].baselineOffset;
  _agfx.setCursor(_menuValuesLeftOffset, yText);
  
  printMenuItemValue(select->getOptionNameByIndex(_valueSelectNum));
  _agfx.drawBitmap(_agfx.width() - 7, getCurrentItemTopOffset(true, true), selectArrows_bits, selectArrows_width, selectArrows_height, _menuBackgroundColor);
  _agfx.setTextColor(_menuForegroundColor);
}

void GEM_adafruit_gfx::saveEditValue() {
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
    if (menuItemTmp->callbackWithArgs) {
      menuItemTmp->callbackActionArg(menuItemTmp->callbackData);
    } else {
      menuItemTmp->callbackAction();
    }
    exitEditValue();
  } else {
    exitEditValue(false);
  }
}

void GEM_adafruit_gfx::cancelEditValue() {
  exitEditValue(false);
}

void GEM_adafruit_gfx::exitEditValue(boolean redrawMenu) {
  memset(_valueString, '\0', GEM_STR_LEN - 1);
  _valueSelectNum = -1;
  _editValueMode = false;
  if (redrawMenu) {
    drawMenu();
  } else {
    drawMenuPointer(_menuPointerType == GEM_POINTER_DASH);
  }
}

// Trim leading/trailing whitespaces
// Author: Adam Rosenfield, https://stackoverflow.com/a/122721
char* GEM_adafruit_gfx::trimString(char* str) {
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

boolean GEM_adafruit_gfx::readyForKey() {
  if ( (context.loop == nullptr) ||
      ((context.loop != nullptr) && (context.allowExit)) ) {
    return true;
  } else {
    registerKeyPress(GEM_KEY_NONE);
    return false;
  }

}

void GEM_adafruit_gfx::registerKeyPress(byte keyCode) {
  _currentKey = keyCode;
  dispatchKeyPress();
}

void GEM_adafruit_gfx::dispatchKeyPress() {

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
          } else {
            nextEditValueDigit();
          }
          break;
        case GEM_KEY_RIGHT:
          if (_editValueType != GEM_VAL_SELECT) {
            nextEditValueCursorPosition();
          }
          break;
        case GEM_KEY_DOWN:
          if (_editValueType == GEM_VAL_SELECT) {
            nextEditValueSelect();
          } else {
            prevEditValueDigit();
          }
          break;
        case GEM_KEY_LEFT:
          if (_editValueType != GEM_VAL_SELECT) {
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
