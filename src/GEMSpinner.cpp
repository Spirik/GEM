/*
  GEMSpinner - increment/decrement spinner for GEM library (similar to GEMSpinner).

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
#include "GEMSpinner.h"
#include "constants.h"

GEMSpinner::GEMSpinner(GEMSpinnerBoundariesByte boundaries_, bool loop_)
  : _boundaries{ { .boundariesByte = { .step = boundaries_.step, .min = boundaries_.min < boundaries_.max ? boundaries_.min : boundaries_.max, .max = boundaries_.max > boundaries_.min ? boundaries_.max : boundaries_.min } } }
  , _type(GEM_VAL_BYTE)
  , _length(abs((boundaries_.max - boundaries_.min) / boundaries_.step) + 1)
  , _loop(loop_)
{ }

GEMSpinner::GEMSpinner(GEMSpinnerBoundariesInt boundaries_, bool loop_)
  : _boundaries{ { .boundariesInt = { .step = abs(boundaries_.step), .min = boundaries_.min < boundaries_.max ? boundaries_.min : boundaries_.max, .max = boundaries_.max > boundaries_.min ? boundaries_.max : boundaries_.min } } }
  , _type(GEM_VAL_INTEGER)
  , _length(abs((boundaries_.max - boundaries_.min) / boundaries_.step) + 1)
  , _loop(loop_)
{ }

#ifdef GEM_SUPPORT_FLOAT_EDIT
GEMSpinner::GEMSpinner(GEMSpinnerBoundariesFloat boundaries_, bool loop_)
  : _boundaries{ { .boundariesFloat = { .step = abs(boundaries_.step), .min = boundaries_.min < boundaries_.max ? boundaries_.min : boundaries_.max, .max = boundaries_.max > boundaries_.min ? boundaries_.max : boundaries_.min } } }
  , _type(GEM_VAL_FLOAT)
  , _length(abs((boundaries_.max - boundaries_.min) / boundaries_.step) + 1)
  , _loop(loop_)
{ }

GEMSpinner::GEMSpinner(GEMSpinnerBoundariesDouble boundaries_, bool loop_)
  : _boundaries{ { .boundariesDouble = { .step = abs(boundaries_.step), .min = boundaries_.min < boundaries_.max ? boundaries_.min : boundaries_.max, .max = boundaries_.max > boundaries_.min ? boundaries_.max : boundaries_.min } } }
  , _type(GEM_VAL_DOUBLE)
  , _length(abs((boundaries_.max - boundaries_.min) / boundaries_.step) + 1)
  , _loop(loop_)
{ }
#endif

GEMSpinner& GEMSpinner::setLoop(bool mode) {
  _loop = mode;
  return *this;
}

bool GEMSpinner::getLoop() {
  return _loop;
}

byte GEMSpinner::getType() {
  return _type;
}

int GEMSpinner::getLength() {
  return _length;
}

int GEMSpinner::getSelectedOptionNum(void* variable) {
  int num = -1;
  switch (_type) {
    case GEM_VAL_BYTE:
      {
        GEMSpinnerBoundariesByte boundaries = _boundaries.boundariesByte;
        byte val = *(byte*)variable;
        if (val >= boundaries.min && val <= boundaries.max) {
          num = (val - boundaries.min) / boundaries.step;
        }
      }
      break;
    case GEM_VAL_INTEGER:
      {
        GEMSpinnerBoundariesInt boundaries = _boundaries.boundariesInt;
        int val = *(int*)variable;
        if (val >= boundaries.min && val <= boundaries.max) {
          num = (val - boundaries.min) / boundaries.step;
        }
      }
      break;
    #ifdef GEM_SUPPORT_FLOAT_EDIT
    case GEM_VAL_FLOAT:
      {
        GEMSpinnerBoundariesFloat boundaries = _boundaries.boundariesFloat;
        float val = *(float*)variable;
        if (val >= boundaries.min && val <= boundaries.max) {
          num = (val - boundaries.min) / boundaries.step;
        }
      }
      break;
    case GEM_VAL_DOUBLE:
      {
        GEMSpinnerBoundariesDouble boundaries = _boundaries.boundariesDouble;
        double val = *(double*)variable;
        if (val >= boundaries.min && val <= boundaries.max) {
          num = (val - boundaries.min) / boundaries.step;
        }
      }
      break;
    #endif
  }
  return num;
}

GEMSpinnerValue GEMSpinner::getOptionNameByIndex(void* variable, int index) {
  int selectedOptionNum = getSelectedOptionNum(variable);
  GEMSpinnerValue value = { 0 };
  switch (_type) {
    case GEM_VAL_BYTE:
      if (selectedOptionNum > -1) {
        GEMSpinnerBoundariesByte boundaries = _boundaries.boundariesByte;
        byte val = *(byte*)variable + (index - selectedOptionNum) * boundaries.step;
        value.valByte = val <= boundaries.max ? val : val - boundaries.step;
      } else {
        value.valByte = *(byte*)variable;
      }
      break;
    case GEM_VAL_INTEGER:
      if (selectedOptionNum > -1) {
        GEMSpinnerBoundariesInt boundaries = _boundaries.boundariesInt;
        int val = *(int*)variable + (index - selectedOptionNum) * boundaries.step;
        value.valInt = val <= boundaries.max ? val : val - boundaries.step;
      } else {
        value.valInt = *(int*)variable;
      }
      break;
    #ifdef GEM_SUPPORT_FLOAT_EDIT
    case GEM_VAL_FLOAT:
      if (selectedOptionNum > -1) {
        GEMSpinnerBoundariesFloat boundaries = _boundaries.boundariesFloat;
        float val = *(float*)variable + (index - selectedOptionNum) * boundaries.step;
        value.valFloat = val <= boundaries.max ? val : val - boundaries.step;
      } else {
        value.valFloat = *(float*)variable;
      }
      break;
    case GEM_VAL_DOUBLE:
      if (selectedOptionNum > -1) {
        GEMSpinnerBoundariesDouble boundaries = _boundaries.boundariesDouble;
        double val = *(double*)variable + (index - selectedOptionNum) * boundaries.step;
        value.valDouble = val <= boundaries.max ? val : val - boundaries.step;
      } else {
        value.valDouble = *(double*)variable;
      }
      break;
    #endif
  }
  return value;
}

void GEMSpinner::setValue(void* variable, int index) {
  GEMSpinnerValue value = getOptionNameByIndex(variable, index);
  switch (_type) {
    case GEM_VAL_BYTE:
      *(byte*)variable = value.valByte;
      break;
    case GEM_VAL_INTEGER:
      *(int*)variable = value.valInt;
      break;
    #ifdef GEM_SUPPORT_FLOAT_EDIT
    case GEM_VAL_FLOAT:
      *(float*)variable = value.valFloat;
      break;
    case GEM_VAL_DOUBLE:
      *(double*)variable = value.valDouble;
      break;
    #endif
  }
}
