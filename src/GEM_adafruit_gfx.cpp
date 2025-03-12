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
#define GEM_CHAR_CODE_TILDA 126
#define GEM_CHAR_CODE_BANG 33
#define GEM_CHAR_CODE_a 97
#define GEM_CHAR_CODE_ACCENT 96

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

#define logo_width_scaled  40
#define logo_height_scaled 16
static const uint8_t logo_bits_scaled [] PROGMEM = {
  0xff, 0x03, 0xff, 0x0c, 0x0c, 0xff, 0x03, 0xff, 0x0c, 0x0c, 0x00, 0x00, 0x00, 0x0c, 0x0c, 0x00, 
  0x00, 0x00, 0x0c, 0x0c, 0x00, 0x00, 0x00, 0x0c, 0xcc, 0x00, 0x00, 0x00, 0x0c, 0xcc, 0x03, 0xc3, 
  0xfc, 0x0c, 0xcc, 0x03, 0xc3, 0xfc, 0x0c, 0xcc, 0x00, 0x00, 0x00, 0x0c, 0x0c, 0x00, 0x00, 0x00, 
  0x0c, 0x0c, 0x00, 0x00, 0x00, 0x0c, 0x0c, 0x00, 0x00, 0x00, 0x0c, 0x0c, 0xff, 0xc3, 0xff, 0x0c, 
  0x0c, 0xff, 0xc3, 0xff, 0x0c, 0x0c, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

const Splash logo[] = {
  {logo_width, logo_height, logo_bits},
  {logo_width_scaled, logo_height_scaled, logo_bits_scaled}
};

// Sprites of the UI elements used to draw menu

#define sprite_height  8

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

#define sprite_height_scaled  16

#define arrowRight_width_scaled  12
#define arrowRight_height_scaled 16
static const uint8_t arrowRight_bits_scaled [] PROGMEM = {
  0x00, 0x00, 0x00, 0x00, 0x0c, 0x00, 0x0e, 0x00, 0x0f, 0x00, 0x0f, 0x80, 0x0f, 0xc0, 0x0f, 0xc0, 
  0x0f, 0x80, 0x0f, 0x00, 0x0e, 0x00, 0x0c, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
};

#define arrowLeft_width_scaled  12
#define arrowLeft_height_scaled 16
static const uint8_t arrowLeft_bits_scaled [] PROGMEM = {
  0x00, 0x00, 0x00, 0x00, 0x0c, 0x00, 0x1c, 0x00, 0x3c, 0x00, 0x7c, 0x00, 0xfc, 0x00, 0xfc, 0x00, 
  0x7c, 0x00, 0x3c, 0x00, 0x1c, 0x00, 0x0c, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
};

#define arrowBtn_width_scaled  12
#define arrowBtn_height_scaled 16
static const uint8_t arrowBtn_bits_scaled [] PROGMEM = {
  0x00, 0x00, 0x00, 0x00, 0xf0, 0x00, 0xf8, 0x00, 0xdc, 0x00, 0xce, 0x00, 0xc7, 0x00, 0xc7, 0x00, 
  0xce, 0x00, 0xdc, 0x00, 0xf8, 0x00, 0xf0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
};

#define checkboxUnchecked_width_scaled  14
#define checkboxUnchecked_height_scaled 16
static const uint8_t checkboxUnchecked_bits_scaled [] PROGMEM = {
  0x00, 0x00, 0x00, 0x00, 0xff, 0xf0, 0xff, 0xf0, 0xc0, 0x30, 0xc0, 0x30, 0xc0, 0x30, 0xc0, 0x30, 
  0xc0, 0x30, 0xc0, 0x30, 0xc0, 0x30, 0xc0, 0x30, 0xff, 0xf0, 0xff, 0xf0, 0x00, 0x00, 0x00, 0x00,
};

#define checkboxChecked_width_scaled  14
#define checkboxChecked_height_scaled 16
static const uint8_t checkboxChecked_bits_scaled [] PROGMEM = {
  0x00, 0x0c, 0x00, 0x1c, 0xff, 0x38, 0xfe, 0x70, 0xc0, 0xf0, 0xc1, 0xf0, 0xf3, 0xb0, 0xff, 0x30, 
  0xde, 0x30, 0xcc, 0x30, 0xc0, 0x30, 0xc0, 0x30, 0xff, 0xf0, 0xff, 0xf0, 0x00, 0x00, 0x00, 0x00,
};

#define selectArrows_width_scaled  12
#define selectArrows_height_scaled 16
static const uint8_t selectArrows_bits_scaled [] PROGMEM = {
  0x00, 0x00, 0x00, 0x00, 0x0c, 0x00, 0x1e, 0x00, 0x3f, 0x00, 0x3f, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x3f, 0x00, 0x3f, 0x00, 0x1e, 0x00, 0x0c, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

const Splash arrowRight[] = {
  {arrowRight_width, arrowRight_height, arrowRight_bits},
  {arrowRight_width_scaled, arrowRight_height_scaled, arrowRight_bits_scaled}
};

const Splash arrowLeft[] = {
  {arrowLeft_width, arrowLeft_height, arrowLeft_bits},
  {arrowLeft_width_scaled, arrowLeft_height_scaled, arrowLeft_bits_scaled}
};

const Splash arrowBtn[] = {
  {arrowBtn_width, arrowBtn_height, arrowBtn_bits},
  {arrowBtn_width_scaled, arrowBtn_height_scaled, arrowBtn_bits_scaled}
};

const Splash checkboxUnchecked[] = {
  {checkboxUnchecked_width, checkboxUnchecked_height, checkboxUnchecked_bits},
  {checkboxUnchecked_width_scaled, checkboxUnchecked_height_scaled, checkboxUnchecked_bits_scaled}
};

const Splash checkboxChecked[] = {
  {checkboxChecked_width, checkboxChecked_height, checkboxChecked_bits},
  {checkboxChecked_width_scaled, checkboxChecked_height_scaled, checkboxChecked_bits_scaled}
};

const Splash selectArrows[] = {
  {selectArrows_width, selectArrows_height, selectArrows_bits},
  {selectArrows_width_scaled, selectArrows_height_scaled, selectArrows_bits_scaled}
};

GEM_adafruit_gfx::GEM_adafruit_gfx(Adafruit_GFX& agfx_, byte menuPointerType_, byte menuItemsPerScreen_, byte menuItemHeight_, byte menuPageScreenTopOffset_, byte menuValuesLeftOffset_)
  : _agfx(agfx_)
{
  _appearance.menuPointerType = menuPointerType_;
  _appearance.menuItemsPerScreen = menuItemsPerScreen_;
  _appearance.menuItemHeight = menuItemHeight_;
  _appearance.menuPageScreenTopOffset = menuPageScreenTopOffset_;
  _appearance.menuValuesLeftOffset = menuValuesLeftOffset_;
  _appearanceCurrent = &_appearance;
  _splash = logo[_textSize > 1 ? 1 : 0];
  clearContext();
  _editValueMode = false;
  _editValueCursorPosition = 0;
  memset(_valueString, '\0', GEM_STR_LEN - 1);
  _valueSelectNum = -1;
}

GEM_adafruit_gfx::GEM_adafruit_gfx(Adafruit_GFX& agfx_, GEMAppearance appearance_)
  : _agfx(agfx_)
  , _appearance(appearance_)
{
  _appearanceCurrent = &_appearance;
  _splash = logo[_textSize > 1 ? 1 : 0];
  clearContext();
  _editValueMode = false;
  _editValueCursorPosition = 0;
  memset(_valueString, '\0', GEM_STR_LEN - 1);
  _valueSelectNum = -1;
}

//====================== APPEARANCE OPERATIONS

GEM_adafruit_gfx& GEM_adafruit_gfx::setAppearance(GEMAppearance appearance) {
  _appearance = appearance;
  return *this;
}

GEMAppearance* GEM_adafruit_gfx::getCurrentAppearance() {
  return (_menuPageCurrent != nullptr && _menuPageCurrent->_appearance != nullptr) ? _menuPageCurrent->_appearance : &_appearance;
}

byte GEM_adafruit_gfx::getMenuItemsPerScreen() {
  return getCurrentAppearance()->menuItemsPerScreen == GEM_ITEMS_COUNT_AUTO ? (_agfx.height() - getCurrentAppearance()->menuPageScreenTopOffset) / getCurrentAppearance()->menuItemHeight : getCurrentAppearance()->menuItemsPerScreen;
}

byte GEM_adafruit_gfx::getMenuItemFontSize() {
  return getCurrentAppearance()->menuItemHeight >= _menuItemFont[0].height * _textSize ? 0 : 1;
}

byte GEM_adafruit_gfx::getMenuItemTitleLength() {
  return (getCurrentAppearance()->menuValuesLeftOffset - 5 * _textSize) / (_menuItemFont[getMenuItemFontSize()].width * _textSize);
}

byte GEM_adafruit_gfx::getMenuItemValueLength() {
  return (_agfx.width() - getCurrentAppearance()->menuValuesLeftOffset - 6 * _textSize) / (_menuItemFont[getMenuItemFontSize()].width * _textSize);
}


//====================== INIT OPERATIONS

GEM_adafruit_gfx& GEM_adafruit_gfx::setSplash(byte width, byte height, const uint8_t *image) {
  _splash = {width, height, image};
  return *this;
}

GEM_adafruit_gfx& GEM_adafruit_gfx::setSplashDelay(uint16_t value) {
  _splashDelay = value;
  return *this;
}

GEM_adafruit_gfx& GEM_adafruit_gfx::hideVersion(bool flag) {
  _enableVersion = !flag;
  return *this;
}

GEM_adafruit_gfx& GEM_adafruit_gfx::setTextSize(uint8_t size) {
  _textSize = size > 0 ? size : 1;
  setSpriteSize(_textSize);
  return *this;
}

GEM_adafruit_gfx& GEM_adafruit_gfx::setSpriteSize(uint8_t size) {
  _spriteSize = size > 1 ? 2 : 1;
  if (_splash.image == logo[0].image || _splash.image == logo[1].image) {
    _splash = logo[_spriteSize > 1 ? 1 : 0];
  }
  return *this;
}

GEM_adafruit_gfx& GEM_adafruit_gfx::setFontBig(const GFXfont* font, uint8_t width, uint8_t height, uint8_t baselineOffset) {
  _fontFamilies.big = font;
  _menuItemFont[0] = {width, height, baselineOffset};
  return *this;
}

GEM_adafruit_gfx& GEM_adafruit_gfx::setFontSmall(const GFXfont* font, uint8_t width, uint8_t height, uint8_t baselineOffset) {
  _fontFamilies.small = font;
  _menuItemFont[1] = {width, height, baselineOffset};
  return *this;
}

GEM_adafruit_gfx& GEM_adafruit_gfx::setForegroundColor(uint16_t color) {
  _menuForegroundColor = color;
  return *this;
}

GEM_adafruit_gfx& GEM_adafruit_gfx::setBackgroundColor(uint16_t color) {
  _menuBackgroundColor = color;
  return *this;
}

GEM_adafruit_gfx& GEM_adafruit_gfx::invertKeysDuringEdit(bool invert) {
  _invertKeysDuringEdit = invert;
  return *this;
}

GEM_adafruit_gfx& GEM_adafruit_gfx::init() {
  _agfx.setTextSize(_textSize);
  _agfx.setTextWrap(false);
  _agfx.setTextColor(_menuForegroundColor);
  _agfx.fillScreen(_menuBackgroundColor);

  if (_splashDelay > 0) {

    _agfx.drawBitmap((_agfx.width() - _splash.width) / 2, (_agfx.height() - _splash.height) / 2, _splash.image, _splash.width, _splash.height, _menuForegroundColor);

    if (_enableVersion) {
      delay(_splashDelay / 2);
      _agfx.setFont(_fontFamilies.small);
      byte x = _agfx.width() - strlen(GEM_VER) * 4 * _textSize;
      byte y = _agfx.height() - 1;
      if (_splash.image != logo[0].image && _splash.image != logo[1].image) {
        _agfx.setCursor(x - 12 * _textSize, y);
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

  return *this;
}

GEM_adafruit_gfx& GEM_adafruit_gfx::reInit() {
  _agfx.setTextSize(_textSize);
  _agfx.setTextWrap(false);
  _agfx.setTextColor(_menuForegroundColor);
  _agfx.fillScreen(_menuBackgroundColor);
  return *this;
}

GEM_adafruit_gfx& GEM_adafruit_gfx::setMenuPageCurrent(GEMPage& menuPageCurrent) {
  _menuPageCurrent = &menuPageCurrent;
  return *this;
}

GEMPage* GEM_adafruit_gfx::getCurrentMenuPage() {
  return _menuPageCurrent;
}

//====================== CONTEXT OPERATIONS

GEM_adafruit_gfx& GEM_adafruit_gfx::clearContext() {
  context.loop = nullptr;
  context.enter = nullptr;
  context.exit = nullptr;
  context.allowExit = true;
  return *this;
}

//====================== DRAW OPERATIONS

GEM_adafruit_gfx& GEM_adafruit_gfx::drawMenu() {
  _agfx.fillScreen(_menuBackgroundColor);
  drawTitleBar();
  printMenuItems();
  drawMenuPointer();
  drawScrollbar();
  if (drawMenuCallback != nullptr) {
    drawMenuCallback();
  }
  return *this;
}

GEM_adafruit_gfx& GEM_adafruit_gfx::setDrawMenuCallback(void (*drawMenuCallback_)()) {
  drawMenuCallback = drawMenuCallback_;
  return *this;
}

GEM_adafruit_gfx& GEM_adafruit_gfx::removeDrawMenuCallback() {
  drawMenuCallback = nullptr;
  return *this;
}

void GEM_adafruit_gfx::drawTitleBar() {
  _agfx.setFont(_fontFamilies.small);
  _agfx.setTextWrap(true);
  _agfx.setTextColor(_menuForegroundColor);
  _agfx.setCursor(5 * _textSize, _menuItemFont[1].baselineOffset * _textSize + 1);
  _agfx.print(_menuPageCurrent->title);
  _agfx.setTextWrap(false);
  _agfx.setFont(getMenuItemFontSize() ? _fontFamilies.small : _fontFamilies.big);
}

void GEM_adafruit_gfx::drawSprite(int16_t x, int16_t y, const Splash sprite[], uint16_t color) {
  byte variant = _spriteSize - 1;
  _agfx.drawBitmap(x, y, sprite[variant].image, sprite[variant].width, sprite[variant].height, color);
}

void GEM_adafruit_gfx::printMenuItemString(const char* str, byte num, byte startPos) {
  byte i = startPos;
  while (i < num + startPos && str[i] != '\0') {
    _agfx.print(str[i]);
    i++;
  }
}

void GEM_adafruit_gfx::printMenuItemTitle(const char* str, int offset) {
  printMenuItemString(str, getMenuItemTitleLength() + offset);
}

void GEM_adafruit_gfx::printMenuItemValue(const char* str, int offset, byte startPos) {
  printMenuItemString(str, getMenuItemValueLength() + offset, startPos);
}

void GEM_adafruit_gfx::printMenuItemFull(const char* str, int offset) {
  printMenuItemString(str, getMenuItemTitleLength() + getMenuItemValueLength() + offset);
}

byte GEM_adafruit_gfx::getMenuItemInsetOffset(bool forSprite) {
  byte menuItemFontSize = getMenuItemFontSize();
  byte spriteHeight = _spriteSize > 1 ? sprite_height_scaled : sprite_height;
  byte menuItemInsetOffset = (getCurrentAppearance()->menuItemHeight - _menuItemFont[menuItemFontSize].height * _textSize) / 2;
  return menuItemInsetOffset + (forSprite ? (_menuItemFont[menuItemFontSize].height * _textSize - spriteHeight) / 2 : -1 * _textSize); // With additional offset for sprites and text for better visual alignment
}

byte GEM_adafruit_gfx::getCurrentItemTopOffset(bool withInsetOffset, bool forSprite) {
  return (_menuPageCurrent->currentItemNum % getMenuItemsPerScreen()) * getCurrentAppearance()->menuItemHeight + getCurrentAppearance()->menuPageScreenTopOffset + (withInsetOffset ? getMenuItemInsetOffset(forSprite) : 0);
}

byte GEM_adafruit_gfx::calculateSpriteOverlap(const Splash sprite[]) {
  return ((sprite[_spriteSize - 1].width - 3 * _textSize) / _menuItemFont[getMenuItemFontSize()].width * _textSize);
}

void GEM_adafruit_gfx::printMenuItem(GEMItem* menuItemTmp, byte yText, byte yDraw, uint16_t color) {
  _agfx.setTextColor(color);
  char valueStringTmp[GEM_STR_LEN];
  switch (menuItemTmp->type) {
    case GEM_ITEM_VAL:
      {
        _agfx.setCursor(5 * _textSize, yText);
        if (menuItemTmp->readonly) {
          printMenuItemTitle(menuItemTmp->title, -1);
          _agfx.print("^");
        } else {
          printMenuItemTitle(menuItemTmp->title);
        }

        byte menuValuesLeftOffset = getCurrentAppearance()->menuValuesLeftOffset;
        _agfx.setCursor(menuValuesLeftOffset, yText);
        switch (menuItemTmp->linkedType) {
          case GEM_VAL_INTEGER:
            itoa(*(int*)menuItemTmp->linkedVariable, valueStringTmp, 10);
            printMenuItemValue(valueStringTmp);
            break;
          case GEM_VAL_BYTE:
            itoa(*(byte*)menuItemTmp->linkedVariable, valueStringTmp, 10);
            printMenuItemValue(valueStringTmp);
            break;
          case GEM_VAL_CHAR:
            printMenuItemValue((char*)menuItemTmp->linkedVariable);
            break;
          case GEM_VAL_BOOL:
            if (*(bool*)menuItemTmp->linkedVariable) {
              drawSprite(menuValuesLeftOffset, yDraw, checkboxChecked, color);
            } else {
              drawSprite(menuValuesLeftOffset, yDraw, checkboxUnchecked, color);
            }
            break;
          case GEM_VAL_SELECT:
            {
              GEMSelect* select = menuItemTmp->select;
              printMenuItemValue(select->getSelectedOptionName(menuItemTmp->linkedVariable), -1 * calculateSpriteOverlap(selectArrows));
              drawSprite(_agfx.width() - 7 * _spriteSize, yDraw, selectArrows, color);
            }
            break;
          #ifdef GEM_SUPPORT_SPINNER
          case GEM_VAL_SPINNER:
            {
              GEMSpinner* spinner = menuItemTmp->spinner;
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
              printMenuItemValue(valueStringTmp, -1 * calculateSpriteOverlap(selectArrows));
              drawSprite(_agfx.width() - 7 * _spriteSize, yDraw, selectArrows, color);
            }
            break;
          #endif
          #ifdef GEM_SUPPORT_FLOAT_EDIT
          case GEM_VAL_FLOAT:
            // sprintf(valueStringTmp,"%.6f", *(float*)menuItemTmp->linkedVariable); // May work for non-AVR boards
            dtostrf(*(float*)menuItemTmp->linkedVariable, menuItemTmp->precision + 1, menuItemTmp->precision, valueStringTmp);
            printMenuItemValue(valueStringTmp);
            break;
          case GEM_VAL_DOUBLE:
            // sprintf(valueStringTmp,"%.6f", *(double*)menuItemTmp->linkedVariable); // May work for non-AVR boards
            dtostrf(*(double*)menuItemTmp->linkedVariable, menuItemTmp->precision + 1, menuItemTmp->precision, valueStringTmp);
            printMenuItemValue(valueStringTmp);
            break;
          #endif
        }
        break;
      }
    case GEM_ITEM_LINK:
      _agfx.setCursor(5 * _textSize, yText);
      if (menuItemTmp->readonly) {
        printMenuItemFull(menuItemTmp->title, -1);
        _agfx.print("^");
      } else {
        printMenuItemFull(menuItemTmp->title, -1 * calculateSpriteOverlap(arrowRight));
      }
      drawSprite(_agfx.width() - 8 * _spriteSize, yDraw, arrowRight, color);
      break;
    case GEM_ITEM_BACK:
      drawSprite(5 * _textSize + 2 * (_spriteSize > 1 ? 1 : 0), yDraw, arrowLeft, color);
      break;
    case GEM_ITEM_BUTTON:
      {
        byte variant = _spriteSize > 1 ? 1 : 0;
        _agfx.setCursor((5 * _textSize + arrowBtn[variant].width + 2 * variant), yText);
        if (menuItemTmp->readonly) {
          printMenuItemFull(menuItemTmp->title, -1);
          _agfx.print("^");
        } else {
          printMenuItemFull(menuItemTmp->title);
        }
        drawSprite(5 * _textSize + 2 * variant, yDraw, arrowBtn, color);
        break;
      }
    case GEM_ITEM_LABEL:
      _agfx.setCursor(5 * _textSize, yText);
      printMenuItemFull(menuItemTmp->title);
      break;
  }
  memset(valueStringTmp, '\0', GEM_STR_LEN - 1);
  _agfx.setTextColor(_menuForegroundColor);
}

void GEM_adafruit_gfx::printMenuItems() {
  byte menuItemsPerScreen = getMenuItemsPerScreen();
  byte currentPageScreenNum = _menuPageCurrent->currentItemNum / menuItemsPerScreen;
  GEMItem* menuItemTmp = _menuPageCurrent->getMenuItem(currentPageScreenNum * menuItemsPerScreen);
  byte y = getCurrentAppearance()->menuPageScreenTopOffset;
  byte i = 0;
  while (menuItemTmp != nullptr && i < menuItemsPerScreen) {
    byte yText = y + getMenuItemInsetOffset() + _menuItemFont[getMenuItemFontSize()].baselineOffset * _textSize;
    byte yDraw = y + getMenuItemInsetOffset(true);

    printMenuItem(menuItemTmp, yText, yDraw, _menuForegroundColor);

    menuItemTmp = menuItemTmp->getMenuItemNext();
    y += getCurrentAppearance()->menuItemHeight;
    i++;
  }
}

void GEM_adafruit_gfx::drawMenuPointer(bool clear) {
  if (_menuPageCurrent->itemsCount > 0) {
    GEMItem* menuItemTmp = _menuPageCurrent->getCurrentMenuItem();
    int pointerPosition = getCurrentItemTopOffset(false);
    byte menuItemHeight = getCurrentAppearance()->menuItemHeight;
    if (getCurrentAppearance()->menuPointerType == GEM_POINTER_DASH) {
      byte menuPageScreenTopOffset = getCurrentAppearance()->menuPageScreenTopOffset;
      _agfx.fillRect(0, menuPageScreenTopOffset, 2 * _spriteSize, _agfx.height() - menuPageScreenTopOffset, _menuBackgroundColor);
      if (menuItemTmp->readonly || menuItemTmp->type == GEM_ITEM_LABEL) {
        for (byte i = 0; i < (menuItemHeight - 1) / 2; i++) {
          _agfx.drawPixel(0, pointerPosition + i * 2, _menuForegroundColor);
          _agfx.drawPixel(1, pointerPosition + i * 2 + 1, _menuForegroundColor);
          if (_spriteSize > 1) {
            _agfx.drawPixel(2, pointerPosition + i * 2, _menuForegroundColor);
            _agfx.drawPixel(3, pointerPosition + i * 2 + 1, _menuForegroundColor);
          }
        }
      } else {
        _agfx.fillRect(0, pointerPosition, 2 * _spriteSize, menuItemHeight - 1, _menuForegroundColor);
      }
      if (clear) {
        byte yText = pointerPosition + getMenuItemInsetOffset() + _menuItemFont[getMenuItemFontSize()].baselineOffset * _textSize;
        byte yDraw = pointerPosition + getMenuItemInsetOffset(true);
        _agfx.fillRect(5 * _spriteSize, pointerPosition - 1, _agfx.width() - 2, menuItemHeight + 1, _menuBackgroundColor);
        printMenuItem(menuItemTmp, yText, yDraw, _menuForegroundColor);
      }
    } else {
      byte yText = pointerPosition + getMenuItemInsetOffset() + _menuItemFont[getMenuItemFontSize()].baselineOffset * _textSize;
      byte yDraw = pointerPosition + getMenuItemInsetOffset(true);
      _agfx.fillRect(0, pointerPosition - 1, _agfx.width() - 2, menuItemHeight + 1, clear ? _menuBackgroundColor : _menuForegroundColor);
      printMenuItem(menuItemTmp, yText, yDraw, clear ? _menuForegroundColor : _menuBackgroundColor);
      if (menuItemTmp->readonly || menuItemTmp->type == GEM_ITEM_LABEL) {
        for (byte i = 0; i < (menuItemHeight + 2) / 2; i++) {
          _agfx.drawPixel(0, pointerPosition + i * 2, _menuBackgroundColor);
          _agfx.drawPixel(1, pointerPosition + i * 2 - 1, _menuBackgroundColor);
          if (_spriteSize > 1) {
            _agfx.drawPixel(2, pointerPosition + i * 2, _menuBackgroundColor);
            _agfx.drawPixel(3, pointerPosition + i * 2 - 1, _menuBackgroundColor);
          }
        }
      }
    }
  }
}

void GEM_adafruit_gfx::drawScrollbar() {
  byte menuItemsPerScreen = getMenuItemsPerScreen();
  byte screensCount = (_menuPageCurrent->itemsCount % menuItemsPerScreen == 0) ? _menuPageCurrent->itemsCount / menuItemsPerScreen : _menuPageCurrent->itemsCount / menuItemsPerScreen + 1;
  if (screensCount > 1) {
    byte currentScreenNum = _menuPageCurrent->currentItemNum / menuItemsPerScreen;
    byte menuPageScreenTopOffset = getCurrentAppearance()->menuPageScreenTopOffset;
    byte scrollbarHeight = (_agfx.height() - menuPageScreenTopOffset + 1) / screensCount;
    byte scrollbarPosition = currentScreenNum * scrollbarHeight + menuPageScreenTopOffset - 1;
    _agfx.drawLine(_agfx.width() - 1, scrollbarPosition, _agfx.width() - 1, scrollbarPosition + scrollbarHeight, _menuForegroundColor);
  }
}

//====================== MENU ITEMS NAVIGATION

void GEM_adafruit_gfx::nextMenuItem() {
  if (getCurrentAppearance()->menuPointerType != GEM_POINTER_DASH) {
    drawMenuPointer(true);
  }
  if (_menuPageCurrent->currentItemNum == _menuPageCurrent->itemsCount-1) {
    _menuPageCurrent->currentItemNum = 0;
  } else {
    _menuPageCurrent->currentItemNum++;
  }
  byte menuItemsPerScreen = getMenuItemsPerScreen();
  bool redrawMenu = (_menuPageCurrent->itemsCount > menuItemsPerScreen && _menuPageCurrent->currentItemNum % menuItemsPerScreen == 0);
  if (redrawMenu) {
    drawMenu();
  } else {
    drawMenuPointer();
  }
}

void GEM_adafruit_gfx::prevMenuItem() {
  if (getCurrentAppearance()->menuPointerType != GEM_POINTER_DASH) {
    drawMenuPointer(true);
  }
  byte menuItemsPerScreen = getMenuItemsPerScreen();
  bool redrawMenu = (_menuPageCurrent->itemsCount > menuItemsPerScreen && _menuPageCurrent->currentItemNum % menuItemsPerScreen == 0);
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
  if ((getCurrentAppearance()->menuPointerType != GEM_POINTER_DASH) && (_editValueType != GEM_VAL_BOOL)) {
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
    case GEM_VAL_BOOL:
      checkboxToggle();
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

void GEM_adafruit_gfx::checkboxToggle() {
  GEMItem* menuItemTmp = _menuPageCurrent->getCurrentMenuItem();
  int topOffset = getCurrentItemTopOffset(true, true);
  bool checkboxValue = *(bool*)menuItemTmp->linkedVariable;
  *(bool*)menuItemTmp->linkedVariable = !checkboxValue;
  if (menuItemTmp->callbackAction != nullptr) {
    resetEditValueState(); // Explicitly reset edit value state to be more predictable before user-defined callback is called
    if (menuItemTmp->callbackWithArgs) {
      menuItemTmp->callbackActionArg(menuItemTmp->callbackData);
    } else {
      menuItemTmp->callbackAction();
    }
    if (!_editValueMode) { // Edge case e.g. when edit mode was activated from inside callback (e.g. on another menu item)
      drawMenu();
    }
  } else {
    byte menuPointerType = getCurrentAppearance()->menuPointerType;
    uint16_t foreColor = (menuPointerType == GEM_POINTER_DASH) ? _menuForegroundColor : _menuBackgroundColor;
    uint16_t backColor = (menuPointerType == GEM_POINTER_DASH) ? _menuBackgroundColor : _menuForegroundColor;
    byte variant = _spriteSize > 1 ? 1 : 0;
    byte menuValuesLeftOffset = getCurrentAppearance()->menuValuesLeftOffset;
    if (!checkboxValue) {
      _agfx.fillRect(menuValuesLeftOffset, topOffset, checkboxChecked[variant].width, checkboxChecked[variant].height, backColor);
      drawSprite(menuValuesLeftOffset, topOffset, checkboxChecked, foreColor);
    } else {
      _agfx.fillRect(menuValuesLeftOffset, topOffset, checkboxUnchecked[variant].width, checkboxUnchecked[variant].height, backColor);
      drawSprite(menuValuesLeftOffset, topOffset, checkboxUnchecked, foreColor);
    }
    _editValueMode = false;
  }
}

void GEM_adafruit_gfx::clearValueVisibleRange() {
  int pointerPosition = getCurrentItemTopOffset(false);
  byte cursorLeftOffset = getCurrentAppearance()->menuValuesLeftOffset;
  _agfx.fillRect(cursorLeftOffset - 1, pointerPosition - 1, _agfx.width() - cursorLeftOffset - 1, getCurrentAppearance()->menuItemHeight + 1, _menuBackgroundColor);
}

void GEM_adafruit_gfx::initEditValueCursor() {
  _editValueCursorPosition = 0;
  _editValueVirtualCursorPosition = 0;
  if (_editValueType == GEM_VAL_SELECT || _editValueType == GEM_VAL_SPINNER) {
    drawEditValueSelect();
  } else {
    char chr = _valueString[_editValueVirtualCursorPosition];
    drawEditValueDigit(chr);
  }
}

void GEM_adafruit_gfx::nextEditValueCursorPosition() {
  char chr = _valueString[_editValueVirtualCursorPosition];
  drawEditValueDigit(chr, true);
  byte menuItemValueLength = getMenuItemValueLength();
  if ((_editValueCursorPosition != menuItemValueLength - 1) && (_editValueCursorPosition != _editValueLength - 1) && (_valueString[_editValueCursorPosition] != '\0')) {
    _editValueCursorPosition++;
  }
  if ((_editValueVirtualCursorPosition != _editValueLength - 1) && (_valueString[_editValueVirtualCursorPosition] != '\0')) {
    _editValueVirtualCursorPosition++;
    if (_editValueCursorPosition == menuItemValueLength - 1) {
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

void GEM_adafruit_gfx::drawEditValueCursor(bool clear) {
  int pointerPosition = getCurrentItemTopOffset(false);
  byte menuItemFontSize = getMenuItemFontSize();
  byte menuValuesLeftOffset = getCurrentAppearance()->menuValuesLeftOffset;
  byte cursorLeftOffset = menuValuesLeftOffset + _editValueCursorPosition * _menuItemFont[menuItemFontSize].width * _textSize;
  if (_editValueType == GEM_VAL_SELECT || _editValueType == GEM_VAL_SPINNER) {
    _agfx.fillRect(cursorLeftOffset - 1, pointerPosition - 1, _agfx.width() - cursorLeftOffset - 1, getCurrentAppearance()->menuItemHeight + 1, clear ? _menuBackgroundColor : _menuForegroundColor);
  } else {
    _agfx.fillRect(cursorLeftOffset - 1, pointerPosition - 1, _menuItemFont[menuItemFontSize].width * _textSize + 1, getCurrentAppearance()->menuItemHeight + 1, clear ? _menuBackgroundColor : _menuForegroundColor);
    byte yText = pointerPosition + getMenuItemInsetOffset() + _menuItemFont[menuItemFontSize].baselineOffset * _textSize;
    _agfx.setCursor(menuValuesLeftOffset, yText);
  }
}

void GEM_adafruit_gfx::nextEditValueDigit() {
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

void GEM_adafruit_gfx::prevEditValueDigit() {
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

void GEM_adafruit_gfx::drawEditValueDigit(byte code, bool clear) {
  drawEditValueCursor(clear);
  uint16_t foreColor = (clear) ? _menuForegroundColor : _menuBackgroundColor;
  uint16_t backColor = (clear) ? _menuBackgroundColor : _menuForegroundColor;
  int pointerPosition = getCurrentItemTopOffset(false);
  byte menuItemFontSize = getMenuItemFontSize();
  byte xText = getCurrentAppearance()->menuValuesLeftOffset + _editValueCursorPosition * _menuItemFont[menuItemFontSize].width * _textSize;
  byte yText = pointerPosition + getMenuItemInsetOffset() + _menuItemFont[menuItemFontSize].baselineOffset * _textSize;
  char chrNew = (char)code;
  if (chrNew != '\0') {
    _valueString[_editValueVirtualCursorPosition] = chrNew;
    _agfx.drawChar(xText, yText, code, foreColor, backColor, _textSize);
  }
}

void GEM_adafruit_gfx::nextEditValueSelect() {
  GEMItem* menuItemTmp = _menuPageCurrent->getCurrentMenuItem();
  GEMSelect* select = menuItemTmp->select;
  if (_valueSelectNum+1 < select->getLength()) {
    _valueSelectNum++;
  } else if (select->getLoop()) {
    _valueSelectNum = 0;
  }
  drawEditValueSelect();
}

void GEM_adafruit_gfx::prevEditValueSelect() {
  GEMItem* menuItemTmp = _menuPageCurrent->getCurrentMenuItem();
  GEMSelect* select = menuItemTmp->select;
  if (_valueSelectNum > 0) {
    _valueSelectNum--;
  } else if (select->getLoop()) {
    _valueSelectNum = select->getLength() - 1;
  }
  drawEditValueSelect();
}

#ifdef GEM_SUPPORT_SPINNER
void GEM_adafruit_gfx::nextEditValueSpinner() {
  GEMItem* menuItemTmp = _menuPageCurrent->getCurrentMenuItem();
  GEMSpinner* spinner = menuItemTmp->spinner;
  if (_valueSelectNum+1 < spinner->getLength()) {
    _valueSelectNum++;
  } else if (spinner->getLoop()) {
    _valueSelectNum = 0;
  }
  drawEditValueSelect();
}

void GEM_adafruit_gfx::prevEditValueSpinner() {
  GEMItem* menuItemTmp = _menuPageCurrent->getCurrentMenuItem();
  GEMSpinner* spinner = menuItemTmp->spinner;
  if (_valueSelectNum > 0) {
    _valueSelectNum--;
  } else if (spinner->getLoop()) {
    _valueSelectNum = spinner->getLength() - 1;
  }
  drawEditValueSelect();
}
#endif

void GEM_adafruit_gfx::drawEditValueSelect() {
  GEMItem* menuItemTmp = _menuPageCurrent->getCurrentMenuItem();
  drawEditValueCursor();
  _agfx.setTextColor(_menuBackgroundColor);
  
  int pointerPosition = getCurrentItemTopOffset(false);
  byte yText = pointerPosition + getMenuItemInsetOffset() + _menuItemFont[getMenuItemFontSize()].baselineOffset * _textSize;
  _agfx.setCursor(getCurrentAppearance()->menuValuesLeftOffset, yText);
  
  switch (menuItemTmp->linkedType) {
    case GEM_VAL_SELECT:
      {
        GEMSelect* select = menuItemTmp->select;
        printMenuItemValue(select->getOptionNameByIndex(_valueSelectNum), -1 * calculateSpriteOverlap(selectArrows));
      }
      break;
    #ifdef GEM_SUPPORT_SPINNER
    case GEM_VAL_SPINNER:
      {
        char valueStringTmp[GEM_STR_LEN];
        GEMSpinner* spinner = menuItemTmp->spinner;
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
        printMenuItemValue(valueStringTmp, -1 * calculateSpriteOverlap(selectArrows));
      }
      break;
    #endif
  }

  drawSprite(_agfx.width() - 7 * _spriteSize, getCurrentItemTopOffset(true, true), selectArrows, _menuBackgroundColor);
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
    if (!_editValueMode) { // Edge case e.g. when edit mode was activated from inside callback (e.g. on another menu item)
      drawMenu();
    }
  } else {
    // exitEditValue(false); // Can speed up work of Adafruit GFX version of GEM on UNO R3, but disabled to be in line with other GEM versions
    exitEditValue();
  }
}

void GEM_adafruit_gfx::cancelEditValue() {
  exitEditValue(false);
}

void GEM_adafruit_gfx::resetEditValueState() {
  memset(_valueString, '\0', GEM_STR_LEN - 1);
  _valueSelectNum = -1;
  _editValueMode = false;
}

void GEM_adafruit_gfx::exitEditValue(bool redrawMenu) {
  resetEditValueState();
  if (redrawMenu) {
    drawMenu();
  } else {
    drawMenuPointer(getCurrentAppearance()->menuPointerType == GEM_POINTER_DASH);
  }
}

bool GEM_adafruit_gfx::isEditMode() {
  return _editValueMode;
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

bool GEM_adafruit_gfx::readyForKey() {
  if ( (context.loop == nullptr) ||
      ((context.loop != nullptr) && (context.allowExit)) ) {
    return true;
  } else {
    registerKeyPress(GEM_KEY_NONE);
    return false;
  }

}

GEM_adafruit_gfx& GEM_adafruit_gfx::registerKeyPress(byte keyCode) {
  _currentKey = keyCode;
  dispatchKeyPress();
  return *this;
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
