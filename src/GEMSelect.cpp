/*
  GEMSelect - option select for GEM library.

  GEM (a.k.a. Good Enough Menu) - Arduino library for creation of graphic multi-level menu with
  editable menu items, such as variables (supports int, byte, boolean, char[17] data types) and
  option selects. User-defined callback function can be specified to invoke when menu item is saved.
  
  Supports buttons that can invoke user-defined actions and create action-specific
  context, which can have its own enter (setup) and exit callbacks as well as loop function.

  Requires AltSerialGraphicLCD library by Jon Green (http://www.jasspa.com/serialGLCD.html).
  LCD screen must be equipped with SparkFun Graphic LCD Serial Backpack and properly set up
  to operate using firmware provided with aforementioned library.

  For documentation visit:
  https://github.com/Spirik/GEM

  Created by Alexander 'Spirik' Spiridonov, 21 Nov 2017
  
  This is free software. You can redistribute it and/or modify it under
  the terms of Creative Commons Attribution-ShareAlike 4.0 International License.
  To view a copy of this license, visit https://creativecommons.org/licenses/by-sa/4.0/
*/

#include <Arduino.h>
#include "GEMSelect.h"
#include "GEM.h"

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

byte GEMSelect::getType() {
  return _type;
}

byte GEMSelect::getLength() {
  return _length;
}

int GEMSelect::getSelectedOptionNum(void* variable) {
  SelectOptionInt* optsInt = _options;
  SelectOptionByte* optsByte = _options;
  SelectOptionChar* optsChar = _options;
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
    }
    if (found) { return i; }
  }
  if (!found) { return -1; }
}

char* GEMSelect::getSelectedOptionName(void* variable) {
  int selectedOptionNum = getSelectedOptionNum(variable);
  return getOptionNameByIndex(selectedOptionNum);
}

char* GEMSelect::getOptionNameByIndex(int index) {
  char* name;
  SelectOptionInt* optsInt = _options;
  SelectOptionByte* optsByte = _options;
  SelectOptionChar* optsChar = _options;
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
  }
  return name;
}

void GEMSelect::setValue(void* variable, int index) {
  SelectOptionInt* optsInt = _options;
  SelectOptionByte* optsByte = _options;
  SelectOptionChar* optsChar = _options;
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
    }
  }
}
