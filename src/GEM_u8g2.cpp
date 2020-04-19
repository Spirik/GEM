/*

  !!! Work-in-progress of u8g2 support, may be highly unstable !!!

  GEM (a.k.a. Good Enough Menu) - Arduino library for creation of graphic multi-level menu with
  editable menu items, such as variables (supports int, byte, boolean, char[17] data types) and
  option selects. User-defined callback function can be specified to invoke when menu item is saved.
  
  Supports buttons that can invoke user-defined actions and create action-specific
  context, which can have its own enter (setup) and exit callbacks as well as loop function.

  Requires U8G2 library by olikraus (https://github.com/olikraus/U8g2_Arduino).

  For documentation visit:
  https://github.com/Spirik/GEM
  
  Copyright (c) 2018-2020 Alexander 'Spirik' Spiridonov

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

// Macro constant (alias) for current version of GEM library, printed on _splash screen
#define GEM_VER "1.1"

// Macro constants (aliases) for IDs of sprites of UI elements used to draw menu
// #define GEM_SPR_SELECT_ARROWS 0
// #define GEM_SPR_ARROW_RIGHT 1
// #define GEM_SPR_ARROW_LEFT 2
// #define GEM_SPR_ARROW_BTN 3
// #define GEM_SPR_CHECKBOX_UNCHECKED 4
// #define GEM_SPR_CHECKBOX_CHECKED 5

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
/* #define logo_width  12
#define logo_height 12
static const unsigned char logo_bits [] U8X8_PROGMEM = {
  0xc0,0xf7,0x60,0xfe,0x30,0xfe,0x18,0xff,0x8c,0xff,0xc6,0xff,
  0xe3,0xff,0xf1,0xff,0x19,0xff,0x7f,0xfc,0xff,0xfd,0xfe,0xf7
}; */

// Sprite of the default GEM _splash screen (GEM logo v2)
#define logo_width  20
#define logo_height 8
static const unsigned char logo_bits [] U8X8_PROGMEM = {
  0x8f,0x4f,0xf4,0x00,0x40,0xf4,0x00,0x40,0xf5,0x98,0x47,0xf5,
  0x00,0x40,0xf4,0x00,0x40,0xf4,0x9f,0x4f,0xf4,0x00,0x00,0xf0
};

// Sprites of the UI elements used to draw menu
/* static const uint8_t arrowRight [] PROGMEM = {
  6, 8,
  0,0,62,28,8,0
}; */
#define arrowRight_width  6
#define arrowRight_height 8
static const unsigned char arrowRight_bits [] U8X8_PROGMEM = {
   0xc0,0xc4,0xcc,0xdc,0xcc,0xc4,0xc0,0xc0
};
/* static const uint8_t arrowLeft [] PROGMEM = {
  6, 8,
  8,28,62,0,0,0
}; */
#define arrowLeft_width  6
#define arrowLeft_height 8
static const unsigned char arrowLeft_bits [] U8X8_PROGMEM = {
   0xc0,0xc4,0xc6,0xc7,0xc6,0xc4,0xc0,0xc0
};
/* static const uint8_t arrowBtn [] PROGMEM = {
  6, 8,
  62,34,20,8,0,0
}; */
#define arrowBtn_width  6
#define arrowBtn_height 8
static const unsigned char arrowBtn_bits [] U8X8_PROGMEM = {
  0xc0,0xc3,0xc5,0xc9,0xc5,0xc3,0xc0,0xc0
};
/* static const uint8_t checkboxUnchecked [] PROGMEM = {
  7, 8,
  126,66,66,66,66,126,0
}; */
#define checkboxUnchecked_width  7
#define checkboxUnchecked_height 8
static const unsigned char checkboxUnchecked_bits [] U8X8_PROGMEM = {
   0x80,0xbf,0xa1,0xa1,0xa1,0xa1,0xbf,0x80
};
/* static const uint8_t checkboxChecked [] PROGMEM = {
  7, 8,
  126,74,82,74,68,126,1
}; */
#define checkboxChecked_width  7
#define checkboxChecked_height 8
static const unsigned char checkboxChecked_bits [] U8X8_PROGMEM = {
   0xc0,0xaf,0xb1,0xab,0xa5,0xa1,0xbf,0x80
};
/* static const uint8_t selectArrows [] PROGMEM = {
  6, 8,
  0,20,54,20,0,0
}; */
#define selectArrows_width  6
#define selectArrows_height 8
static const unsigned char selectArrows_bits [] U8X8_PROGMEM = {
   0xc0,0xc4,0xce,0xc0,0xce,0xc4,0xc0,0xc0
};

GEM_u8g2::GEM_u8g2(U8G2& u8g2_, byte menuPointerType_, byte menuItemsPerScreen_, byte menuItemHeight_, byte menuPageScreenTopOffset_, byte menuValuesLeftOffset_)
  : _u8g2(u8g2_)
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

void GEM_u8g2::setSplash(byte width, byte height, const unsigned char U8X8_PROGMEM *image) {
  _splash = {width, height, image};
}

void GEM_u8g2::hideVersion(boolean flag) {
  _enableVersion = !flag;
}

void GEM_u8g2::enableCyrillic(boolean flag) {
  if (flag) {
    _fontFamilies = {(uint8_t *)GEM_FONT_BIG_CYR, (uint8_t *)GEM_FONT_SMALL_CYR};
  } else {
    _fontFamilies = {(uint8_t *)GEM_FONT_BIG, (uint8_t *)GEM_FONT_SMALL};
  }
}

void GEM_u8g2::init() {
  /*
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
  */
  _u8g2.clear();
  _u8g2.setFontPosTop();
  _u8g2.enableUTF8Print();
  
  _menuItemTitleLength = (_menuValuesLeftOffset - 5) / _menuItemFont[_menuItemFontSize].width;
  _menuItemValueLength = (_u8g2.getDisplayWidth() - _menuValuesLeftOffset - 6) / _menuItemFont[_menuItemFontSize].width;

  _u8g2.firstPage();
  do {
    _u8g2.drawXBMP((_u8g2.getDisplayWidth() - _splash.width) / 2, (_u8g2.getDisplayHeight() - _splash.height) / 2, _splash.width, _splash.height, _splash.image);
  } while (_u8g2.nextPage());

  if (_enableVersion) {
    delay(500);
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
    delay(500);
  } else {
    delay(1000);
  }

  _u8g2.clear();
}

void GEM_u8g2::setMenuPageCurrent(GEMPage& menuPageCurrent) {
  _menuPageCurrent = &menuPageCurrent;
}

//====================== CONTEXT OPERATIONS

void GEM_u8g2::clearContext() {
  context.loop = nullptr;
  context.enter = nullptr;
  context.exit = nullptr;
  context.allowExit = true;
}

//====================== DRAW OPERATIONS

void GEM_u8g2::drawMenu() {
  // _u8g2.clear(); // Not clearing for better performance
  _u8g2.firstPage();
  do {
    drawTitleBar();
    printMenuItems();
    drawMenuPointer();
    drawScrollbar();
  } while (_u8g2.nextPage());
}

void GEM_u8g2::drawTitleBar() {
 _u8g2.setFont(_fontFamilies.small);
 _u8g2.setCursor(5, 0);
 _u8g2.print(_menuPageCurrent->title);
 _u8g2.setFont(_menuItemFontSize ? _fontFamilies.small : _fontFamilies.big);
}

void GEM_u8g2::printMenuItemString(char* str, byte num, byte startPos) {
  byte i = startPos;
  while (i < num + startPos && str[i] != '\0') {
    _u8g2.print(str[i]);
    i++;
  }
}

void GEM_u8g2::printMenuItemTitle(char* str, int offset) {
  printMenuItemString(str, _menuItemTitleLength + offset);
}

void GEM_u8g2::printMenuItemValue(char* str, int offset, byte startPos) {
  printMenuItemString(str, _menuItemValueLength + offset, startPos);
}

void GEM_u8g2::printMenuItemFull(char* str, int offset) {
  printMenuItemString(str, _menuItemTitleLength + _menuItemValueLength + offset);
}

byte GEM_u8g2::getMenuItemInsetOffset(boolean forSprite) {
  // return _menuItemInsetOffset + (forSprite ? (_menuItemFontSize ? -1 : 0) : 0 ); // With additional offset for 6x8 sprites to compensate for smaller font size
  // return _menuItemInsetOffset + (forSprite ? (_menuItemFontSize ? 0 : 1) : 0 ); // With additional offset for 6x8 sprites to compensate for smaller font size
  return _menuItemInsetOffset + (forSprite ? (_menuItemFontSize ? -1 : 0) : -1 ); // With additional offset for 6x8 sprites to compensate for smaller font size
}

byte GEM_u8g2::getCurrentItemTopOffset(boolean withInsetOffset, boolean forSprite) {
  return (_menuPageCurrent->currentItemNum % _menuItemsPerScreen) * _menuItemHeight + _menuPageScreenTopOffset + (withInsetOffset ? getMenuItemInsetOffset(forSprite) : 0);
}

void GEM_u8g2::printMenuItems() {
  byte currentPageScreenNum = _menuPageCurrent->currentItemNum / _menuItemsPerScreen;
  GEMItem* menuItemTmp = (_menuPageCurrent)->getMenuItem(currentPageScreenNum * _menuItemsPerScreen);
  byte y = _menuPageScreenTopOffset;
  byte i = 0;
  while (menuItemTmp != 0 && i < _menuItemsPerScreen) {
    byte yText = y + getMenuItemInsetOffset();
    byte yDraw = y + getMenuItemInsetOffset(true);
    switch (menuItemTmp->type) {
      case GEM_ITEM_VAL:
        _u8g2.setCursor(5, yText);
        if (menuItemTmp->readonly) {
          printMenuItemTitle(menuItemTmp->title, -1);
          _u8g2.print("^");
        } else {
          printMenuItemTitle(menuItemTmp->title);
        }
        _u8g2.setCursor(_menuValuesLeftOffset, yText);
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
              _u8g2.drawXBMP(_menuValuesLeftOffset, yDraw, checkboxChecked_width, checkboxChecked_height, checkboxChecked_bits);
            } else {
              _u8g2.drawXBMP(_menuValuesLeftOffset, yDraw, checkboxUnchecked_width, checkboxUnchecked_height, checkboxUnchecked_bits);
            }
            break;
          case GEM_VAL_SELECT:
            GEMSelect* select = menuItemTmp->select;
            printMenuItemValue(select->getSelectedOptionName(menuItemTmp->linkedVariable));
            _u8g2.drawXBMP(_u8g2.getDisplayWidth() - 7, yDraw, selectArrows_width, selectArrows_height, selectArrows_bits);
            break;
        }
        //!!! - Added this
        /* if (_editValueMode) {
          drawEditValueCursor();
        } */
        break;
      case GEM_ITEM_LINK:
        _u8g2.setCursor(5, yText);
        printMenuItemFull(menuItemTmp->title);
        _u8g2.drawXBMP(_u8g2.getDisplayWidth() - 8, yDraw, arrowRight_width, arrowRight_height, arrowRight_bits);
        break;
      case GEM_ITEM_BACK:
        _u8g2.setCursor(11, yText);
        _u8g2.drawXBMP(5, yDraw, arrowLeft_width, arrowLeft_height, arrowLeft_bits);
        break;
      case GEM_ITEM_BUTTON:
        _u8g2.setCursor(11, yText);
        printMenuItemFull(menuItemTmp->title);
        _u8g2.drawXBMP(5, yDraw, arrowBtn_width, arrowBtn_height, arrowBtn_bits);
        break;
    }
    menuItemTmp = menuItemTmp->menuItemNext;    
    y += _menuItemHeight;
    i++;
  }
  memset(_valueString, '\0', GEM_STR_LEN - 1);
}

void GEM_u8g2::drawMenuPointer() {
  if (_menuPageCurrent->itemsCount > 0) {
    int pointerPosition = getCurrentItemTopOffset(false);
    if (_menuPointerType == GEM_POINTER_DASH) {
      _u8g2.drawBox(0, pointerPosition, 2, _menuItemHeight - 1);
    } else {
      _u8g2.setDrawColor(2);
      _u8g2.drawBox(0, pointerPosition - 1, _u8g2.getDisplayWidth() - 2, _menuItemHeight + 1);
      _u8g2.setDrawColor(1);
    }
  }
}

void GEM_u8g2::drawScrollbar() {
  byte screensCount = (_menuPageCurrent->itemsCount % _menuItemsPerScreen == 0) ? _menuPageCurrent->itemsCount / _menuItemsPerScreen : _menuPageCurrent->itemsCount / _menuItemsPerScreen + 1;
  if (screensCount > 1) {
    byte currentScreenNum = _menuPageCurrent->currentItemNum / _menuItemsPerScreen;
    byte scrollbarHeight = (_u8g2.getDisplayHeight() - _menuPageScreenTopOffset) / screensCount;
    byte scrollbarPosition = currentScreenNum * scrollbarHeight + _menuPageScreenTopOffset;
    _u8g2.drawLine(_u8g2.getDisplayWidth() - 1, scrollbarPosition, _u8g2.getDisplayWidth() - 1, scrollbarPosition + scrollbarHeight-1);
  }
}

//====================== MENU ITEMS NAVIGATION

void GEM_u8g2::nextMenuItem() {
  /* if (_menuPointerType != GEM_POINTER_DASH) {
    drawMenuPointer();
  } */
  if (_menuPageCurrent->currentItemNum == _menuPageCurrent->itemsCount-1) {
    _menuPageCurrent->currentItemNum = 0;
  } else {
    _menuPageCurrent->currentItemNum++;
  }
  /*
  boolean redrawMenu = (_menuPageCurrent->itemsCount > 1 && _menuPageCurrent->currentItemNum % _menuItemsPerScreen == 0);
  if (redrawMenu) {
    drawMenu();
  } else {
    drawMenuPointer();
  }
  */
  drawMenu(); //!!! Always redraw becuase of how u8g2 works
}

void GEM_u8g2::prevMenuItem() {
  /* if (_menuPointerType != GEM_POINTER_DASH) {
    drawMenuPointer();
  } */
  // boolean redrawMenu = (_menuPageCurrent->itemsCount > 1 && _menuPageCurrent->currentItemNum % _menuItemsPerScreen == 0);
  if (_menuPageCurrent->currentItemNum == 0) {
    _menuPageCurrent->currentItemNum = _menuPageCurrent->itemsCount-1;
  } else {
    _menuPageCurrent->currentItemNum--;
  }
  /* if (redrawMenu) {
    drawMenu();
  } else {
    drawMenuPointer();
  } */
  drawMenu(); //!!! Always redraw becuase of how u8g2 works
}

void GEM_u8g2::menuItemSelect() {
  GEMItem* menuItemTmp = _menuPageCurrent->getCurrentMenuItem();
  switch (menuItemTmp->type) {
    case GEM_ITEM_VAL:
      if (!menuItemTmp->readonly) {
        enterEditValueMode();
        drawMenu(); //!!! Added this
      }
      break;
    case GEM_ITEM_LINK:
      _menuPageCurrent = menuItemTmp->linkedPage;
      drawMenu();
      break;
    case GEM_ITEM_BACK:
      _menuPageCurrent->currentItemNum = (_menuPageCurrent->itemsCount > 1) ? 1 : 0;
      _menuPageCurrent = menuItemTmp->linkedPage;
      drawMenu();
      break;
    case GEM_ITEM_BUTTON:
      menuItemTmp->buttonAction();
      break;
  }
}

//====================== VALUE EDIT

void GEM_u8g2::enterEditValueMode() {
  _editValueMode = true;
  
  GEMItem* menuItemTmp = _menuPageCurrent->getCurrentMenuItem();
  /* if (_menuPointerType != GEM_POINTER_DASH) {
    drawMenuPointer();
  } */
  _editValueType = menuItemTmp->linkedType;
  switch (_editValueType) {
    case GEM_VAL_INTEGER:
      itoa(*(int*)menuItemTmp->linkedVariable, _valueString, 10);
      _editValueLength = 6;
      initEditValueCursor();
      drawMenu(); //!!!
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
      GEMSelect* select = menuItemTmp->select;
      _valueSelectNum = select->getSelectedOptionNum(menuItemTmp->linkedVariable);
      initEditValueCursor();
      break;
  }
}

void GEM_u8g2::checkboxToggle() {
  GEMItem* menuItemTmp = _menuPageCurrent->getCurrentMenuItem();
  int topOffset = getCurrentItemTopOffset(true, true);
  boolean checkboxValue = *(boolean*)menuItemTmp->linkedVariable;
  *(boolean*)menuItemTmp->linkedVariable = !checkboxValue;
  if (menuItemTmp->saveAction != nullptr) {
    menuItemTmp->saveAction();
    exitEditValue();
  } else {
    /* if (!checkboxValue) {
      _u8g2.drawXBMP(_menuValuesLeftOffset, topOffset, checkboxChecked_width, checkboxChecked_height, checkboxChecked_bits);
    } else {
      _u8g2.drawXBMP(_menuValuesLeftOffset, topOffset, checkboxUnchecked_width, checkboxUnchecked_height, checkboxUnchecked_bits);
    } */
    /* if (_menuPointerType != GEM_POINTER_DASH) {
      drawMenuPointer();
    } */
    _editValueMode = false;
  }
}

void GEM_u8g2::clearValueVisibleRange() {
  int pointerPosition = getCurrentItemTopOffset(false);
  byte cursorLeftOffset = _menuValuesLeftOffset;
  /*
  _glcd.fillBox(cursorLeftOffset - 1, pointerPosition - 1, _glcd.xdim - 3, pointerPosition + _menuItemHeight - 1, 0x00);
  _glcd.setX(_menuValuesLeftOffset);
  _glcd.setY(getCurrentItemTopOffset());
  */
 _u8g2.drawBox(cursorLeftOffset - 1, pointerPosition - 1, _u8g2.getDisplayWidth() - 3, _menuItemHeight);
 _u8g2.setCursor(_menuValuesLeftOffset, getCurrentItemTopOffset());
}

void GEM_u8g2::initEditValueCursor() {
  _editValueCursorPosition = 0;
  _editValueVirtualCursorPosition = 0;
  drawEditValueCursor();
}

void GEM_u8g2::nextEditValueCursorPosition() {
  drawEditValueCursor();
  if ((_editValueCursorPosition != _menuItemValueLength - 1) && (_editValueCursorPosition != _editValueLength - 1) && (_valueString[_editValueCursorPosition] != '\0')) {
    _editValueCursorPosition++;
  }
  if ((_editValueVirtualCursorPosition != _editValueLength - 1) && (_valueString[_editValueVirtualCursorPosition] != '\0')) {
    _editValueVirtualCursorPosition++;
    if (_editValueCursorPosition == _menuItemValueLength - 1) {
      clearValueVisibleRange();
      printMenuItemValue(_valueString, 0, _editValueVirtualCursorPosition - _menuItemValueLength + 1);
    }
  }
  drawEditValueCursor();
}

void GEM_u8g2::prevEditValueCursorPosition() {
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

void GEM_u8g2::drawEditValueCursor() {
  int pointerPosition = getCurrentItemTopOffset(false);
  byte cursorLeftOffset = _menuValuesLeftOffset + _editValueCursorPosition * _menuItemFont[_menuItemFontSize].width;
  // _glcd.drawMode(GLCD_MODE_XOR);
  _u8g2.setDrawColor(2);
  if (_editValueType == GEM_VAL_SELECT) {
    // _glcd.fillBox(cursorLeftOffset - 1, pointerPosition - 1, _glcd.xdim - 3, pointerPosition + _menuItemHeight - 1);
  } else {
    // _glcd.fillBox(cursorLeftOffset - 1, pointerPosition - 1, cursorLeftOffset + _menuItemFont[_menuItemFontSize].width - 1, pointerPosition + _menuItemHeight - 1);
    _u8g2.drawBox(cursorLeftOffset - 1, pointerPosition - 1, _menuItemFont[_menuItemFontSize].width, _menuItemHeight);
  }
  // _glcd.drawMode(GLCD_MODE_NORMAL);
  _u8g2.setDrawColor(1);
}

void GEM_u8g2::nextEditValueDigit() {
  char chr = _valueString[_editValueVirtualCursorPosition];
  byte code = (byte)chr;
  //todo: Update for compatibility with u8g2 fonts (and Cyrillic fonts in perticular)
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
        code = (_editValueCursorPosition == 0 && _editValueType == GEM_VAL_INTEGER) ? GEM_CHAR_CODE_MINUS : GEM_CHAR_CODE_SPACE;
        break;
      case GEM_CHAR_CODE_MINUS:
        code = GEM_CHAR_CODE_SPACE;
        break;
      case GEM_CHAR_CODE_SPACE:
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
        code = (_editValueCursorPosition == 0 && _editValueType == GEM_VAL_INTEGER) ? GEM_CHAR_CODE_MINUS : GEM_CHAR_CODE_9;
        break;
      case GEM_CHAR_CODE_MINUS:
        code = GEM_CHAR_CODE_9;
        break;
      case GEM_CHAR_CODE_0:
        code = GEM_CHAR_CODE_SPACE;
        break;
      case GEM_CHAR_CODE_SPACE:
        code = (_editValueCursorPosition == 0 && _editValueType == GEM_VAL_INTEGER) ? GEM_CHAR_CODE_MINUS : GEM_CHAR_CODE_9;
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
  drawEditValueCursor();
  int pointerPosition = getCurrentItemTopOffset();
  /*
  _glcd.setX(_menuValuesLeftOffset + _editValueCursorPosition * _menuItemFont[_menuItemFontSize].width);
  _glcd.setY(pointerPosition);
  _glcd.put(code);
  */
  // _u8g2.setCursor(_menuValuesLeftOffset + _editValueCursorPosition * _menuItemFont[_menuItemFontSize].width, pointerPosition);
  _u8g2.drawGlyph(_menuValuesLeftOffset + _editValueCursorPosition * _menuItemFont[_menuItemFontSize].width, pointerPosition, code);
  drawEditValueCursor();
}

void GEM_u8g2::nextEditValueSelect() {
  GEMItem* menuItemTmp = _menuPageCurrent->getCurrentMenuItem();
  GEMSelect* select = menuItemTmp->select;
  if (_valueSelectNum+1 < select->getLength()) {
    _valueSelectNum++;
  }
  drawEditValueSelect();
}

void GEM_u8g2::prevEditValueSelect() {
  GEMItem* menuItemTmp = _menuPageCurrent->getCurrentMenuItem();
  GEMSelect* select = menuItemTmp->select;
  if (_valueSelectNum > 0) {
    _valueSelectNum--;
  }
  drawEditValueSelect();
}

void GEM_u8g2::drawEditValueSelect() {
  GEMItem* menuItemTmp = _menuPageCurrent->getCurrentMenuItem();
  GEMSelect* select = menuItemTmp->select;
  clearValueVisibleRange();
  printMenuItemValue(select->getOptionNameByIndex(_valueSelectNum));
  // _glcd.drawSprite(_glcd.xdim - 7, getCurrentItemTopOffset(true, true), GEM_SPR_SELECT_ARROWS, GLCD_MODE_NORMAL);
  _u8g2.drawXBMP(_u8g2.getDisplayWidth() - 7, getCurrentItemTopOffset(true, true), selectArrows_width, selectArrows_height, selectArrows_bits);
  drawEditValueCursor();
}

void GEM_u8g2::saveEditValue() {
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
      GEMSelect* select = menuItemTmp->select;
      select->setValue(menuItemTmp->linkedVariable, _valueSelectNum);
      break;
  }
  if (menuItemTmp->saveAction != nullptr) {
    menuItemTmp->saveAction();
  }
  exitEditValue();
}

void GEM_u8g2::cancelEditValue() {
  exitEditValue();
}

void GEM_u8g2::exitEditValue() {
  memset(_valueString, '\0', GEM_STR_LEN - 1);
  _valueSelectNum = -1;
  _editValueMode = false;
  drawEditValueCursor();
  drawMenu();
}

void GEM_u8g2::editValueLoop() {
  delay(1000);
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

boolean GEM_u8g2::readyForKey() {
  if ( (context.loop == nullptr) ||
      ((context.loop != nullptr) && (context.allowExit)) ) {
    return true;
  } else {
    registerKeyPress(GEM_KEY_NONE);
    return false;
  }

}

void GEM_u8g2::registerKeyPress(byte keyCode) {
  _currentKey = keyCode;
  dispatchKeyPress();
}

void GEM_u8g2::dispatchKeyPress() {

  if (context.loop != nullptr) {
    if ((context.allowExit) && (_currentKey == GEM_KEY_CANCEL)) {
      if (context.exit != nullptr) {
        context.exit();
      } else {
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
          }
          break;
        case GEM_KEY_OK:
          menuItemSelect();
          break;
      }
    }

  }
}
