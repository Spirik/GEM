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
  
  Copyright (c) 2018-2021 Alexander 'Spirik' Spiridonov

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
#include "GEM.h"

#ifdef GEM_ENABLE_GLCD_VERSION

// AVR-based Arduinos have suppoort for dtostrf, some others may require manual inclusion (e.g. SAMD),
// see https://github.com/plotly/arduino-api/issues/38#issuecomment-108987647
#if defined(GEM_SUPPORT_FLOAT_EDIT) && (defined(ARDUINO_ARCH_SAMD) || defined(ARDUINO_ARCH_SAM))
#include <avr/dtostrf.h>
#endif

// Macro constants (aliases) for IDs of sprites of UI elements used to draw menu
#define GEM_SPR_SELECT_ARROWS 0
#define GEM_SPR_ARROW_RIGHT 1
#define GEM_SPR_ARROW_LEFT 2
#define GEM_SPR_ARROW_BTN 3
#define GEM_SPR_CHECKBOX_UNCHECKED 4
#define GEM_SPR_CHECKBOX_CHECKED 5

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
static const uint8_t logo [] PROGMEM = {
  15, 15,
  0,0,0,128,192,96,48,24,140,196,228,252,252,248,0,0,
  0,31,57,56,60,62,59,59,51,55,39,63,31,0,0,0
};
*/

// Sprite of the default GEM _splash screen (GEM logo v2)
static const uint8_t logo [] PROGMEM = {
  20, 8,
  0, 65,65,65,73,72,0, 0, 73,73,73,73,65,0, 0, 127,0,12,0,127,0
};

// Sprites of the UI elements used to draw menu
static const uint8_t arrowRight [] PROGMEM = {
  6, 8,
  0,0,62,28,8,0
};
static const uint8_t arrowLeft [] PROGMEM = {
  6, 8,
  8,28,62,0,0,0
};
static const uint8_t arrowBtn [] PROGMEM = {
  6, 8,
  62,34,20,8,0,0
};
static const uint8_t checkboxUnchecked [] PROGMEM = {
  7, 8,
  126,66,66,66,66,126,0
};
static const uint8_t checkboxChecked [] PROGMEM = {
  7, 8,
  126,74,82,74,68,126,1
};
static const uint8_t selectArrows [] PROGMEM = {
  6, 8,
  0,20,54,20,0,0
};

GEM::GEM(GLCD& glcd_, byte menuPointerType_, byte menuItemsPerScreen_, byte menuItemHeight_, byte menuPageScreenTopOffset_, byte menuValuesLeftOffset_)
  : _glcd(glcd_)
  , _menuPointerType(menuPointerType_)
  , _menuItemsPerScreen(menuItemsPerScreen_)
  , _menuItemHeight(menuItemHeight_)
  , _menuPageScreenTopOffset(menuPageScreenTopOffset_)
  , _menuValuesLeftOffset(menuValuesLeftOffset_)
{
  _menuItemFontSize = _menuItemHeight >= 8 ? 0 : 1;
  _menuItemInsetOffset = (_menuItemHeight - _menuItemFont[_menuItemFontSize].height) / 2;
  _splash = logo;
  clearContext();
  _editValueMode = false;
  _editValueCursorPosition = 0;
  memset(_valueString, '\0', GEM_STR_LEN - 1);
  _valueSelectNum = -1;
}

//====================== INIT OPERATIONS

void GEM::setSplash(const uint8_t *sprite) {
  _splash = sprite;
}

void GEM::setSplashDelay(uint16_t value) {
  _splashDelay = value;
}

void GEM::hideVersion(boolean flag) {
  _enableVersion = !flag;
}

void GEM::init() {
  _glcd.loadSprite_P(GEM_SPR_ARROW_RIGHT, arrowRight);
  _glcd.loadSprite_P(GEM_SPR_ARROW_LEFT, arrowLeft);
  _glcd.loadSprite_P(GEM_SPR_ARROW_BTN, arrowBtn);
  _glcd.loadSprite_P(GEM_SPR_CHECKBOX_UNCHECKED, checkboxUnchecked);
  _glcd.loadSprite_P(GEM_SPR_CHECKBOX_CHECKED, checkboxChecked);
  _glcd.loadSprite_P(GEM_SPR_SELECT_ARROWS, selectArrows);
  
  _glcd.drawMode(GLCD_MODE_NORMAL);
  _glcd.fontMode(GLCD_MODE_NORMAL);
  _glcd.set(GLCD_ID_CRLF, 0);
  _glcd.set(GLCD_ID_SCROLL, 0);
  _glcd.clearScreen();
  
  _menuItemTitleLength = (_menuValuesLeftOffset - 5) / _menuItemFont[_menuItemFontSize].width;
  _menuItemValueLength = (_glcd.xdim - _menuValuesLeftOffset - 6) / _menuItemFont[_menuItemFontSize].width;
  if (_menuItemsPerScreen == GEM_ITEMS_COUNT_AUTO) {
    _menuItemsPerScreen = (_glcd.ydim - _menuPageScreenTopOffset) / _menuItemHeight;
  }
  
  if (_splashDelay > 0) {

    _glcd.bitblt_P(_glcd.xdim/2-(pgm_read_byte(_splash)+1)/2, _glcd.ydim/2-(pgm_read_byte(_splash+1)+1)/2, GLCD_MODE_NORMAL, _splash);

    if (_enableVersion) {
      delay(_splashDelay / 2);
      _glcd.fontFace(1);
      _glcd.setY(_glcd.ydim - 6);
      if (_splash != logo) {
        _glcd.setX(_glcd.xdim - strlen(GEM_VER)*4 - 12);
        _glcd.putstr((char*)"GEM");
      } else {
        _glcd.setX(_glcd.xdim - strlen(GEM_VER)*4);
      }
      _glcd.putstr((char*)GEM_VER);
      delay(_splashDelay / 2);
    } else {
      delay(_splashDelay);
    }

  }
}

void GEM::reInit() {
  _glcd.drawMode(GLCD_MODE_NORMAL);
  _glcd.fontMode(GLCD_MODE_NORMAL);
  _glcd.set(GLCD_ID_CRLF, 0);
  _glcd.set(GLCD_ID_SCROLL, 0);
  _glcd.clearScreen();
}

void GEM::setMenuPageCurrent(GEMPage& menuPageCurrent) {
  _menuPageCurrent = &menuPageCurrent;
}

//====================== CONTEXT OPERATIONS

void GEM::clearContext() {
  context.loop = nullptr;
  context.enter = nullptr;
  context.exit = nullptr;
  context.allowExit = true;
}

//====================== DRAW OPERATIONS

void GEM::drawMenu() {
  _glcd.clearScreen();
  drawTitleBar();
  printMenuItems();
  drawMenuPointer();
  drawScrollbar();
}

void GEM::drawTitleBar() {
  _glcd.fontFace(1);
  _glcd.setXY(5,1);
  _glcd.putstr((char*)_menuPageCurrent->title);
  _glcd.fontFace(_menuItemFontSize);
}

void GEM::printMenuItemString(const char* str, byte num, byte startPos) {
  byte i = startPos;
  while (i < num + startPos && str[i] != '\0') {
    _glcd.put(str[i]);
    i++;
  }
}

void GEM::printMenuItemTitle(const char* str, int offset) {
  printMenuItemString(str, _menuItemTitleLength + offset);
}

void GEM::printMenuItemValue(const char* str, int offset, byte startPos) {
  printMenuItemString(str, _menuItemValueLength + offset, startPos);
}

void GEM::printMenuItemFull(const char* str, int offset) {
  printMenuItemString(str, _menuItemTitleLength + _menuItemValueLength + offset);
}

byte GEM::getMenuItemInsetOffset(boolean forSprite) {
  return _menuItemInsetOffset + (forSprite ? (_menuItemFontSize ? -1 : 0) : 0 ); // With additional offset for 6x8 sprites to compensate for smaller font size
}

byte GEM::getCurrentItemTopOffset(boolean withInsetOffset, boolean forSprite) {
  return (_menuPageCurrent->currentItemNum % _menuItemsPerScreen) * _menuItemHeight + _menuPageScreenTopOffset + (withInsetOffset ? getMenuItemInsetOffset(forSprite) : 0);
}

void GEM::printMenuItems() {
  byte currentPageScreenNum = _menuPageCurrent->currentItemNum / _menuItemsPerScreen;
  GEMItem* menuItemTmp = _menuPageCurrent->getMenuItem(currentPageScreenNum * _menuItemsPerScreen);
  byte y = _menuPageScreenTopOffset;
  byte i = 0;
  while (menuItemTmp != nullptr && i < _menuItemsPerScreen) {
    _glcd.setY(y + getMenuItemInsetOffset());
    byte yDraw = y + getMenuItemInsetOffset(true);
    switch (menuItemTmp->type) {
      case GEM_ITEM_VAL:
        _glcd.setX(5);
        if (menuItemTmp->readonly) {
          printMenuItemTitle(menuItemTmp->title, -1);
          _glcd.putstr((char*)"^");
        } else {
          printMenuItemTitle(menuItemTmp->title);
        }
        _glcd.setX(_menuValuesLeftOffset);
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
              _glcd.drawSprite(_menuValuesLeftOffset, yDraw, GEM_SPR_CHECKBOX_CHECKED, GLCD_MODE_NORMAL);
            } else {
              _glcd.drawSprite(_menuValuesLeftOffset, yDraw, GEM_SPR_CHECKBOX_UNCHECKED, GLCD_MODE_NORMAL);
            }
            break;
          case GEM_VAL_SELECT:
            {
              GEMSelect* select = menuItemTmp->select;
              printMenuItemValue(select->getSelectedOptionName(menuItemTmp->linkedVariable));
              _glcd.drawSprite(_glcd.xdim-7, yDraw, GEM_SPR_SELECT_ARROWS, GLCD_MODE_NORMAL);
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
        _glcd.setX(5);
        if (menuItemTmp->readonly) {
          printMenuItemFull(menuItemTmp->title, -1);
          _glcd.putstr((char*)"^");
        } else {
          printMenuItemFull(menuItemTmp->title);
        }
        _glcd.drawSprite(_glcd.xdim-8, yDraw, GEM_SPR_ARROW_RIGHT, GLCD_MODE_NORMAL);
        break;
      case GEM_ITEM_BACK:
        _glcd.setX(11);
        _glcd.drawSprite(5, yDraw, GEM_SPR_ARROW_LEFT, GLCD_MODE_NORMAL);
        break;
      case GEM_ITEM_BUTTON:
        _glcd.setX(11);
        if (menuItemTmp->readonly) {
          printMenuItemFull(menuItemTmp->title, -1);
          _glcd.putstr((char*)"^");
        } else {
          printMenuItemFull(menuItemTmp->title);
        }
        _glcd.drawSprite(5, yDraw, GEM_SPR_ARROW_BTN, GLCD_MODE_NORMAL);
        break;
    }
    menuItemTmp = menuItemTmp->getMenuItemNext();
    y += _menuItemHeight;
    i++;
  }
  memset(_valueString, '\0', GEM_STR_LEN - 1);
}

void GEM::drawMenuPointer() {
  if (_menuPageCurrent->itemsCount > 0) {
    GEMItem* menuItemTmp = _menuPageCurrent->getCurrentMenuItem();
    int pointerPosition = getCurrentItemTopOffset(false);
    if (_menuPointerType == GEM_POINTER_DASH) {
      _glcd.eraseBox(0, _menuPageScreenTopOffset, 1, _glcd.ydim-1);
      if (menuItemTmp->readonly) {
        for (byte i = 0; i < (_menuItemHeight - 1) / 2; i++) {
          _glcd.drawPixel(0, pointerPosition + i * 2, GLCD_MODE_NORMAL);
          _glcd.drawPixel(1, pointerPosition + i * 2 + 1, GLCD_MODE_NORMAL);
        }
      } else {
        _glcd.drawBox(0, pointerPosition, 1, pointerPosition + _menuItemHeight - 2, GLCD_MODE_NORMAL);
      }
    } else {
      _glcd.drawMode(GLCD_MODE_XOR);
      _glcd.fillBox(0, pointerPosition-1, _glcd.xdim-3, pointerPosition + _menuItemHeight - 1);
      _glcd.drawMode(GLCD_MODE_NORMAL);
      if (menuItemTmp->readonly) {
        for (byte i = 0; i < (_menuItemHeight + 2) / 2; i++) {
          _glcd.drawPixel(0, pointerPosition + i * 2, GLCD_MODE_REVERSE);
          _glcd.drawPixel(1, pointerPosition + i * 2 - 1, GLCD_MODE_REVERSE);
        }
      }
    }
  }
}

void GEM::drawScrollbar() {
  byte screensCount = (_menuPageCurrent->itemsCount % _menuItemsPerScreen == 0) ? _menuPageCurrent->itemsCount / _menuItemsPerScreen : _menuPageCurrent->itemsCount / _menuItemsPerScreen + 1;
  if (screensCount > 1) {
    byte currentScreenNum = _menuPageCurrent->currentItemNum / _menuItemsPerScreen;
    byte scrollbarHeight = (_glcd.ydim - _menuPageScreenTopOffset + 1) / screensCount;
    byte scrollbarPosition = currentScreenNum * scrollbarHeight + _menuPageScreenTopOffset - 1;
    _glcd.drawLine(_glcd.xdim - 1, scrollbarPosition, _glcd.xdim - 1, scrollbarPosition + scrollbarHeight, GLCD_MODE_NORMAL);
  }
}

//====================== MENU ITEMS NAVIGATION

void GEM::nextMenuItem() {
  if (_menuPointerType != GEM_POINTER_DASH) {
    drawMenuPointer();
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

void GEM::prevMenuItem() {
  if (_menuPointerType != GEM_POINTER_DASH) {
    drawMenuPointer();
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

void GEM::menuItemSelect() {
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
        menuItemTmp->buttonAction();
      }
      break;
  }
}

//====================== VALUE EDIT

void GEM::enterEditValueMode() {
  _editValueMode = true;
  
  GEMItem* menuItemTmp = _menuPageCurrent->getCurrentMenuItem();
  if (_menuPointerType != GEM_POINTER_DASH) {
    drawMenuPointer();
  }
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

void GEM::checkboxToggle() {
  GEMItem* menuItemTmp = _menuPageCurrent->getCurrentMenuItem();
  int topOffset = getCurrentItemTopOffset(true, true);
  boolean checkboxValue = *(boolean*)menuItemTmp->linkedVariable;
  *(boolean*)menuItemTmp->linkedVariable = !checkboxValue;
  if (menuItemTmp->saveAction != nullptr) {
    menuItemTmp->saveAction();
    exitEditValue();
  } else {
    if (!checkboxValue) {
      _glcd.drawSprite(_menuValuesLeftOffset, topOffset, GEM_SPR_CHECKBOX_CHECKED, GLCD_MODE_NORMAL);
    } else {
      _glcd.drawSprite(_menuValuesLeftOffset, topOffset, GEM_SPR_CHECKBOX_UNCHECKED, GLCD_MODE_NORMAL);
    }
    if (_menuPointerType != GEM_POINTER_DASH) {
      drawMenuPointer();
    }
    _editValueMode = false;
  }
}

void GEM::clearValueVisibleRange() {
  int pointerPosition = getCurrentItemTopOffset(false);
  byte cursorLeftOffset = _menuValuesLeftOffset;
  _glcd.fillBox(cursorLeftOffset - 1, pointerPosition - 1, _glcd.xdim - 3, pointerPosition + _menuItemHeight - 1, 0x00);
  _glcd.setX(_menuValuesLeftOffset);
  _glcd.setY(getCurrentItemTopOffset());
}

void GEM::initEditValueCursor() {
  _editValueCursorPosition = 0;
  _editValueVirtualCursorPosition = 0;
  drawEditValueCursor();
}

void GEM::nextEditValueCursorPosition() {
  drawEditValueCursor();
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
  drawEditValueCursor();
}

void GEM::prevEditValueCursorPosition() {
  drawEditValueCursor();
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
  drawEditValueCursor();
}

void GEM::drawEditValueCursor() {
  int pointerPosition = getCurrentItemTopOffset(false);
  byte cursorLeftOffset = _menuValuesLeftOffset + _editValueCursorPosition * _menuItemFont[_menuItemFontSize].width;
  _glcd.drawMode(GLCD_MODE_XOR);
  if (_editValueType == GEM_VAL_SELECT) {
    _glcd.fillBox(cursorLeftOffset - 1, pointerPosition - 1, _glcd.xdim - 3, pointerPosition + _menuItemHeight - 1);
  } else {
    _glcd.fillBox(cursorLeftOffset - 1, pointerPosition - 1, cursorLeftOffset + _menuItemFont[_menuItemFontSize].width - 1, pointerPosition + _menuItemHeight - 1);
  }
  _glcd.drawMode(GLCD_MODE_NORMAL);
}

void GEM::nextEditValueDigit() {
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

void GEM::prevEditValueDigit() {
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

void GEM::drawEditValueDigit(byte code) {
  char chrNew = (char)code;
  _valueString[_editValueVirtualCursorPosition] = chrNew;
  drawEditValueCursor();
  _glcd.setX(_menuValuesLeftOffset + _editValueCursorPosition * _menuItemFont[_menuItemFontSize].width);
  int pointerPosition = getCurrentItemTopOffset();
  _glcd.setY(pointerPosition);
  _glcd.put(code);
  drawEditValueCursor();
}

void GEM::nextEditValueSelect() {
  GEMItem* menuItemTmp = _menuPageCurrent->getCurrentMenuItem();
  GEMSelect* select = menuItemTmp->select;
  if (_valueSelectNum+1 < select->getLength()) {
    _valueSelectNum++;
  }
  drawEditValueSelect();
}

void GEM::prevEditValueSelect() {
  GEMItem* menuItemTmp = _menuPageCurrent->getCurrentMenuItem();
  GEMSelect* select = menuItemTmp->select;
  if (_valueSelectNum > 0) {
    _valueSelectNum--;
  }
  drawEditValueSelect();
}

void GEM::drawEditValueSelect() {
  GEMItem* menuItemTmp = _menuPageCurrent->getCurrentMenuItem();
  GEMSelect* select = menuItemTmp->select;
  clearValueVisibleRange();
  printMenuItemValue(select->getOptionNameByIndex(_valueSelectNum));
  _glcd.drawSprite(_glcd.xdim - 7, getCurrentItemTopOffset(true, true), GEM_SPR_SELECT_ARROWS, GLCD_MODE_NORMAL);
  drawEditValueCursor();
}

void GEM::saveEditValue() {
  GEMItem* menuItemTmp = _menuPageCurrent->getCurrentMenuItem();
  void* temp;
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
  if (menuItemTmp->saveAction != nullptr) {
    menuItemTmp->saveAction();
  }
  exitEditValue();
}

void GEM::cancelEditValue() {
  exitEditValue();
}

void GEM::exitEditValue() {
  memset(_valueString, '\0', GEM_STR_LEN - 1);
  _valueSelectNum = -1;
  _editValueMode = false;
  drawEditValueCursor();
  drawMenu();
}

// Trim leading/trailing whitespaces
// Author: Adam Rosenfield, https://stackoverflow.com/a/122721
char* GEM::trimString(char* str) {
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

boolean GEM::readyForKey() {
  if ( (context.loop == nullptr) ||
      ((context.loop != nullptr) && (context.allowExit)) ) {
    return true;
  } else {
    registerKeyPress(GEM_KEY_NONE);
    return false;
  }

}

void GEM::registerKeyPress(byte keyCode) {
  _currentKey = keyCode;
  dispatchKeyPress();
}

void GEM::dispatchKeyPress() {

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
