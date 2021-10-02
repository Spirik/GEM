/*
  GEMSelect - option select for GEM library.

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
#include "GEMSelect.h"
#include "constants.h"

GEMSelect::GEMSelect(byte length_, SelectOptionInt* options_)
  : _type(GEM_VAL_INTEGER)
  , _length(length_)
  , _options(options_)
{ }

GEMSelect::GEMSelect(byte length_, SelectOptionByte* options_)
  : _type(GEM_VAL_BYTE)
  , _length(length_)
  , _options(options_)
{ }

GEMSelect::GEMSelect(byte length_, SelectOptionChar* options_)
  : _type(GEM_VAL_CHAR)
  , _length(length_)
  , _options(options_)
{ }

GEMSelect::GEMSelect(byte length_, SelectOptionFloat* options_)
  : _type(GEM_VAL_FLOAT)
  , _length(length_)
  , _options(options_)
{ }

GEMSelect::GEMSelect(byte length_, SelectOptionDouble* options_)
  : _type(GEM_VAL_DOUBLE)
  , _length(length_)
  , _options(options_)
{ }

byte GEMSelect::getType() {
  return _type;
}

byte GEMSelect::getLength() {
  return _length;
}

int GEMSelect::getSelectedOptionNum(void* variable) {
  SelectOptionInt* optsInt = (SelectOptionInt*)_options;
  SelectOptionByte* optsByte = (SelectOptionByte*)_options;
  SelectOptionChar* optsChar = (SelectOptionChar*)_options;
  SelectOptionFloat* optsFloat = (SelectOptionFloat*)_options;
  SelectOptionDouble* optsDouble = (SelectOptionDouble*)_options;
  boolean found = false;
  for (byte i=0; i<_length; i++) {
    switch (_type) {
      case GEM_VAL_INTEGER:
        if (optsInt[i].val_int == *(int*)variable) { found = true; }
        break;
      case GEM_VAL_BYTE:
        if (optsByte[i].val_byte == *(byte*)variable) { found = true; }
        break;
      case GEM_VAL_CHAR:
        if (strcmp(optsChar[i].val_char, (char*)variable) == 0) { found = true; }
        break;
      case GEM_VAL_FLOAT:
        if (optsFloat[i].val_float == *(float*)variable) { found = true; }
        break;
      case GEM_VAL_DOUBLE:
        if (optsDouble[i].val_double == *(double*)variable) { found = true; }
        break;
    }
    if (found) { return i; }
  }
  return -1;
}

const char* GEMSelect::getSelectedOptionName(void* variable) {
  int selectedOptionNum = getSelectedOptionNum(variable);
  return getOptionNameByIndex(selectedOptionNum);
}

const char* GEMSelect::getOptionNameByIndex(int index) {
  const char* name;
  SelectOptionInt* optsInt = (SelectOptionInt*)_options;
  SelectOptionByte* optsByte = (SelectOptionByte*)_options;
  SelectOptionChar* optsChar = (SelectOptionChar*)_options;
  SelectOptionFloat* optsFloat = (SelectOptionFloat*)_options;
  SelectOptionDouble* optsDouble = (SelectOptionDouble*)_options;
  switch (_type) {
    case GEM_VAL_INTEGER:
      name = (index > -1 && index < _length) ? optsInt[index].name : "";
      break;
    case GEM_VAL_BYTE:
      name = (index > -1 && index < _length) ? optsByte[index].name : "";
      break;
    case GEM_VAL_CHAR:
      name = (index > -1 && index < _length) ? optsChar[index].name : "";
      break;
    case GEM_VAL_FLOAT:
      name = (index > -1 && index < _length) ? optsFloat[index].name : "";
      break;
    case GEM_VAL_DOUBLE:
      name = (index > -1 && index < _length) ? optsDouble[index].name : "";
      break;
  }
  return name;
}

void GEMSelect::setValue(void* variable, int index) {
  SelectOptionInt* optsInt = (SelectOptionInt*)_options;
  SelectOptionByte* optsByte = (SelectOptionByte*)_options;
  SelectOptionChar* optsChar = (SelectOptionChar*)_options;
  SelectOptionFloat* optsFloat = (SelectOptionFloat*)_options;
  SelectOptionDouble* optsDouble = (SelectOptionDouble*)_options;
  if (index > -1 && index < _length) {
    switch (_type) {
      case GEM_VAL_INTEGER:
        *(int*)variable = optsInt[index].val_int;
        break;
      case GEM_VAL_BYTE:
        *(byte*)variable = optsByte[index].val_byte;
        break;
      case GEM_VAL_CHAR:
        strcpy((char*)variable, optsChar[index].val_char);
        break;
      case GEM_VAL_FLOAT:
        *(float*)variable = optsFloat[index].val_float;
        break;
        break;
      case GEM_VAL_DOUBLE:
        *(double*)variable = optsDouble[index].val_double;
        break;
    }
  }
}
