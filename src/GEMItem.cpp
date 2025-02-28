/*
  GEMItem - menu item for GEM library.

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
#include "GEMItem.h"
#include "constants.h"

GEMItem::GEMItem(const char* title_)
  : title(title_)
  , type(GEM_ITEM_LABEL)
{ }

//---

GEMItem::GEMItem(const char* title_, byte& linkedVariable_, GEMSelect& select_, void (*callbackAction_)())
  : title(title_)
  , linkedVariable(&linkedVariable_)
  , linkedType(GEM_VAL_SELECT)
  , type(GEM_ITEM_VAL)
  , select(&select_)
  , callbackAction(callbackAction_)
{ }

GEMItem::GEMItem(const char* title_, int& linkedVariable_, GEMSelect& select_, void (*callbackAction_)())
  : title(title_)
  , linkedVariable(&linkedVariable_)
  , linkedType(GEM_VAL_SELECT)
  , type(GEM_ITEM_VAL)
  , select(&select_)
  , callbackAction(callbackAction_)
{ }

GEMItem::GEMItem(const char* title_, char* linkedVariable_, GEMSelect& select_, void (*callbackAction_)())
  : title(title_)
  , linkedVariable(linkedVariable_)
  , linkedType(GEM_VAL_SELECT)
  , type(GEM_ITEM_VAL)
  , select(&select_)
  , callbackAction(callbackAction_)
{ }

GEMItem::GEMItem(const char* title_, float& linkedVariable_, GEMSelect& select_, void (*callbackAction_)())
  : title(title_)
  , linkedVariable(&linkedVariable_)
  , linkedType(GEM_VAL_SELECT)
  , type(GEM_ITEM_VAL)
  , select(&select_)
  , callbackAction(callbackAction_)
{ }

GEMItem::GEMItem(const char* title_, double& linkedVariable_, GEMSelect& select_, void (*callbackAction_)())
  : title(title_)
  , linkedVariable(&linkedVariable_)
  , linkedType(GEM_VAL_SELECT)
  , type(GEM_ITEM_VAL)
  , select(&select_)
  , callbackAction(callbackAction_)
{ }

//---

GEMItem::GEMItem(const char* title_, byte& linkedVariable_, GEMSelect& select_, void (*callbackAction_)(GEMCallbackData))
  : title(title_)
  , linkedVariable(&linkedVariable_)
  , linkedType(GEM_VAL_SELECT)
  , type(GEM_ITEM_VAL)
  , select(&select_)
  , callbackActionArg(callbackAction_)
  , callbackWithArgs(true)
  , callbackData{ this, { 0 } }
{ }

GEMItem::GEMItem(const char* title_, byte& linkedVariable_, GEMSelect& select_, void (*callbackAction_)(GEMCallbackData), byte callbackVal_)
  : title(title_)
  , linkedVariable(&linkedVariable_)
  , linkedType(GEM_VAL_SELECT)
  , type(GEM_ITEM_VAL)
  , select(&select_)
  , callbackActionArg(callbackAction_)
  , callbackWithArgs(true)
  , callbackData{ this, { .valByte = callbackVal_ }}
{ }

GEMItem::GEMItem(const char* title_, byte& linkedVariable_, GEMSelect& select_, void (*callbackAction_)(GEMCallbackData), int callbackVal_)
  : title(title_)
  , linkedVariable(&linkedVariable_)
  , linkedType(GEM_VAL_SELECT)
  , type(GEM_ITEM_VAL)
  , select(&select_)
  , callbackActionArg(callbackAction_)
  , callbackWithArgs(true)
  , callbackData{ this, { .valInt = callbackVal_ }}
{ }

GEMItem::GEMItem(const char* title_, byte& linkedVariable_, GEMSelect& select_, void (*callbackAction_)(GEMCallbackData), float callbackVal_)
  : title(title_)
  , linkedVariable(&linkedVariable_)
  , linkedType(GEM_VAL_SELECT)
  , type(GEM_ITEM_VAL)
  , select(&select_)
  , callbackActionArg(callbackAction_)
  , callbackWithArgs(true)
  , callbackData{ this, { .valFloat = callbackVal_ }}
{ }

GEMItem::GEMItem(const char* title_, byte& linkedVariable_, GEMSelect& select_, void (*callbackAction_)(GEMCallbackData), double callbackVal_)
  : title(title_)
  , linkedVariable(&linkedVariable_)
  , linkedType(GEM_VAL_SELECT)
  , type(GEM_ITEM_VAL)
  , select(&select_)
  , callbackActionArg(callbackAction_)
  , callbackWithArgs(true)
  , callbackData{ this, { .valDouble = callbackVal_ }}
{ }

GEMItem::GEMItem(const char* title_, byte& linkedVariable_, GEMSelect& select_, void (*callbackAction_)(GEMCallbackData), bool callbackVal_)
  : title(title_)
  , linkedVariable(&linkedVariable_)
  , linkedType(GEM_VAL_SELECT)
  , type(GEM_ITEM_VAL)
  , select(&select_)
  , callbackActionArg(callbackAction_)
  , callbackWithArgs(true)
  , callbackData{ this, { .valBool = callbackVal_ }}
{ }

GEMItem::GEMItem(const char* title_, byte& linkedVariable_, GEMSelect& select_, void (*callbackAction_)(GEMCallbackData), const char* callbackVal_)
  : title(title_)
  , linkedVariable(&linkedVariable_)
  , linkedType(GEM_VAL_SELECT)
  , type(GEM_ITEM_VAL)
  , select(&select_)
  , callbackActionArg(callbackAction_)
  , callbackWithArgs(true)
  , callbackData{ this, { .valChar = callbackVal_ }}
{ }

GEMItem::GEMItem(const char* title_, byte& linkedVariable_, GEMSelect& select_, void (*callbackAction_)(GEMCallbackData), void* callbackVal_)
  : title(title_)
  , linkedVariable(&linkedVariable_)
  , linkedType(GEM_VAL_SELECT)
  , type(GEM_ITEM_VAL)
  , select(&select_)
  , callbackActionArg(callbackAction_)
  , callbackWithArgs(true)
  , callbackData{ this, { .valPointer = callbackVal_ }}
{ }

//---

GEMItem::GEMItem(const char* title_, int& linkedVariable_, GEMSelect& select_, void (*callbackAction_)(GEMCallbackData))
  : title(title_)
  , linkedVariable(&linkedVariable_)
  , linkedType(GEM_VAL_SELECT)
  , type(GEM_ITEM_VAL)
  , select(&select_)
  , callbackActionArg(callbackAction_)
  , callbackWithArgs(true)
  , callbackData{ this, { 0 } }
{ }

GEMItem::GEMItem(const char* title_, int& linkedVariable_, GEMSelect& select_, void (*callbackAction_)(GEMCallbackData), byte callbackVal_)
  : title(title_)
  , linkedVariable(&linkedVariable_)
  , linkedType(GEM_VAL_SELECT)
  , type(GEM_ITEM_VAL)
  , select(&select_)
  , callbackActionArg(callbackAction_)
  , callbackWithArgs(true)
  , callbackData{ this, { .valByte = callbackVal_ }}
{ }

GEMItem::GEMItem(const char* title_, int& linkedVariable_, GEMSelect& select_, void (*callbackAction_)(GEMCallbackData), int callbackVal_)
  : title(title_)
  , linkedVariable(&linkedVariable_)
  , linkedType(GEM_VAL_SELECT)
  , type(GEM_ITEM_VAL)
  , select(&select_)
  , callbackActionArg(callbackAction_)
  , callbackWithArgs(true)
  , callbackData{ this, { .valInt = callbackVal_ }}
{ }

GEMItem::GEMItem(const char* title_, int& linkedVariable_, GEMSelect& select_, void (*callbackAction_)(GEMCallbackData), float callbackVal_)
  : title(title_)
  , linkedVariable(&linkedVariable_)
  , linkedType(GEM_VAL_SELECT)
  , type(GEM_ITEM_VAL)
  , select(&select_)
  , callbackActionArg(callbackAction_)
  , callbackWithArgs(true)
  , callbackData{ this, { .valFloat = callbackVal_ }}
{ }

GEMItem::GEMItem(const char* title_, int& linkedVariable_, GEMSelect& select_, void (*callbackAction_)(GEMCallbackData), double callbackVal_)
  : title(title_)
  , linkedVariable(&linkedVariable_)
  , linkedType(GEM_VAL_SELECT)
  , type(GEM_ITEM_VAL)
  , select(&select_)
  , callbackActionArg(callbackAction_)
  , callbackWithArgs(true)
  , callbackData{ this, { .valDouble = callbackVal_ }}
{ }

GEMItem::GEMItem(const char* title_, int& linkedVariable_, GEMSelect& select_, void (*callbackAction_)(GEMCallbackData), bool callbackVal_)
  : title(title_)
  , linkedVariable(&linkedVariable_)
  , linkedType(GEM_VAL_SELECT)
  , type(GEM_ITEM_VAL)
  , select(&select_)
  , callbackActionArg(callbackAction_)
  , callbackWithArgs(true)
  , callbackData{ this, { .valBool = callbackVal_ }}
{ }

GEMItem::GEMItem(const char* title_, int& linkedVariable_, GEMSelect& select_, void (*callbackAction_)(GEMCallbackData), const char* callbackVal_)
  : title(title_)
  , linkedVariable(&linkedVariable_)
  , linkedType(GEM_VAL_SELECT)
  , type(GEM_ITEM_VAL)
  , select(&select_)
  , callbackActionArg(callbackAction_)
  , callbackWithArgs(true)
  , callbackData{ this, { .valChar = callbackVal_ }}
{ }

GEMItem::GEMItem(const char* title_, int& linkedVariable_, GEMSelect& select_, void (*callbackAction_)(GEMCallbackData), void* callbackVal_)
  : title(title_)
  , linkedVariable(&linkedVariable_)
  , linkedType(GEM_VAL_SELECT)
  , type(GEM_ITEM_VAL)
  , select(&select_)
  , callbackActionArg(callbackAction_)
  , callbackWithArgs(true)
  , callbackData{ this, { .valPointer = callbackVal_ }}
{ }

//---

GEMItem::GEMItem(const char* title_, char* linkedVariable_, GEMSelect& select_, void (*callbackAction_)(GEMCallbackData))
  : title(title_)
  , linkedVariable(linkedVariable_)
  , linkedType(GEM_VAL_SELECT)
  , type(GEM_ITEM_VAL)
  , select(&select_)
  , callbackActionArg(callbackAction_)
  , callbackWithArgs(true)
  , callbackData{ this, { 0 } }
{ }

GEMItem::GEMItem(const char* title_, char* linkedVariable_, GEMSelect& select_, void (*callbackAction_)(GEMCallbackData), byte callbackVal_)
  : title(title_)
  , linkedVariable(linkedVariable_)
  , linkedType(GEM_VAL_SELECT)
  , type(GEM_ITEM_VAL)
  , select(&select_)
  , callbackActionArg(callbackAction_)
  , callbackWithArgs(true)
  , callbackData{ this, { .valByte = callbackVal_ }}
{ }

GEMItem::GEMItem(const char* title_, char* linkedVariable_, GEMSelect& select_, void (*callbackAction_)(GEMCallbackData), int callbackVal_)
  : title(title_)
  , linkedVariable(linkedVariable_)
  , linkedType(GEM_VAL_SELECT)
  , type(GEM_ITEM_VAL)
  , select(&select_)
  , callbackActionArg(callbackAction_)
  , callbackWithArgs(true)
  , callbackData{ this, { .valInt = callbackVal_ }}
{ }

GEMItem::GEMItem(const char* title_, char* linkedVariable_, GEMSelect& select_, void (*callbackAction_)(GEMCallbackData), float callbackVal_)
  : title(title_)
  , linkedVariable(linkedVariable_)
  , linkedType(GEM_VAL_SELECT)
  , type(GEM_ITEM_VAL)
  , select(&select_)
  , callbackActionArg(callbackAction_)
  , callbackWithArgs(true)
  , callbackData{ this, { .valFloat = callbackVal_ }}
{ }

GEMItem::GEMItem(const char* title_, char* linkedVariable_, GEMSelect& select_, void (*callbackAction_)(GEMCallbackData), double callbackVal_)
  : title(title_)
  , linkedVariable(linkedVariable_)
  , linkedType(GEM_VAL_SELECT)
  , type(GEM_ITEM_VAL)
  , select(&select_)
  , callbackActionArg(callbackAction_)
  , callbackWithArgs(true)
  , callbackData{ this, { .valDouble = callbackVal_ }}
{ }

GEMItem::GEMItem(const char* title_, char* linkedVariable_, GEMSelect& select_, void (*callbackAction_)(GEMCallbackData), bool callbackVal_)
  : title(title_)
  , linkedVariable(linkedVariable_)
  , linkedType(GEM_VAL_SELECT)
  , type(GEM_ITEM_VAL)
  , select(&select_)
  , callbackActionArg(callbackAction_)
  , callbackWithArgs(true)
  , callbackData{ this, { .valBool = callbackVal_ }}
{ }

GEMItem::GEMItem(const char* title_, char* linkedVariable_, GEMSelect& select_, void (*callbackAction_)(GEMCallbackData), const char* callbackVal_)
  : title(title_)
  , linkedVariable(linkedVariable_)
  , linkedType(GEM_VAL_SELECT)
  , type(GEM_ITEM_VAL)
  , select(&select_)
  , callbackActionArg(callbackAction_)
  , callbackWithArgs(true)
  , callbackData{ this, { .valChar = callbackVal_ }}
{ }

GEMItem::GEMItem(const char* title_, char* linkedVariable_, GEMSelect& select_, void (*callbackAction_)(GEMCallbackData), void* callbackVal_)
  : title(title_)
  , linkedVariable(linkedVariable_)
  , linkedType(GEM_VAL_SELECT)
  , type(GEM_ITEM_VAL)
  , select(&select_)
  , callbackActionArg(callbackAction_)
  , callbackWithArgs(true)
  , callbackData{ this, { .valPointer = callbackVal_ }}
{ }

//---

GEMItem::GEMItem(const char* title_, float& linkedVariable_, GEMSelect& select_, void (*callbackAction_)(GEMCallbackData))
  : title(title_)
  , linkedVariable(&linkedVariable_)
  , linkedType(GEM_VAL_SELECT)
  , type(GEM_ITEM_VAL)
  , select(&select_)
  , callbackActionArg(callbackAction_)
  , callbackWithArgs(true)
  , callbackData{ this, { 0 } }
{ }

GEMItem::GEMItem(const char* title_, float& linkedVariable_, GEMSelect& select_, void (*callbackAction_)(GEMCallbackData), byte callbackVal_)
  : title(title_)
  , linkedVariable(&linkedVariable_)
  , linkedType(GEM_VAL_SELECT)
  , type(GEM_ITEM_VAL)
  , select(&select_)
  , callbackActionArg(callbackAction_)
  , callbackWithArgs(true)
  , callbackData{ this, { .valByte = callbackVal_ }}
{ }

GEMItem::GEMItem(const char* title_, float& linkedVariable_, GEMSelect& select_, void (*callbackAction_)(GEMCallbackData), int callbackVal_)
  : title(title_)
  , linkedVariable(&linkedVariable_)
  , linkedType(GEM_VAL_SELECT)
  , type(GEM_ITEM_VAL)
  , select(&select_)
  , callbackActionArg(callbackAction_)
  , callbackWithArgs(true)
  , callbackData{ this, { .valInt = callbackVal_ }}
{ }

GEMItem::GEMItem(const char* title_, float& linkedVariable_, GEMSelect& select_, void (*callbackAction_)(GEMCallbackData), float callbackVal_)
  : title(title_)
  , linkedVariable(&linkedVariable_)
  , linkedType(GEM_VAL_SELECT)
  , type(GEM_ITEM_VAL)
  , select(&select_)
  , callbackActionArg(callbackAction_)
  , callbackWithArgs(true)
  , callbackData{ this, { .valFloat = callbackVal_ }}
{ }

GEMItem::GEMItem(const char* title_, float& linkedVariable_, GEMSelect& select_, void (*callbackAction_)(GEMCallbackData), double callbackVal_)
  : title(title_)
  , linkedVariable(&linkedVariable_)
  , linkedType(GEM_VAL_SELECT)
  , type(GEM_ITEM_VAL)
  , select(&select_)
  , callbackActionArg(callbackAction_)
  , callbackWithArgs(true)
  , callbackData{ this, { .valDouble = callbackVal_ }}
{ }

GEMItem::GEMItem(const char* title_, float& linkedVariable_, GEMSelect& select_, void (*callbackAction_)(GEMCallbackData), bool callbackVal_)
  : title(title_)
  , linkedVariable(&linkedVariable_)
  , linkedType(GEM_VAL_SELECT)
  , type(GEM_ITEM_VAL)
  , select(&select_)
  , callbackActionArg(callbackAction_)
  , callbackWithArgs(true)
  , callbackData{ this, { .valBool = callbackVal_ }}
{ }

GEMItem::GEMItem(const char* title_, float& linkedVariable_, GEMSelect& select_, void (*callbackAction_)(GEMCallbackData), const char* callbackVal_)
  : title(title_)
  , linkedVariable(&linkedVariable_)
  , linkedType(GEM_VAL_SELECT)
  , type(GEM_ITEM_VAL)
  , select(&select_)
  , callbackActionArg(callbackAction_)
  , callbackWithArgs(true)
  , callbackData{ this, { .valChar = callbackVal_ }}
{ }

GEMItem::GEMItem(const char* title_, float& linkedVariable_, GEMSelect& select_, void (*callbackAction_)(GEMCallbackData), void* callbackVal_)
  : title(title_)
  , linkedVariable(&linkedVariable_)
  , linkedType(GEM_VAL_SELECT)
  , type(GEM_ITEM_VAL)
  , select(&select_)
  , callbackActionArg(callbackAction_)
  , callbackWithArgs(true)
  , callbackData{ this, { .valPointer = callbackVal_ }}
{ }

//---

GEMItem::GEMItem(const char* title_, double& linkedVariable_, GEMSelect& select_, void (*callbackAction_)(GEMCallbackData))
  : title(title_)
  , linkedVariable(&linkedVariable_)
  , linkedType(GEM_VAL_SELECT)
  , type(GEM_ITEM_VAL)
  , select(&select_)
  , callbackActionArg(callbackAction_)
  , callbackWithArgs(true)
  , callbackData{ this, { 0 } }
{ }

GEMItem::GEMItem(const char* title_, double& linkedVariable_, GEMSelect& select_, void (*callbackAction_)(GEMCallbackData), byte callbackVal_)
  : title(title_)
  , linkedVariable(&linkedVariable_)
  , linkedType(GEM_VAL_SELECT)
  , type(GEM_ITEM_VAL)
  , select(&select_)
  , callbackActionArg(callbackAction_)
  , callbackWithArgs(true)
  , callbackData{ this, { .valByte = callbackVal_ }}
{ }

GEMItem::GEMItem(const char* title_, double& linkedVariable_, GEMSelect& select_, void (*callbackAction_)(GEMCallbackData), int callbackVal_)
  : title(title_)
  , linkedVariable(&linkedVariable_)
  , linkedType(GEM_VAL_SELECT)
  , type(GEM_ITEM_VAL)
  , select(&select_)
  , callbackActionArg(callbackAction_)
  , callbackWithArgs(true)
  , callbackData{ this, { .valInt = callbackVal_ }}
{ }

GEMItem::GEMItem(const char* title_, double& linkedVariable_, GEMSelect& select_, void (*callbackAction_)(GEMCallbackData), float callbackVal_)
  : title(title_)
  , linkedVariable(&linkedVariable_)
  , linkedType(GEM_VAL_SELECT)
  , type(GEM_ITEM_VAL)
  , select(&select_)
  , callbackActionArg(callbackAction_)
  , callbackWithArgs(true)
  , callbackData{ this, { .valFloat = callbackVal_ }}
{ }

GEMItem::GEMItem(const char* title_, double& linkedVariable_, GEMSelect& select_, void (*callbackAction_)(GEMCallbackData), double callbackVal_)
  : title(title_)
  , linkedVariable(&linkedVariable_)
  , linkedType(GEM_VAL_SELECT)
  , type(GEM_ITEM_VAL)
  , select(&select_)
  , callbackActionArg(callbackAction_)
  , callbackWithArgs(true)
  , callbackData{ this, { .valDouble = callbackVal_ }}
{ }

GEMItem::GEMItem(const char* title_, double& linkedVariable_, GEMSelect& select_, void (*callbackAction_)(GEMCallbackData), bool callbackVal_)
  : title(title_)
  , linkedVariable(&linkedVariable_)
  , linkedType(GEM_VAL_SELECT)
  , type(GEM_ITEM_VAL)
  , select(&select_)
  , callbackActionArg(callbackAction_)
  , callbackWithArgs(true)
  , callbackData{ this, { .valBool = callbackVal_ }}
{ }

GEMItem::GEMItem(const char* title_, double& linkedVariable_, GEMSelect& select_, void (*callbackAction_)(GEMCallbackData), const char* callbackVal_)
  : title(title_)
  , linkedVariable(&linkedVariable_)
  , linkedType(GEM_VAL_SELECT)
  , type(GEM_ITEM_VAL)
  , select(&select_)
  , callbackActionArg(callbackAction_)
  , callbackWithArgs(true)
  , callbackData{ this, { .valChar = callbackVal_ }}
{ }

GEMItem::GEMItem(const char* title_, double& linkedVariable_, GEMSelect& select_, void (*callbackAction_)(GEMCallbackData), void* callbackVal_)
  : title(title_)
  , linkedVariable(&linkedVariable_)
  , linkedType(GEM_VAL_SELECT)
  , type(GEM_ITEM_VAL)
  , select(&select_)
  , callbackActionArg(callbackAction_)
  , callbackWithArgs(true)
  , callbackData{ this, { .valPointer = callbackVal_ }}
{ }

//---

GEMItem::GEMItem(const char* title_, byte& linkedVariable_, GEMSelect& select_, bool readonly_)
  : title(title_)
  , linkedVariable(&linkedVariable_)
  , linkedType(GEM_VAL_SELECT)
  , type(GEM_ITEM_VAL)
  , readonly(readonly_)
  , select(&select_)
{ }

GEMItem::GEMItem(const char* title_, int& linkedVariable_, GEMSelect& select_, bool readonly_)
  : title(title_)
  , linkedVariable(&linkedVariable_)
  , linkedType(GEM_VAL_SELECT)
  , type(GEM_ITEM_VAL)
  , readonly(readonly_)
  , select(&select_)
{ }

GEMItem::GEMItem(const char* title_, char* linkedVariable_, GEMSelect& select_, bool readonly_)
  : title(title_)
  , linkedVariable(linkedVariable_)
  , linkedType(GEM_VAL_SELECT)
  , type(GEM_ITEM_VAL)
  , readonly(readonly_)
  , select(&select_)
{ }

GEMItem::GEMItem(const char* title_, float& linkedVariable_, GEMSelect& select_, bool readonly_)
  : title(title_)
  , linkedVariable(&linkedVariable_)
  , linkedType(GEM_VAL_SELECT)
  , type(GEM_ITEM_VAL)
  , readonly(readonly_)
  , select(&select_)
{ }

GEMItem::GEMItem(const char* title_, double& linkedVariable_, GEMSelect& select_, bool readonly_)
  : title(title_)
  , linkedVariable(&linkedVariable_)
  , linkedType(GEM_VAL_SELECT)
  , type(GEM_ITEM_VAL)
  , readonly(readonly_)
  , select(&select_)
{ }

//---

#ifdef GEM_SUPPORT_SPINNER
GEMItem::GEMItem(const char* title_, byte& linkedVariable_, GEMSpinner& spinner_, void (*callbackAction_)())
  : title(title_)
  , linkedVariable(&linkedVariable_)
  , linkedType(GEM_VAL_SPINNER)
  , type(GEM_ITEM_VAL)
  , spinner(&spinner_)
  , callbackAction(callbackAction_)
{ }

GEMItem::GEMItem(const char* title_, int& linkedVariable_, GEMSpinner& spinner_, void (*callbackAction_)())
  : title(title_)
  , linkedVariable(&linkedVariable_)
  , linkedType(GEM_VAL_SPINNER)
  , type(GEM_ITEM_VAL)
  , spinner(&spinner_)
  , callbackAction(callbackAction_)
{ }

GEMItem::GEMItem(const char* title_, float& linkedVariable_, GEMSpinner& spinner_, void (*callbackAction_)())
  : title(title_)
  , linkedVariable(&linkedVariable_)
  , linkedType(GEM_VAL_SPINNER)
  , type(GEM_ITEM_VAL)
  , spinner(&spinner_)
  , callbackAction(callbackAction_)
{ }

GEMItem::GEMItem(const char* title_, double& linkedVariable_, GEMSpinner& spinner_, void (*callbackAction_)())
  : title(title_)
  , linkedVariable(&linkedVariable_)
  , linkedType(GEM_VAL_SPINNER)
  , type(GEM_ITEM_VAL)
  , spinner(&spinner_)
  , callbackAction(callbackAction_)
{ }

//---

GEMItem::GEMItem(const char* title_, byte& linkedVariable_, GEMSpinner& spinner_, void (*callbackAction_)(GEMCallbackData))
  : title(title_)
  , linkedVariable(&linkedVariable_)
  , linkedType(GEM_VAL_SPINNER)
  , type(GEM_ITEM_VAL)
  , spinner(&spinner_)
  , callbackActionArg(callbackAction_)
  , callbackWithArgs(true)
  , callbackData{ this, { 0 } }
{ }

GEMItem::GEMItem(const char* title_, byte& linkedVariable_, GEMSpinner& spinner_, void (*callbackAction_)(GEMCallbackData), byte callbackVal_)
  : title(title_)
  , linkedVariable(&linkedVariable_)
  , linkedType(GEM_VAL_SPINNER)
  , type(GEM_ITEM_VAL)
  , spinner(&spinner_)
  , callbackActionArg(callbackAction_)
  , callbackWithArgs(true)
  , callbackData{ this, { .valByte = callbackVal_ }}
{ }

GEMItem::GEMItem(const char* title_, byte& linkedVariable_, GEMSpinner& spinner_, void (*callbackAction_)(GEMCallbackData), int callbackVal_)
  : title(title_)
  , linkedVariable(&linkedVariable_)
  , linkedType(GEM_VAL_SPINNER)
  , type(GEM_ITEM_VAL)
  , spinner(&spinner_)
  , callbackActionArg(callbackAction_)
  , callbackWithArgs(true)
  , callbackData{ this, { .valInt = callbackVal_ }}
{ }

GEMItem::GEMItem(const char* title_, byte& linkedVariable_, GEMSpinner& spinner_, void (*callbackAction_)(GEMCallbackData), float callbackVal_)
  : title(title_)
  , linkedVariable(&linkedVariable_)
  , linkedType(GEM_VAL_SPINNER)
  , type(GEM_ITEM_VAL)
  , spinner(&spinner_)
  , callbackActionArg(callbackAction_)
  , callbackWithArgs(true)
  , callbackData{ this, { .valFloat = callbackVal_ }}
{ }

GEMItem::GEMItem(const char* title_, byte& linkedVariable_, GEMSpinner& spinner_, void (*callbackAction_)(GEMCallbackData), double callbackVal_)
  : title(title_)
  , linkedVariable(&linkedVariable_)
  , linkedType(GEM_VAL_SPINNER)
  , type(GEM_ITEM_VAL)
  , spinner(&spinner_)
  , callbackActionArg(callbackAction_)
  , callbackWithArgs(true)
  , callbackData{ this, { .valDouble = callbackVal_ }}
{ }

GEMItem::GEMItem(const char* title_, byte& linkedVariable_, GEMSpinner& spinner_, void (*callbackAction_)(GEMCallbackData), bool callbackVal_)
  : title(title_)
  , linkedVariable(&linkedVariable_)
  , linkedType(GEM_VAL_SPINNER)
  , type(GEM_ITEM_VAL)
  , spinner(&spinner_)
  , callbackActionArg(callbackAction_)
  , callbackWithArgs(true)
  , callbackData{ this, { .valBool = callbackVal_ }}
{ }

GEMItem::GEMItem(const char* title_, byte& linkedVariable_, GEMSpinner& spinner_, void (*callbackAction_)(GEMCallbackData), const char* callbackVal_)
  : title(title_)
  , linkedVariable(&linkedVariable_)
  , linkedType(GEM_VAL_SPINNER)
  , type(GEM_ITEM_VAL)
  , spinner(&spinner_)
  , callbackActionArg(callbackAction_)
  , callbackWithArgs(true)
  , callbackData{ this, { .valChar = callbackVal_ }}
{ }

GEMItem::GEMItem(const char* title_, byte& linkedVariable_, GEMSpinner& spinner_, void (*callbackAction_)(GEMCallbackData), void* callbackVal_)
  : title(title_)
  , linkedVariable(&linkedVariable_)
  , linkedType(GEM_VAL_SPINNER)
  , type(GEM_ITEM_VAL)
  , spinner(&spinner_)
  , callbackActionArg(callbackAction_)
  , callbackWithArgs(true)
  , callbackData{ this, { .valPointer = callbackVal_ }}
{ }

//---

GEMItem::GEMItem(const char* title_, int& linkedVariable_, GEMSpinner& spinner_, void (*callbackAction_)(GEMCallbackData))
  : title(title_)
  , linkedVariable(&linkedVariable_)
  , linkedType(GEM_VAL_SPINNER)
  , type(GEM_ITEM_VAL)
  , spinner(&spinner_)
  , callbackActionArg(callbackAction_)
  , callbackWithArgs(true)
  , callbackData{ this, { 0 } }
{ }

GEMItem::GEMItem(const char* title_, int& linkedVariable_, GEMSpinner& spinner_, void (*callbackAction_)(GEMCallbackData), byte callbackVal_)
  : title(title_)
  , linkedVariable(&linkedVariable_)
  , linkedType(GEM_VAL_SPINNER)
  , type(GEM_ITEM_VAL)
  , spinner(&spinner_)
  , callbackActionArg(callbackAction_)
  , callbackWithArgs(true)
  , callbackData{ this, { .valByte = callbackVal_ }}
{ }

GEMItem::GEMItem(const char* title_, int& linkedVariable_, GEMSpinner& spinner_, void (*callbackAction_)(GEMCallbackData), int callbackVal_)
  : title(title_)
  , linkedVariable(&linkedVariable_)
  , linkedType(GEM_VAL_SPINNER)
  , type(GEM_ITEM_VAL)
  , spinner(&spinner_)
  , callbackActionArg(callbackAction_)
  , callbackWithArgs(true)
  , callbackData{ this, { .valInt = callbackVal_ }}
{ }

GEMItem::GEMItem(const char* title_, int& linkedVariable_, GEMSpinner& spinner_, void (*callbackAction_)(GEMCallbackData), float callbackVal_)
  : title(title_)
  , linkedVariable(&linkedVariable_)
  , linkedType(GEM_VAL_SPINNER)
  , type(GEM_ITEM_VAL)
  , spinner(&spinner_)
  , callbackActionArg(callbackAction_)
  , callbackWithArgs(true)
  , callbackData{ this, { .valFloat = callbackVal_ }}
{ }

GEMItem::GEMItem(const char* title_, int& linkedVariable_, GEMSpinner& spinner_, void (*callbackAction_)(GEMCallbackData), double callbackVal_)
  : title(title_)
  , linkedVariable(&linkedVariable_)
  , linkedType(GEM_VAL_SPINNER)
  , type(GEM_ITEM_VAL)
  , spinner(&spinner_)
  , callbackActionArg(callbackAction_)
  , callbackWithArgs(true)
  , callbackData{ this, { .valDouble = callbackVal_ }}
{ }

GEMItem::GEMItem(const char* title_, int& linkedVariable_, GEMSpinner& spinner_, void (*callbackAction_)(GEMCallbackData), bool callbackVal_)
  : title(title_)
  , linkedVariable(&linkedVariable_)
  , linkedType(GEM_VAL_SPINNER)
  , type(GEM_ITEM_VAL)
  , spinner(&spinner_)
  , callbackActionArg(callbackAction_)
  , callbackWithArgs(true)
  , callbackData{ this, { .valBool = callbackVal_ }}
{ }

GEMItem::GEMItem(const char* title_, int& linkedVariable_, GEMSpinner& spinner_, void (*callbackAction_)(GEMCallbackData), const char* callbackVal_)
  : title(title_)
  , linkedVariable(&linkedVariable_)
  , linkedType(GEM_VAL_SPINNER)
  , type(GEM_ITEM_VAL)
  , spinner(&spinner_)
  , callbackActionArg(callbackAction_)
  , callbackWithArgs(true)
  , callbackData{ this, { .valChar = callbackVal_ }}
{ }

GEMItem::GEMItem(const char* title_, int& linkedVariable_, GEMSpinner& spinner_, void (*callbackAction_)(GEMCallbackData), void* callbackVal_)
  : title(title_)
  , linkedVariable(&linkedVariable_)
  , linkedType(GEM_VAL_SPINNER)
  , type(GEM_ITEM_VAL)
  , spinner(&spinner_)
  , callbackActionArg(callbackAction_)
  , callbackWithArgs(true)
  , callbackData{ this, { .valPointer = callbackVal_ }}
{ }

//---

GEMItem::GEMItem(const char* title_, float& linkedVariable_, GEMSpinner& spinner_, void (*callbackAction_)(GEMCallbackData))
  : title(title_)
  , linkedVariable(&linkedVariable_)
  , linkedType(GEM_VAL_SPINNER)
  , type(GEM_ITEM_VAL)
  , spinner(&spinner_)
  , callbackActionArg(callbackAction_)
  , callbackWithArgs(true)
  , callbackData{ this, { 0 } }
{ }

GEMItem::GEMItem(const char* title_, float& linkedVariable_, GEMSpinner& spinner_, void (*callbackAction_)(GEMCallbackData), byte callbackVal_)
  : title(title_)
  , linkedVariable(&linkedVariable_)
  , linkedType(GEM_VAL_SPINNER)
  , type(GEM_ITEM_VAL)
  , spinner(&spinner_)
  , callbackActionArg(callbackAction_)
  , callbackWithArgs(true)
  , callbackData{ this, { .valByte = callbackVal_ }}
{ }

GEMItem::GEMItem(const char* title_, float& linkedVariable_, GEMSpinner& spinner_, void (*callbackAction_)(GEMCallbackData), int callbackVal_)
  : title(title_)
  , linkedVariable(&linkedVariable_)
  , linkedType(GEM_VAL_SPINNER)
  , type(GEM_ITEM_VAL)
  , spinner(&spinner_)
  , callbackActionArg(callbackAction_)
  , callbackWithArgs(true)
  , callbackData{ this, { .valInt = callbackVal_ }}
{ }

GEMItem::GEMItem(const char* title_, float& linkedVariable_, GEMSpinner& spinner_, void (*callbackAction_)(GEMCallbackData), float callbackVal_)
  : title(title_)
  , linkedVariable(&linkedVariable_)
  , linkedType(GEM_VAL_SPINNER)
  , type(GEM_ITEM_VAL)
  , spinner(&spinner_)
  , callbackActionArg(callbackAction_)
  , callbackWithArgs(true)
  , callbackData{ this, { .valFloat = callbackVal_ }}
{ }

GEMItem::GEMItem(const char* title_, float& linkedVariable_, GEMSpinner& spinner_, void (*callbackAction_)(GEMCallbackData), double callbackVal_)
  : title(title_)
  , linkedVariable(&linkedVariable_)
  , linkedType(GEM_VAL_SPINNER)
  , type(GEM_ITEM_VAL)
  , spinner(&spinner_)
  , callbackActionArg(callbackAction_)
  , callbackWithArgs(true)
  , callbackData{ this, { .valDouble = callbackVal_ }}
{ }

GEMItem::GEMItem(const char* title_, float& linkedVariable_, GEMSpinner& spinner_, void (*callbackAction_)(GEMCallbackData), bool callbackVal_)
  : title(title_)
  , linkedVariable(&linkedVariable_)
  , linkedType(GEM_VAL_SPINNER)
  , type(GEM_ITEM_VAL)
  , spinner(&spinner_)
  , callbackActionArg(callbackAction_)
  , callbackWithArgs(true)
  , callbackData{ this, { .valBool = callbackVal_ }}
{ }

GEMItem::GEMItem(const char* title_, float& linkedVariable_, GEMSpinner& spinner_, void (*callbackAction_)(GEMCallbackData), const char* callbackVal_)
  : title(title_)
  , linkedVariable(&linkedVariable_)
  , linkedType(GEM_VAL_SPINNER)
  , type(GEM_ITEM_VAL)
  , spinner(&spinner_)
  , callbackActionArg(callbackAction_)
  , callbackWithArgs(true)
  , callbackData{ this, { .valChar = callbackVal_ }}
{ }

GEMItem::GEMItem(const char* title_, float& linkedVariable_, GEMSpinner& spinner_, void (*callbackAction_)(GEMCallbackData), void* callbackVal_)
  : title(title_)
  , linkedVariable(&linkedVariable_)
  , linkedType(GEM_VAL_SPINNER)
  , type(GEM_ITEM_VAL)
  , spinner(&spinner_)
  , callbackActionArg(callbackAction_)
  , callbackWithArgs(true)
  , callbackData{ this, { .valPointer = callbackVal_ }}
{ }

//---

GEMItem::GEMItem(const char* title_, double& linkedVariable_, GEMSpinner& spinner_, void (*callbackAction_)(GEMCallbackData))
  : title(title_)
  , linkedVariable(&linkedVariable_)
  , linkedType(GEM_VAL_SPINNER)
  , type(GEM_ITEM_VAL)
  , spinner(&spinner_)
  , callbackActionArg(callbackAction_)
  , callbackWithArgs(true)
  , callbackData{ this, { 0 } }
{ }

GEMItem::GEMItem(const char* title_, double& linkedVariable_, GEMSpinner& spinner_, void (*callbackAction_)(GEMCallbackData), byte callbackVal_)
  : title(title_)
  , linkedVariable(&linkedVariable_)
  , linkedType(GEM_VAL_SPINNER)
  , type(GEM_ITEM_VAL)
  , spinner(&spinner_)
  , callbackActionArg(callbackAction_)
  , callbackWithArgs(true)
  , callbackData{ this, { .valByte = callbackVal_ }}
{ }

GEMItem::GEMItem(const char* title_, double& linkedVariable_, GEMSpinner& spinner_, void (*callbackAction_)(GEMCallbackData), int callbackVal_)
  : title(title_)
  , linkedVariable(&linkedVariable_)
  , linkedType(GEM_VAL_SPINNER)
  , type(GEM_ITEM_VAL)
  , spinner(&spinner_)
  , callbackActionArg(callbackAction_)
  , callbackWithArgs(true)
  , callbackData{ this, { .valInt = callbackVal_ }}
{ }

GEMItem::GEMItem(const char* title_, double& linkedVariable_, GEMSpinner& spinner_, void (*callbackAction_)(GEMCallbackData), float callbackVal_)
  : title(title_)
  , linkedVariable(&linkedVariable_)
  , linkedType(GEM_VAL_SPINNER)
  , type(GEM_ITEM_VAL)
  , spinner(&spinner_)
  , callbackActionArg(callbackAction_)
  , callbackWithArgs(true)
  , callbackData{ this, { .valFloat = callbackVal_ }}
{ }

GEMItem::GEMItem(const char* title_, double& linkedVariable_, GEMSpinner& spinner_, void (*callbackAction_)(GEMCallbackData), double callbackVal_)
  : title(title_)
  , linkedVariable(&linkedVariable_)
  , linkedType(GEM_VAL_SPINNER)
  , type(GEM_ITEM_VAL)
  , spinner(&spinner_)
  , callbackActionArg(callbackAction_)
  , callbackWithArgs(true)
  , callbackData{ this, { .valDouble = callbackVal_ }}
{ }

GEMItem::GEMItem(const char* title_, double& linkedVariable_, GEMSpinner& spinner_, void (*callbackAction_)(GEMCallbackData), bool callbackVal_)
  : title(title_)
  , linkedVariable(&linkedVariable_)
  , linkedType(GEM_VAL_SPINNER)
  , type(GEM_ITEM_VAL)
  , spinner(&spinner_)
  , callbackActionArg(callbackAction_)
  , callbackWithArgs(true)
  , callbackData{ this, { .valBool = callbackVal_ }}
{ }

GEMItem::GEMItem(const char* title_, double& linkedVariable_, GEMSpinner& spinner_, void (*callbackAction_)(GEMCallbackData), const char* callbackVal_)
  : title(title_)
  , linkedVariable(&linkedVariable_)
  , linkedType(GEM_VAL_SPINNER)
  , type(GEM_ITEM_VAL)
  , spinner(&spinner_)
  , callbackActionArg(callbackAction_)
  , callbackWithArgs(true)
  , callbackData{ this, { .valChar = callbackVal_ }}
{ }

GEMItem::GEMItem(const char* title_, double& linkedVariable_, GEMSpinner& spinner_, void (*callbackAction_)(GEMCallbackData), void* callbackVal_)
  : title(title_)
  , linkedVariable(&linkedVariable_)
  , linkedType(GEM_VAL_SPINNER)
  , type(GEM_ITEM_VAL)
  , spinner(&spinner_)
  , callbackActionArg(callbackAction_)
  , callbackWithArgs(true)
  , callbackData{ this, { .valPointer = callbackVal_ }}
{ }

//---

GEMItem::GEMItem(const char* title_, byte& linkedVariable_, GEMSpinner& spinner_, bool readonly_)
  : title(title_)
  , linkedVariable(&linkedVariable_)
  , linkedType(GEM_VAL_SPINNER)
  , type(GEM_ITEM_VAL)
  , readonly(readonly_)
  , spinner(&spinner_)
{ }

GEMItem::GEMItem(const char* title_, int& linkedVariable_, GEMSpinner& spinner_, bool readonly_)
  : title(title_)
  , linkedVariable(&linkedVariable_)
  , linkedType(GEM_VAL_SPINNER)
  , type(GEM_ITEM_VAL)
  , readonly(readonly_)
  , spinner(&spinner_)
{ }

GEMItem::GEMItem(const char* title_, float& linkedVariable_, GEMSpinner& spinner_, bool readonly_)
  : title(title_)
  , linkedVariable(&linkedVariable_)
  , linkedType(GEM_VAL_SPINNER)
  , type(GEM_ITEM_VAL)
  , readonly(readonly_)
  , spinner(&spinner_)
{ }

GEMItem::GEMItem(const char* title_, double& linkedVariable_, GEMSpinner& spinner_, bool readonly_)
  : title(title_)
  , linkedVariable(&linkedVariable_)
  , linkedType(GEM_VAL_SPINNER)
  , type(GEM_ITEM_VAL)
  , readonly(readonly_)
  , spinner(&spinner_)
{ }
#endif

//---

GEMItem::GEMItem(const char* title_, byte& linkedVariable_, void (*callbackAction_)())
  : title(title_)
  , linkedVariable(&linkedVariable_)
  , linkedType(GEM_VAL_BYTE)
  , type(GEM_ITEM_VAL)
  , callbackAction(callbackAction_)
{ }

GEMItem::GEMItem(const char* title_, int& linkedVariable_, void (*callbackAction_)())
  : title(title_)
  , linkedVariable(&linkedVariable_)
  , linkedType(GEM_VAL_INTEGER)
  , type(GEM_ITEM_VAL)
  , callbackAction(callbackAction_)
{ }

GEMItem::GEMItem(const char* title_, char* linkedVariable_, void (*callbackAction_)())
  : title(title_)
  , linkedVariable(linkedVariable_)
  , linkedType(GEM_VAL_CHAR)
  , type(GEM_ITEM_VAL)
  , callbackAction(callbackAction_)
{ }

GEMItem::GEMItem(const char* title_, bool& linkedVariable_, void (*callbackAction_)())
  : title(title_)
  , linkedVariable(&linkedVariable_)
  , linkedType(GEM_VAL_BOOL)
  , type(GEM_ITEM_VAL)
  , callbackAction(callbackAction_)
{ }

GEMItem::GEMItem(const char* title_, float& linkedVariable_, void (*callbackAction_)())
  : title(title_)
  , linkedVariable(&linkedVariable_)
  , linkedType(GEM_VAL_FLOAT)
  , type(GEM_ITEM_VAL)
  , precision(GEM_FLOAT_PREC)
  , callbackAction(callbackAction_)
{ }

GEMItem::GEMItem(const char* title_, double& linkedVariable_, void (*callbackAction_)())
  : title(title_)
  , linkedVariable(&linkedVariable_)
  , linkedType(GEM_VAL_DOUBLE)
  , type(GEM_ITEM_VAL)
  , precision(GEM_DOUBLE_PREC)
  , callbackAction(callbackAction_)
{ }

//---

GEMItem::GEMItem(const char* title_, byte& linkedVariable_, void (*callbackAction_)(GEMCallbackData))
  : title(title_)
  , linkedVariable(&linkedVariable_)
  , linkedType(GEM_VAL_BYTE)
  , type(GEM_ITEM_VAL)
  , callbackActionArg(callbackAction_)
  , callbackWithArgs(true)
  , callbackData{ this, { 0 } }
{ }

GEMItem::GEMItem(const char* title_, byte& linkedVariable_, void (*callbackAction_)(GEMCallbackData), byte callbackVal_)
  : title(title_)
  , linkedVariable(&linkedVariable_)
  , linkedType(GEM_VAL_BYTE)
  , type(GEM_ITEM_VAL)
  , callbackActionArg(callbackAction_)
  , callbackWithArgs(true)
  , callbackData{ this, { .valByte = callbackVal_ }}
{ }

GEMItem::GEMItem(const char* title_, byte& linkedVariable_, void (*callbackAction_)(GEMCallbackData), int callbackVal_)
  : title(title_)
  , linkedVariable(&linkedVariable_)
  , linkedType(GEM_VAL_BYTE)
  , type(GEM_ITEM_VAL)
  , callbackActionArg(callbackAction_)
  , callbackWithArgs(true)
  , callbackData{ this, { .valInt = callbackVal_ }}
{ }

GEMItem::GEMItem(const char* title_, byte& linkedVariable_, void (*callbackAction_)(GEMCallbackData), float callbackVal_)
  : title(title_)
  , linkedVariable(&linkedVariable_)
  , linkedType(GEM_VAL_BYTE)
  , type(GEM_ITEM_VAL)
  , callbackActionArg(callbackAction_)
  , callbackWithArgs(true)
  , callbackData{ this, { .valFloat = callbackVal_ }}
{ }

GEMItem::GEMItem(const char* title_, byte& linkedVariable_, void (*callbackAction_)(GEMCallbackData), double callbackVal_)
  : title(title_)
  , linkedVariable(&linkedVariable_)
  , linkedType(GEM_VAL_BYTE)
  , type(GEM_ITEM_VAL)
  , callbackActionArg(callbackAction_)
  , callbackWithArgs(true)
  , callbackData{ this, { .valDouble = callbackVal_ }}
{ }

GEMItem::GEMItem(const char* title_, byte& linkedVariable_, void (*callbackAction_)(GEMCallbackData), bool callbackVal_)
  : title(title_)
  , linkedVariable(&linkedVariable_)
  , linkedType(GEM_VAL_BYTE)
  , type(GEM_ITEM_VAL)
  , callbackActionArg(callbackAction_)
  , callbackWithArgs(true)
  , callbackData{ this, { .valBool = callbackVal_ }}
{ }

GEMItem::GEMItem(const char* title_, byte& linkedVariable_, void (*callbackAction_)(GEMCallbackData), const char* callbackVal_)
  : title(title_)
  , linkedVariable(&linkedVariable_)
  , linkedType(GEM_VAL_BYTE)
  , type(GEM_ITEM_VAL)
  , callbackActionArg(callbackAction_)
  , callbackWithArgs(true)
  , callbackData{ this, { .valChar = callbackVal_ }}
{ }

GEMItem::GEMItem(const char* title_, byte& linkedVariable_, void (*callbackAction_)(GEMCallbackData), void* callbackVal_)
  : title(title_)
  , linkedVariable(&linkedVariable_)
  , linkedType(GEM_VAL_BYTE)
  , type(GEM_ITEM_VAL)
  , callbackActionArg(callbackAction_)
  , callbackWithArgs(true)
  , callbackData{ this, { .valPointer = callbackVal_ }}
{ }

//---

GEMItem::GEMItem(const char* title_, int& linkedVariable_, void (*callbackAction_)(GEMCallbackData))
  : title(title_)
  , linkedVariable(&linkedVariable_)
  , linkedType(GEM_VAL_INTEGER)
  , type(GEM_ITEM_VAL)
  , callbackActionArg(callbackAction_)
  , callbackWithArgs(true)
  , callbackData{ this, { 0 } }
{ }

GEMItem::GEMItem(const char* title_, int& linkedVariable_, void (*callbackAction_)(GEMCallbackData), byte callbackVal_)
  : title(title_)
  , linkedVariable(&linkedVariable_)
  , linkedType(GEM_VAL_INTEGER)
  , type(GEM_ITEM_VAL)
  , callbackActionArg(callbackAction_)
  , callbackWithArgs(true)
  , callbackData{ this, { .valByte = callbackVal_ }}
{ }

GEMItem::GEMItem(const char* title_, int& linkedVariable_, void (*callbackAction_)(GEMCallbackData), int callbackVal_)
  : title(title_)
  , linkedVariable(&linkedVariable_)
  , linkedType(GEM_VAL_INTEGER)
  , type(GEM_ITEM_VAL)
  , callbackActionArg(callbackAction_)
  , callbackWithArgs(true)
  , callbackData{ this, { .valInt = callbackVal_ }}
{ }

GEMItem::GEMItem(const char* title_, int& linkedVariable_, void (*callbackAction_)(GEMCallbackData), float callbackVal_)
  : title(title_)
  , linkedVariable(&linkedVariable_)
  , linkedType(GEM_VAL_INTEGER)
  , type(GEM_ITEM_VAL)
  , callbackActionArg(callbackAction_)
  , callbackWithArgs(true)
  , callbackData{ this, { .valFloat = callbackVal_ }}
{ }

GEMItem::GEMItem(const char* title_, int& linkedVariable_, void (*callbackAction_)(GEMCallbackData), double callbackVal_)
  : title(title_)
  , linkedVariable(&linkedVariable_)
  , linkedType(GEM_VAL_INTEGER)
  , type(GEM_ITEM_VAL)
  , callbackActionArg(callbackAction_)
  , callbackWithArgs(true)
  , callbackData{ this, { .valDouble = callbackVal_ }}
{ }

GEMItem::GEMItem(const char* title_, int& linkedVariable_, void (*callbackAction_)(GEMCallbackData), bool callbackVal_)
  : title(title_)
  , linkedVariable(&linkedVariable_)
  , linkedType(GEM_VAL_INTEGER)
  , type(GEM_ITEM_VAL)
  , callbackActionArg(callbackAction_)
  , callbackWithArgs(true)
  , callbackData{ this, { .valBool = callbackVal_ }}
{ }

GEMItem::GEMItem(const char* title_, int& linkedVariable_, void (*callbackAction_)(GEMCallbackData), const char* callbackVal_)
  : title(title_)
  , linkedVariable(&linkedVariable_)
  , linkedType(GEM_VAL_INTEGER)
  , type(GEM_ITEM_VAL)
  , callbackActionArg(callbackAction_)
  , callbackWithArgs(true)
  , callbackData{ this, { .valChar = callbackVal_ }}
{ }

GEMItem::GEMItem(const char* title_, int& linkedVariable_, void (*callbackAction_)(GEMCallbackData), void* callbackVal_)
  : title(title_)
  , linkedVariable(&linkedVariable_)
  , linkedType(GEM_VAL_INTEGER)
  , type(GEM_ITEM_VAL)
  , callbackActionArg(callbackAction_)
  , callbackWithArgs(true)
  , callbackData{ this, { .valPointer = callbackVal_ }}
{ }

//---

GEMItem::GEMItem(const char* title_, char* linkedVariable_, void (*callbackAction_)(GEMCallbackData))
  : title(title_)
  , linkedVariable(&linkedVariable_)
  , linkedType(GEM_VAL_CHAR)
  , type(GEM_ITEM_VAL)
  , callbackActionArg(callbackAction_)
  , callbackWithArgs(true)
  , callbackData{ this, { 0 } }
{ }

GEMItem::GEMItem(const char* title_, char* linkedVariable_, void (*callbackAction_)(GEMCallbackData), byte callbackVal_)
  : title(title_)
  , linkedVariable(&linkedVariable_)
  , linkedType(GEM_VAL_CHAR)
  , type(GEM_ITEM_VAL)
  , callbackActionArg(callbackAction_)
  , callbackWithArgs(true)
  , callbackData{ this, { .valByte = callbackVal_ }}
{ }

GEMItem::GEMItem(const char* title_, char* linkedVariable_, void (*callbackAction_)(GEMCallbackData), int callbackVal_)
  : title(title_)
  , linkedVariable(&linkedVariable_)
  , linkedType(GEM_VAL_CHAR)
  , type(GEM_ITEM_VAL)
  , callbackActionArg(callbackAction_)
  , callbackWithArgs(true)
  , callbackData{ this, { .valInt = callbackVal_ }}
{ }

GEMItem::GEMItem(const char* title_, char* linkedVariable_, void (*callbackAction_)(GEMCallbackData), float callbackVal_)
  : title(title_)
  , linkedVariable(&linkedVariable_)
  , linkedType(GEM_VAL_CHAR)
  , type(GEM_ITEM_VAL)
  , callbackActionArg(callbackAction_)
  , callbackWithArgs(true)
  , callbackData{ this, { .valFloat = callbackVal_ }}
{ }

GEMItem::GEMItem(const char* title_, char* linkedVariable_, void (*callbackAction_)(GEMCallbackData), double callbackVal_)
  : title(title_)
  , linkedVariable(&linkedVariable_)
  , linkedType(GEM_VAL_CHAR)
  , type(GEM_ITEM_VAL)
  , callbackActionArg(callbackAction_)
  , callbackWithArgs(true)
  , callbackData{ this, { .valDouble = callbackVal_ }}
{ }

GEMItem::GEMItem(const char* title_, char* linkedVariable_, void (*callbackAction_)(GEMCallbackData), bool callbackVal_)
  : title(title_)
  , linkedVariable(&linkedVariable_)
  , linkedType(GEM_VAL_CHAR)
  , type(GEM_ITEM_VAL)
  , callbackActionArg(callbackAction_)
  , callbackWithArgs(true)
  , callbackData{ this, { .valBool = callbackVal_ }}
{ }

GEMItem::GEMItem(const char* title_, char* linkedVariable_, void (*callbackAction_)(GEMCallbackData), const char* callbackVal_)
  : title(title_)
  , linkedVariable(&linkedVariable_)
  , linkedType(GEM_VAL_CHAR)
  , type(GEM_ITEM_VAL)
  , callbackActionArg(callbackAction_)
  , callbackWithArgs(true)
  , callbackData{ this, { .valChar = callbackVal_ }}
{ }

GEMItem::GEMItem(const char* title_, char* linkedVariable_, void (*callbackAction_)(GEMCallbackData), void* callbackVal_)
  : title(title_)
  , linkedVariable(&linkedVariable_)
  , linkedType(GEM_VAL_CHAR)
  , type(GEM_ITEM_VAL)
  , callbackActionArg(callbackAction_)
  , callbackWithArgs(true)
  , callbackData{ this, { .valPointer = callbackVal_ }}
{ }

//---

GEMItem::GEMItem(const char* title_, bool& linkedVariable_, void (*callbackAction_)(GEMCallbackData))
  : title(title_)
  , linkedVariable(&linkedVariable_)
  , linkedType(GEM_VAL_BOOL)
  , type(GEM_ITEM_VAL)
  , callbackActionArg(callbackAction_)
  , callbackWithArgs(true)
  , callbackData{ this, { 0 } }
{ }

GEMItem::GEMItem(const char* title_, bool& linkedVariable_, void (*callbackAction_)(GEMCallbackData), byte callbackVal_)
  : title(title_)
  , linkedVariable(&linkedVariable_)
  , linkedType(GEM_VAL_BOOL)
  , type(GEM_ITEM_VAL)
  , callbackActionArg(callbackAction_)
  , callbackWithArgs(true)
  , callbackData{ this, { .valByte = callbackVal_ }}
{ }

GEMItem::GEMItem(const char* title_, bool& linkedVariable_, void (*callbackAction_)(GEMCallbackData), int callbackVal_)
  : title(title_)
  , linkedVariable(&linkedVariable_)
  , linkedType(GEM_VAL_BOOL)
  , type(GEM_ITEM_VAL)
  , callbackActionArg(callbackAction_)
  , callbackWithArgs(true)
  , callbackData{ this, { .valInt = callbackVal_ }}
{ }

GEMItem::GEMItem(const char* title_, bool& linkedVariable_, void (*callbackAction_)(GEMCallbackData), float callbackVal_)
  : title(title_)
  , linkedVariable(&linkedVariable_)
  , linkedType(GEM_VAL_BOOL)
  , type(GEM_ITEM_VAL)
  , callbackActionArg(callbackAction_)
  , callbackWithArgs(true)
  , callbackData{ this, { .valFloat = callbackVal_ }}
{ }

GEMItem::GEMItem(const char* title_, bool& linkedVariable_, void (*callbackAction_)(GEMCallbackData), double callbackVal_)
  : title(title_)
  , linkedVariable(&linkedVariable_)
  , linkedType(GEM_VAL_BOOL)
  , type(GEM_ITEM_VAL)
  , callbackActionArg(callbackAction_)
  , callbackWithArgs(true)
  , callbackData{ this, { .valDouble = callbackVal_ }}
{ }

GEMItem::GEMItem(const char* title_, bool& linkedVariable_, void (*callbackAction_)(GEMCallbackData), bool callbackVal_)
  : title(title_)
  , linkedVariable(&linkedVariable_)
  , linkedType(GEM_VAL_BOOL)
  , type(GEM_ITEM_VAL)
  , callbackActionArg(callbackAction_)
  , callbackWithArgs(true)
  , callbackData{ this, { .valBool = callbackVal_ }}
{ }

GEMItem::GEMItem(const char* title_, bool& linkedVariable_, void (*callbackAction_)(GEMCallbackData), const char* callbackVal_)
  : title(title_)
  , linkedVariable(&linkedVariable_)
  , linkedType(GEM_VAL_BOOL)
  , type(GEM_ITEM_VAL)
  , callbackActionArg(callbackAction_)
  , callbackWithArgs(true)
  , callbackData{ this, { .valChar = callbackVal_ }}
{ }

GEMItem::GEMItem(const char* title_, bool& linkedVariable_, void (*callbackAction_)(GEMCallbackData), void* callbackVal_)
  : title(title_)
  , linkedVariable(&linkedVariable_)
  , linkedType(GEM_VAL_BOOL)
  , type(GEM_ITEM_VAL)
  , callbackActionArg(callbackAction_)
  , callbackWithArgs(true)
  , callbackData{ this, { .valPointer = callbackVal_ }}
{ }

//---

GEMItem::GEMItem(const char* title_, float& linkedVariable_, void (*callbackAction_)(GEMCallbackData))
  : title(title_)
  , linkedVariable(&linkedVariable_)
  , linkedType(GEM_VAL_FLOAT)
  , type(GEM_ITEM_VAL)
  , callbackActionArg(callbackAction_)
  , callbackWithArgs(true)
  , callbackData{ this, { 0 } }
{ }

GEMItem::GEMItem(const char* title_, float& linkedVariable_, void (*callbackAction_)(GEMCallbackData), byte callbackVal_)
  : title(title_)
  , linkedVariable(&linkedVariable_)
  , linkedType(GEM_VAL_FLOAT)
  , type(GEM_ITEM_VAL)
  , callbackActionArg(callbackAction_)
  , callbackWithArgs(true)
  , callbackData{ this, { .valByte = callbackVal_ }}
{ }

GEMItem::GEMItem(const char* title_, float& linkedVariable_, void (*callbackAction_)(GEMCallbackData), int callbackVal_)
  : title(title_)
  , linkedVariable(&linkedVariable_)
  , linkedType(GEM_VAL_FLOAT)
  , type(GEM_ITEM_VAL)
  , callbackActionArg(callbackAction_)
  , callbackWithArgs(true)
  , callbackData{ this, { .valInt = callbackVal_ }}
{ }

GEMItem::GEMItem(const char* title_, float& linkedVariable_, void (*callbackAction_)(GEMCallbackData), float callbackVal_)
  : title(title_)
  , linkedVariable(&linkedVariable_)
  , linkedType(GEM_VAL_FLOAT)
  , type(GEM_ITEM_VAL)
  , callbackActionArg(callbackAction_)
  , callbackWithArgs(true)
  , callbackData{ this, { .valFloat = callbackVal_ }}
{ }

GEMItem::GEMItem(const char* title_, float& linkedVariable_, void (*callbackAction_)(GEMCallbackData), double callbackVal_)
  : title(title_)
  , linkedVariable(&linkedVariable_)
  , linkedType(GEM_VAL_FLOAT)
  , type(GEM_ITEM_VAL)
  , callbackActionArg(callbackAction_)
  , callbackWithArgs(true)
  , callbackData{ this, { .valDouble = callbackVal_ }}
{ }

GEMItem::GEMItem(const char* title_, float& linkedVariable_, void (*callbackAction_)(GEMCallbackData), bool callbackVal_)
  : title(title_)
  , linkedVariable(&linkedVariable_)
  , linkedType(GEM_VAL_FLOAT)
  , type(GEM_ITEM_VAL)
  , callbackActionArg(callbackAction_)
  , callbackWithArgs(true)
  , callbackData{ this, { .valBool = callbackVal_ }}
{ }

GEMItem::GEMItem(const char* title_, float& linkedVariable_, void (*callbackAction_)(GEMCallbackData), const char* callbackVal_)
  : title(title_)
  , linkedVariable(&linkedVariable_)
  , linkedType(GEM_VAL_FLOAT)
  , type(GEM_ITEM_VAL)
  , callbackActionArg(callbackAction_)
  , callbackWithArgs(true)
  , callbackData{ this, { .valChar = callbackVal_ }}
{ }

GEMItem::GEMItem(const char* title_, float& linkedVariable_, void (*callbackAction_)(GEMCallbackData), void* callbackVal_)
  : title(title_)
  , linkedVariable(&linkedVariable_)
  , linkedType(GEM_VAL_FLOAT)
  , type(GEM_ITEM_VAL)
  , callbackActionArg(callbackAction_)
  , callbackWithArgs(true)
  , callbackData{ this, { .valPointer = callbackVal_ }}
{ }

//---

GEMItem::GEMItem(const char* title_, double& linkedVariable_, void (*callbackAction_)(GEMCallbackData))
  : title(title_)
  , linkedVariable(&linkedVariable_)
  , linkedType(GEM_VAL_DOUBLE)
  , type(GEM_ITEM_VAL)
  , callbackActionArg(callbackAction_)
  , callbackWithArgs(true)
  , callbackData{ this, { 0 } }
{ }

GEMItem::GEMItem(const char* title_, double& linkedVariable_, void (*callbackAction_)(GEMCallbackData), byte callbackVal_)
  : title(title_)
  , linkedVariable(&linkedVariable_)
  , linkedType(GEM_VAL_DOUBLE)
  , type(GEM_ITEM_VAL)
  , callbackActionArg(callbackAction_)
  , callbackWithArgs(true)
  , callbackData{ this, { .valByte = callbackVal_ }}
{ }

GEMItem::GEMItem(const char* title_, double& linkedVariable_, void (*callbackAction_)(GEMCallbackData), int callbackVal_)
  : title(title_)
  , linkedVariable(&linkedVariable_)
  , linkedType(GEM_VAL_DOUBLE)
  , type(GEM_ITEM_VAL)
  , callbackActionArg(callbackAction_)
  , callbackWithArgs(true)
  , callbackData{ this, { .valInt = callbackVal_ }}
{ }

GEMItem::GEMItem(const char* title_, double& linkedVariable_, void (*callbackAction_)(GEMCallbackData), float callbackVal_)
  : title(title_)
  , linkedVariable(&linkedVariable_)
  , linkedType(GEM_VAL_DOUBLE)
  , type(GEM_ITEM_VAL)
  , callbackActionArg(callbackAction_)
  , callbackWithArgs(true)
  , callbackData{ this, { .valFloat = callbackVal_ }}
{ }

GEMItem::GEMItem(const char* title_, double& linkedVariable_, void (*callbackAction_)(GEMCallbackData), double callbackVal_)
  : title(title_)
  , linkedVariable(&linkedVariable_)
  , linkedType(GEM_VAL_DOUBLE)
  , type(GEM_ITEM_VAL)
  , callbackActionArg(callbackAction_)
  , callbackWithArgs(true)
  , callbackData{ this, { .valDouble = callbackVal_ }}
{ }

GEMItem::GEMItem(const char* title_, double& linkedVariable_, void (*callbackAction_)(GEMCallbackData), bool callbackVal_)
  : title(title_)
  , linkedVariable(&linkedVariable_)
  , linkedType(GEM_VAL_DOUBLE)
  , type(GEM_ITEM_VAL)
  , callbackActionArg(callbackAction_)
  , callbackWithArgs(true)
  , callbackData{ this, { .valBool = callbackVal_ }}
{ }

GEMItem::GEMItem(const char* title_, double& linkedVariable_, void (*callbackAction_)(GEMCallbackData), const char* callbackVal_)
  : title(title_)
  , linkedVariable(&linkedVariable_)
  , linkedType(GEM_VAL_DOUBLE)
  , type(GEM_ITEM_VAL)
  , callbackActionArg(callbackAction_)
  , callbackWithArgs(true)
  , callbackData{ this, { .valChar = callbackVal_ }}
{ }

GEMItem::GEMItem(const char* title_, double& linkedVariable_, void (*callbackAction_)(GEMCallbackData), void* callbackVal_)
  : title(title_)
  , linkedVariable(&linkedVariable_)
  , linkedType(GEM_VAL_DOUBLE)
  , type(GEM_ITEM_VAL)
  , callbackActionArg(callbackAction_)
  , callbackWithArgs(true)
  , callbackData{ this, { .valPointer = callbackVal_ }}
{ }

//---

GEMItem::GEMItem(const char* title_, byte& linkedVariable_, bool readonly_)
  : title(title_)
  , linkedVariable(&linkedVariable_)
  , linkedType(GEM_VAL_BYTE)
  , type(GEM_ITEM_VAL)
  , readonly(readonly_)
{ }

GEMItem::GEMItem(const char* title_, int& linkedVariable_, bool readonly_)
  : title(title_)
  , linkedVariable(&linkedVariable_)
  , linkedType(GEM_VAL_INTEGER)
  , type(GEM_ITEM_VAL)
  , readonly(readonly_)
{ }

GEMItem::GEMItem(const char* title_, char* linkedVariable_, bool readonly_)
  : title(title_)
  , linkedVariable(linkedVariable_)
  , linkedType(GEM_VAL_CHAR)
  , type(GEM_ITEM_VAL)
  , readonly(readonly_)
{ }

GEMItem::GEMItem(const char* title_, bool& linkedVariable_, bool readonly_)
  : title(title_)
  , linkedVariable(&linkedVariable_)
  , linkedType(GEM_VAL_BOOL)
  , type(GEM_ITEM_VAL)
  , readonly(readonly_)
{ }

GEMItem::GEMItem(const char* title_, float& linkedVariable_, bool readonly_)
  : title(title_)
  , linkedVariable(&linkedVariable_)
  , linkedType(GEM_VAL_FLOAT)
  , type(GEM_ITEM_VAL)
  , precision(GEM_FLOAT_PREC)
  , readonly(readonly_)
{ }

GEMItem::GEMItem(const char* title_, double& linkedVariable_, bool readonly_)
  : title(title_)
  , linkedVariable(&linkedVariable_)
  , linkedType(GEM_VAL_DOUBLE)
  , type(GEM_ITEM_VAL)
  , precision(GEM_DOUBLE_PREC)
  , readonly(readonly_)
{ }

//---

GEMItem::GEMItem(const char* title_, GEMPage& linkedPage_, bool readonly_)
  : title(title_)
  , type(GEM_ITEM_LINK)
  , readonly(readonly_)
  , linkedPage(&linkedPage_)
{ }

GEMItem::GEMItem(const char* title_, GEMPage* linkedPage_, bool readonly_)
  : title(title_)
  , type(GEM_ITEM_LINK)
  , readonly(readonly_)
  , linkedPage(linkedPage_)
{ }

GEMItem::GEMItem(const char* title_, void (*callbackAction_)(), bool readonly_)
  : title(title_)
  , type(GEM_ITEM_BUTTON)
  , readonly(readonly_)
  , callbackAction(callbackAction_)
{ }

GEMItem::GEMItem(const char* title_, void (*callbackAction_)(GEMCallbackData))
  : title(title_)
  , type(GEM_ITEM_BUTTON)
  , callbackActionArg(callbackAction_)
  , callbackWithArgs(true)
  , callbackData{ this, { 0 } }
{ }

GEMItem::GEMItem(const char* title_, void (*callbackAction_)(GEMCallbackData), byte callbackVal_, bool readonly_)
  : title(title_)
  , type(GEM_ITEM_BUTTON)
  , readonly(readonly_)
  , callbackActionArg(callbackAction_)
  , callbackWithArgs(true)
  , callbackData{ this, { .valByte = callbackVal_ }}
{ }

GEMItem::GEMItem(const char* title_, void (*callbackAction_)(GEMCallbackData), int callbackVal_, bool readonly_)
  : title(title_)
  , type(GEM_ITEM_BUTTON)
  , readonly(readonly_)
  , callbackActionArg(callbackAction_)
  , callbackWithArgs(true)
  , callbackData{ this, { .valInt = callbackVal_ }}
{ }

GEMItem::GEMItem(const char* title_, void (*callbackAction_)(GEMCallbackData), float callbackVal_, bool readonly_)
  : title(title_)
  , type(GEM_ITEM_BUTTON)
  , readonly(readonly_)
  , callbackActionArg(callbackAction_)
  , callbackWithArgs(true)
  , callbackData{ this, { .valFloat = callbackVal_ }}
{ }

GEMItem::GEMItem(const char* title_, void (*callbackAction_)(GEMCallbackData), double callbackVal_, bool readonly_)
  : title(title_)
  , type(GEM_ITEM_BUTTON)
  , readonly(readonly_)
  , callbackActionArg(callbackAction_)
  , callbackWithArgs(true)
  , callbackData{ this, { .valDouble = callbackVal_ }}
{ }

GEMItem::GEMItem(const char* title_, void (*callbackAction_)(GEMCallbackData), bool callbackVal_, bool readonly_)
  : title(title_)
  , type(GEM_ITEM_BUTTON)
  , readonly(readonly_)
  , callbackActionArg(callbackAction_)
  , callbackWithArgs(true)
  , callbackData{ this, { .valBool = callbackVal_ }}
{ }

GEMItem::GEMItem(const char* title_, void (*callbackAction_)(GEMCallbackData), const char* callbackVal_, bool readonly_)
  : title(title_)
  , type(GEM_ITEM_BUTTON)
  , readonly(readonly_)
  , callbackActionArg(callbackAction_)
  , callbackWithArgs(true)
  , callbackData{ this, { .valChar = callbackVal_ }}
{ }

GEMItem::GEMItem(const char* title_, void (*callbackAction_)(GEMCallbackData), void* callbackVal_, bool readonly_)
  : title(title_)
  , type(GEM_ITEM_BUTTON)
  , readonly(readonly_)
  , callbackActionArg(callbackAction_)
  , callbackWithArgs(true)
  , callbackData{ this, { .valPointer = callbackVal_ }}
{ }

//---

GEMItem& GEMItem::setCallbackVal(byte callbackVal_) {
  callbackData.pMenuItem = this;
  callbackData.valByte = callbackVal_;
  return *this;
}

GEMItem& GEMItem::setCallbackVal(int callbackVal_) {
  callbackData.pMenuItem = this;
  callbackData.valInt = callbackVal_;
  return *this;
}

GEMItem& GEMItem::setCallbackVal(float callbackVal_) {
  callbackData.pMenuItem = this;
  callbackData.valFloat = callbackVal_;
  return *this;
}

GEMItem& GEMItem::setCallbackVal(double callbackVal_) {
  callbackData.pMenuItem = this;
  callbackData.valDouble = callbackVal_;
  return *this;
}

GEMItem& GEMItem::setCallbackVal(bool callbackVal_) {
  callbackData.pMenuItem = this;
  callbackData.valBool = callbackVal_;
  return *this;
}

GEMItem& GEMItem::setCallbackVal(const char* callbackVal_) {
  callbackData.pMenuItem = this;
  callbackData.valChar = callbackVal_;
  return *this;
}

GEMItem& GEMItem::setCallbackVal(void* callbackVal_) {
  callbackData.pMenuItem = this;
  callbackData.valPointer = callbackVal_;
  return *this;
}

GEMCallbackData GEMItem::getCallbackData() {
  return callbackData;
}

//---

GEMItem& GEMItem::setTitle(const char* title_) {
  title = title_;
  return *this;
}

const char* GEMItem::getTitle() {
  return title;
}

byte GEMItem::getLinkedType() {
  return linkedType;
}

byte GEMItem::getType() {
  return type;
}

GEMItem& GEMItem::setPrecision(byte prec) {
  precision = prec;
  return *this;
}

GEMItem& GEMItem::setAdjustedASCIIOrder(bool mode) {
  adjustedAsciiOrder = mode;
  return *this;
}

GEMItem& GEMItem::setReadonly(bool mode) {
  readonly = mode;
  return *this;
}

bool GEMItem::getReadonly() {
  return readonly;
}

GEMItem& GEMItem::hide(bool hide) {
  if (hide) {
    if (!hidden) {
      if (parentPage != nullptr) {
        parentPage->hideMenuItem(*this);
      } else {
        hidden = true;
      }
    }
  } else {
    show();
  }
  return *this;
}

GEMItem& GEMItem::show() {
  if (hidden) {
    if (parentPage != nullptr) {
      parentPage->showMenuItem(*this);
    } else {
      hidden = false;
    }
  }
  return *this;
}

bool GEMItem::getHidden() {
  return hidden;
}

GEMItem& GEMItem::remove() {
  if (parentPage != nullptr) {
    parentPage->removeMenuItem(*this);
  }
  return *this;
}

void* GEMItem::getLinkedVariablePointer() {
  return linkedVariable;
}

GEMPage* GEMItem::getParentPage() {
  return parentPage;
}

GEMPage* GEMItem::getLinkedPage() {
  return linkedPage;
}

GEMItem* GEMItem::getMenuItemNext(bool total) {
  GEMItem* menuItemTmp = menuItemNext;
  while (menuItemTmp != nullptr && !total && menuItemTmp->hidden) {
    menuItemTmp = menuItemTmp->menuItemNext;
  }
  return menuItemTmp;
}
