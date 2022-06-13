/*
  GEMItem - menu item for GEM library.

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
#include "GEMItem.h"
#include "constants.h"

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
  , callbackData{ .pMenuItem = this }
{ }

GEMItem::GEMItem(const char* title_, byte& linkedVariable_, GEMSelect& select_, void (*callbackAction_)(GEMCallbackData), byte callbackVal_)
  : title(title_)
  , linkedVariable(&linkedVariable_)
  , linkedType(GEM_VAL_SELECT)
  , type(GEM_ITEM_VAL)
  , select(&select_)
  , callbackActionArg(callbackAction_)
  , callbackWithArgs(true)
  , callbackData{ .pMenuItem = this, { .valByte = callbackVal_ }}
{ }

GEMItem::GEMItem(const char* title_, byte& linkedVariable_, GEMSelect& select_, void (*callbackAction_)(GEMCallbackData), int callbackVal_)
  : title(title_)
  , linkedVariable(&linkedVariable_)
  , linkedType(GEM_VAL_SELECT)
  , type(GEM_ITEM_VAL)
  , select(&select_)
  , callbackActionArg(callbackAction_)
  , callbackWithArgs(true)
  , callbackData{ .pMenuItem = this, { .valInt = callbackVal_ }}
{ }

GEMItem::GEMItem(const char* title_, byte& linkedVariable_, GEMSelect& select_, void (*callbackAction_)(GEMCallbackData), float callbackVal_)
  : title(title_)
  , linkedVariable(&linkedVariable_)
  , linkedType(GEM_VAL_SELECT)
  , type(GEM_ITEM_VAL)
  , select(&select_)
  , callbackActionArg(callbackAction_)
  , callbackWithArgs(true)
  , callbackData{ .pMenuItem = this, { .valFloat = callbackVal_ }}
{ }

GEMItem::GEMItem(const char* title_, byte& linkedVariable_, GEMSelect& select_, void (*callbackAction_)(GEMCallbackData), double callbackVal_)
  : title(title_)
  , linkedVariable(&linkedVariable_)
  , linkedType(GEM_VAL_SELECT)
  , type(GEM_ITEM_VAL)
  , select(&select_)
  , callbackActionArg(callbackAction_)
  , callbackWithArgs(true)
  , callbackData{ .pMenuItem = this, { .valDouble = callbackVal_ }}
{ }

GEMItem::GEMItem(const char* title_, byte& linkedVariable_, GEMSelect& select_, void (*callbackAction_)(GEMCallbackData), boolean callbackVal_)
  : title(title_)
  , linkedVariable(&linkedVariable_)
  , linkedType(GEM_VAL_SELECT)
  , type(GEM_ITEM_VAL)
  , select(&select_)
  , callbackActionArg(callbackAction_)
  , callbackWithArgs(true)
  , callbackData{ .pMenuItem = this, { .valBool = callbackVal_ }}
{ }

GEMItem::GEMItem(const char* title_, byte& linkedVariable_, GEMSelect& select_, void (*callbackAction_)(GEMCallbackData), const char* callbackVal_)
  : title(title_)
  , linkedVariable(&linkedVariable_)
  , linkedType(GEM_VAL_SELECT)
  , type(GEM_ITEM_VAL)
  , select(&select_)
  , callbackActionArg(callbackAction_)
  , callbackWithArgs(true)
  , callbackData{ .pMenuItem = this, { .valChar = callbackVal_ }}
{ }

GEMItem::GEMItem(const char* title_, byte& linkedVariable_, GEMSelect& select_, void (*callbackAction_)(GEMCallbackData), void* callbackVal_)
  : title(title_)
  , linkedVariable(&linkedVariable_)
  , linkedType(GEM_VAL_SELECT)
  , type(GEM_ITEM_VAL)
  , select(&select_)
  , callbackActionArg(callbackAction_)
  , callbackWithArgs(true)
  , callbackData{ .pMenuItem = this, { .valPointer = callbackVal_ }}
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
  , callbackData{ .pMenuItem = this }
{ }

GEMItem::GEMItem(const char* title_, int& linkedVariable_, GEMSelect& select_, void (*callbackAction_)(GEMCallbackData), byte callbackVal_)
  : title(title_)
  , linkedVariable(&linkedVariable_)
  , linkedType(GEM_VAL_SELECT)
  , type(GEM_ITEM_VAL)
  , select(&select_)
  , callbackActionArg(callbackAction_)
  , callbackWithArgs(true)
  , callbackData{ .pMenuItem = this, { .valByte = callbackVal_ }}
{ }

GEMItem::GEMItem(const char* title_, int& linkedVariable_, GEMSelect& select_, void (*callbackAction_)(GEMCallbackData), int callbackVal_)
  : title(title_)
  , linkedVariable(&linkedVariable_)
  , linkedType(GEM_VAL_SELECT)
  , type(GEM_ITEM_VAL)
  , select(&select_)
  , callbackActionArg(callbackAction_)
  , callbackWithArgs(true)
  , callbackData{ .pMenuItem = this, { .valInt = callbackVal_ }}
{ }

GEMItem::GEMItem(const char* title_, int& linkedVariable_, GEMSelect& select_, void (*callbackAction_)(GEMCallbackData), float callbackVal_)
  : title(title_)
  , linkedVariable(&linkedVariable_)
  , linkedType(GEM_VAL_SELECT)
  , type(GEM_ITEM_VAL)
  , select(&select_)
  , callbackActionArg(callbackAction_)
  , callbackWithArgs(true)
  , callbackData{ .pMenuItem = this, { .valFloat = callbackVal_ }}
{ }

GEMItem::GEMItem(const char* title_, int& linkedVariable_, GEMSelect& select_, void (*callbackAction_)(GEMCallbackData), double callbackVal_)
  : title(title_)
  , linkedVariable(&linkedVariable_)
  , linkedType(GEM_VAL_SELECT)
  , type(GEM_ITEM_VAL)
  , select(&select_)
  , callbackActionArg(callbackAction_)
  , callbackWithArgs(true)
  , callbackData{ .pMenuItem = this, { .valDouble = callbackVal_ }}
{ }

GEMItem::GEMItem(const char* title_, int& linkedVariable_, GEMSelect& select_, void (*callbackAction_)(GEMCallbackData), boolean callbackVal_)
  : title(title_)
  , linkedVariable(&linkedVariable_)
  , linkedType(GEM_VAL_SELECT)
  , type(GEM_ITEM_VAL)
  , select(&select_)
  , callbackActionArg(callbackAction_)
  , callbackWithArgs(true)
  , callbackData{ .pMenuItem = this, { .valBool = callbackVal_ }}
{ }

GEMItem::GEMItem(const char* title_, int& linkedVariable_, GEMSelect& select_, void (*callbackAction_)(GEMCallbackData), const char* callbackVal_)
  : title(title_)
  , linkedVariable(&linkedVariable_)
  , linkedType(GEM_VAL_SELECT)
  , type(GEM_ITEM_VAL)
  , select(&select_)
  , callbackActionArg(callbackAction_)
  , callbackWithArgs(true)
  , callbackData{ .pMenuItem = this, { .valChar = callbackVal_ }}
{ }

GEMItem::GEMItem(const char* title_, int& linkedVariable_, GEMSelect& select_, void (*callbackAction_)(GEMCallbackData), void* callbackVal_)
  : title(title_)
  , linkedVariable(&linkedVariable_)
  , linkedType(GEM_VAL_SELECT)
  , type(GEM_ITEM_VAL)
  , select(&select_)
  , callbackActionArg(callbackAction_)
  , callbackWithArgs(true)
  , callbackData{ .pMenuItem = this, { .valPointer = callbackVal_ }}
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
  , callbackData{ .pMenuItem = this }
{ }

GEMItem::GEMItem(const char* title_, char* linkedVariable_, GEMSelect& select_, void (*callbackAction_)(GEMCallbackData), byte callbackVal_)
  : title(title_)
  , linkedVariable(linkedVariable_)
  , linkedType(GEM_VAL_SELECT)
  , type(GEM_ITEM_VAL)
  , select(&select_)
  , callbackActionArg(callbackAction_)
  , callbackWithArgs(true)
  , callbackData{ .pMenuItem = this, { .valByte = callbackVal_ }}
{ }

GEMItem::GEMItem(const char* title_, char* linkedVariable_, GEMSelect& select_, void (*callbackAction_)(GEMCallbackData), int callbackVal_)
  : title(title_)
  , linkedVariable(linkedVariable_)
  , linkedType(GEM_VAL_SELECT)
  , type(GEM_ITEM_VAL)
  , select(&select_)
  , callbackActionArg(callbackAction_)
  , callbackWithArgs(true)
  , callbackData{ .pMenuItem = this, { .valInt = callbackVal_ }}
{ }

GEMItem::GEMItem(const char* title_, char* linkedVariable_, GEMSelect& select_, void (*callbackAction_)(GEMCallbackData), float callbackVal_)
  : title(title_)
  , linkedVariable(linkedVariable_)
  , linkedType(GEM_VAL_SELECT)
  , type(GEM_ITEM_VAL)
  , select(&select_)
  , callbackActionArg(callbackAction_)
  , callbackWithArgs(true)
  , callbackData{ .pMenuItem = this, { .valFloat = callbackVal_ }}
{ }

GEMItem::GEMItem(const char* title_, char* linkedVariable_, GEMSelect& select_, void (*callbackAction_)(GEMCallbackData), double callbackVal_)
  : title(title_)
  , linkedVariable(linkedVariable_)
  , linkedType(GEM_VAL_SELECT)
  , type(GEM_ITEM_VAL)
  , select(&select_)
  , callbackActionArg(callbackAction_)
  , callbackWithArgs(true)
  , callbackData{ .pMenuItem = this, { .valDouble = callbackVal_ }}
{ }

GEMItem::GEMItem(const char* title_, char* linkedVariable_, GEMSelect& select_, void (*callbackAction_)(GEMCallbackData), boolean callbackVal_)
  : title(title_)
  , linkedVariable(linkedVariable_)
  , linkedType(GEM_VAL_SELECT)
  , type(GEM_ITEM_VAL)
  , select(&select_)
  , callbackActionArg(callbackAction_)
  , callbackWithArgs(true)
  , callbackData{ .pMenuItem = this, { .valBool = callbackVal_ }}
{ }

GEMItem::GEMItem(const char* title_, char* linkedVariable_, GEMSelect& select_, void (*callbackAction_)(GEMCallbackData), const char* callbackVal_)
  : title(title_)
  , linkedVariable(linkedVariable_)
  , linkedType(GEM_VAL_SELECT)
  , type(GEM_ITEM_VAL)
  , select(&select_)
  , callbackActionArg(callbackAction_)
  , callbackWithArgs(true)
  , callbackData{ .pMenuItem = this, { .valChar = callbackVal_ }}
{ }

GEMItem::GEMItem(const char* title_, char* linkedVariable_, GEMSelect& select_, void (*callbackAction_)(GEMCallbackData), void* callbackVal_)
  : title(title_)
  , linkedVariable(linkedVariable_)
  , linkedType(GEM_VAL_SELECT)
  , type(GEM_ITEM_VAL)
  , select(&select_)
  , callbackActionArg(callbackAction_)
  , callbackWithArgs(true)
  , callbackData{ .pMenuItem = this, { .valPointer = callbackVal_ }}
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
  , callbackData{ .pMenuItem = this }
{ }

GEMItem::GEMItem(const char* title_, float& linkedVariable_, GEMSelect& select_, void (*callbackAction_)(GEMCallbackData), byte callbackVal_)
  : title(title_)
  , linkedVariable(&linkedVariable_)
  , linkedType(GEM_VAL_SELECT)
  , type(GEM_ITEM_VAL)
  , select(&select_)
  , callbackActionArg(callbackAction_)
  , callbackWithArgs(true)
  , callbackData{ .pMenuItem = this, { .valByte = callbackVal_ }}
{ }

GEMItem::GEMItem(const char* title_, float& linkedVariable_, GEMSelect& select_, void (*callbackAction_)(GEMCallbackData), int callbackVal_)
  : title(title_)
  , linkedVariable(&linkedVariable_)
  , linkedType(GEM_VAL_SELECT)
  , type(GEM_ITEM_VAL)
  , select(&select_)
  , callbackActionArg(callbackAction_)
  , callbackWithArgs(true)
  , callbackData{ .pMenuItem = this, { .valInt = callbackVal_ }}
{ }

GEMItem::GEMItem(const char* title_, float& linkedVariable_, GEMSelect& select_, void (*callbackAction_)(GEMCallbackData), float callbackVal_)
  : title(title_)
  , linkedVariable(&linkedVariable_)
  , linkedType(GEM_VAL_SELECT)
  , type(GEM_ITEM_VAL)
  , select(&select_)
  , callbackActionArg(callbackAction_)
  , callbackWithArgs(true)
  , callbackData{ .pMenuItem = this, { .valFloat = callbackVal_ }}
{ }

GEMItem::GEMItem(const char* title_, float& linkedVariable_, GEMSelect& select_, void (*callbackAction_)(GEMCallbackData), double callbackVal_)
  : title(title_)
  , linkedVariable(&linkedVariable_)
  , linkedType(GEM_VAL_SELECT)
  , type(GEM_ITEM_VAL)
  , select(&select_)
  , callbackActionArg(callbackAction_)
  , callbackWithArgs(true)
  , callbackData{ .pMenuItem = this, { .valDouble = callbackVal_ }}
{ }

GEMItem::GEMItem(const char* title_, float& linkedVariable_, GEMSelect& select_, void (*callbackAction_)(GEMCallbackData), boolean callbackVal_)
  : title(title_)
  , linkedVariable(&linkedVariable_)
  , linkedType(GEM_VAL_SELECT)
  , type(GEM_ITEM_VAL)
  , select(&select_)
  , callbackActionArg(callbackAction_)
  , callbackWithArgs(true)
  , callbackData{ .pMenuItem = this, { .valBool = callbackVal_ }}
{ }

GEMItem::GEMItem(const char* title_, float& linkedVariable_, GEMSelect& select_, void (*callbackAction_)(GEMCallbackData), const char* callbackVal_)
  : title(title_)
  , linkedVariable(&linkedVariable_)
  , linkedType(GEM_VAL_SELECT)
  , type(GEM_ITEM_VAL)
  , select(&select_)
  , callbackActionArg(callbackAction_)
  , callbackWithArgs(true)
  , callbackData{ .pMenuItem = this, { .valChar = callbackVal_ }}
{ }

GEMItem::GEMItem(const char* title_, float& linkedVariable_, GEMSelect& select_, void (*callbackAction_)(GEMCallbackData), void* callbackVal_)
  : title(title_)
  , linkedVariable(&linkedVariable_)
  , linkedType(GEM_VAL_SELECT)
  , type(GEM_ITEM_VAL)
  , select(&select_)
  , callbackActionArg(callbackAction_)
  , callbackWithArgs(true)
  , callbackData{ .pMenuItem = this, { .valPointer = callbackVal_ }}
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
  , callbackData{ .pMenuItem = this }
{ }

GEMItem::GEMItem(const char* title_, double& linkedVariable_, GEMSelect& select_, void (*callbackAction_)(GEMCallbackData), byte callbackVal_)
  : title(title_)
  , linkedVariable(&linkedVariable_)
  , linkedType(GEM_VAL_SELECT)
  , type(GEM_ITEM_VAL)
  , select(&select_)
  , callbackActionArg(callbackAction_)
  , callbackWithArgs(true)
  , callbackData{ .pMenuItem = this, { .valByte = callbackVal_ }}
{ }

GEMItem::GEMItem(const char* title_, double& linkedVariable_, GEMSelect& select_, void (*callbackAction_)(GEMCallbackData), int callbackVal_)
  : title(title_)
  , linkedVariable(&linkedVariable_)
  , linkedType(GEM_VAL_SELECT)
  , type(GEM_ITEM_VAL)
  , select(&select_)
  , callbackActionArg(callbackAction_)
  , callbackWithArgs(true)
  , callbackData{ .pMenuItem = this, { .valInt = callbackVal_ }}
{ }

GEMItem::GEMItem(const char* title_, double& linkedVariable_, GEMSelect& select_, void (*callbackAction_)(GEMCallbackData), float callbackVal_)
  : title(title_)
  , linkedVariable(&linkedVariable_)
  , linkedType(GEM_VAL_SELECT)
  , type(GEM_ITEM_VAL)
  , select(&select_)
  , callbackActionArg(callbackAction_)
  , callbackWithArgs(true)
  , callbackData{ .pMenuItem = this, { .valFloat = callbackVal_ }}
{ }

GEMItem::GEMItem(const char* title_, double& linkedVariable_, GEMSelect& select_, void (*callbackAction_)(GEMCallbackData), double callbackVal_)
  : title(title_)
  , linkedVariable(&linkedVariable_)
  , linkedType(GEM_VAL_SELECT)
  , type(GEM_ITEM_VAL)
  , select(&select_)
  , callbackActionArg(callbackAction_)
  , callbackWithArgs(true)
  , callbackData{ .pMenuItem = this, { .valDouble = callbackVal_ }}
{ }

GEMItem::GEMItem(const char* title_, double& linkedVariable_, GEMSelect& select_, void (*callbackAction_)(GEMCallbackData), boolean callbackVal_)
  : title(title_)
  , linkedVariable(&linkedVariable_)
  , linkedType(GEM_VAL_SELECT)
  , type(GEM_ITEM_VAL)
  , select(&select_)
  , callbackActionArg(callbackAction_)
  , callbackWithArgs(true)
  , callbackData{ .pMenuItem = this, { .valBool = callbackVal_ }}
{ }

GEMItem::GEMItem(const char* title_, double& linkedVariable_, GEMSelect& select_, void (*callbackAction_)(GEMCallbackData), const char* callbackVal_)
  : title(title_)
  , linkedVariable(&linkedVariable_)
  , linkedType(GEM_VAL_SELECT)
  , type(GEM_ITEM_VAL)
  , select(&select_)
  , callbackActionArg(callbackAction_)
  , callbackWithArgs(true)
  , callbackData{ .pMenuItem = this, { .valChar = callbackVal_ }}
{ }

GEMItem::GEMItem(const char* title_, double& linkedVariable_, GEMSelect& select_, void (*callbackAction_)(GEMCallbackData), void* callbackVal_)
  : title(title_)
  , linkedVariable(&linkedVariable_)
  , linkedType(GEM_VAL_SELECT)
  , type(GEM_ITEM_VAL)
  , select(&select_)
  , callbackActionArg(callbackAction_)
  , callbackWithArgs(true)
  , callbackData{ .pMenuItem = this, { .valPointer = callbackVal_ }}
{ }

//---

GEMItem::GEMItem(const char* title_, byte& linkedVariable_, GEMSelect& select_, boolean readonly_)
  : title(title_)
  , linkedVariable(&linkedVariable_)
  , linkedType(GEM_VAL_SELECT)
  , type(GEM_ITEM_VAL)
  , readonly(readonly_)
  , select(&select_)
{ }

GEMItem::GEMItem(const char* title_, int& linkedVariable_, GEMSelect& select_, boolean readonly_)
  : title(title_)
  , linkedVariable(&linkedVariable_)
  , linkedType(GEM_VAL_SELECT)
  , type(GEM_ITEM_VAL)
  , readonly(readonly_)
  , select(&select_)
{ }

GEMItem::GEMItem(const char* title_, char* linkedVariable_, GEMSelect& select_, boolean readonly_)
  : title(title_)
  , linkedVariable(linkedVariable_)
  , linkedType(GEM_VAL_SELECT)
  , type(GEM_ITEM_VAL)
  , readonly(readonly_)
  , select(&select_)
{ }

GEMItem::GEMItem(const char* title_, float& linkedVariable_, GEMSelect& select_, boolean readonly_)
  : title(title_)
  , linkedVariable(&linkedVariable_)
  , linkedType(GEM_VAL_SELECT)
  , type(GEM_ITEM_VAL)
  , readonly(readonly_)
  , select(&select_)
{ }

GEMItem::GEMItem(const char* title_, double& linkedVariable_, GEMSelect& select_, boolean readonly_)
  : title(title_)
  , linkedVariable(&linkedVariable_)
  , linkedType(GEM_VAL_SELECT)
  , type(GEM_ITEM_VAL)
  , readonly(readonly_)
  , select(&select_)
{ }

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

GEMItem::GEMItem(const char* title_, boolean& linkedVariable_, void (*callbackAction_)())
  : title(title_)
  , linkedVariable(&linkedVariable_)
  , linkedType(GEM_VAL_BOOLEAN)
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
  , callbackData{ .pMenuItem = this }
{ }

GEMItem::GEMItem(const char* title_, byte& linkedVariable_, void (*callbackAction_)(GEMCallbackData), byte callbackVal_)
  : title(title_)
  , linkedVariable(&linkedVariable_)
  , linkedType(GEM_VAL_BYTE)
  , type(GEM_ITEM_VAL)
  , callbackActionArg(callbackAction_)
  , callbackWithArgs(true)
  , callbackData{ .pMenuItem = this, { .valByte = callbackVal_ }}
{ }

GEMItem::GEMItem(const char* title_, byte& linkedVariable_, void (*callbackAction_)(GEMCallbackData), int callbackVal_)
  : title(title_)
  , linkedVariable(&linkedVariable_)
  , linkedType(GEM_VAL_BYTE)
  , type(GEM_ITEM_VAL)
  , callbackActionArg(callbackAction_)
  , callbackWithArgs(true)
  , callbackData{ .pMenuItem = this, { .valInt = callbackVal_ }}
{ }

GEMItem::GEMItem(const char* title_, byte& linkedVariable_, void (*callbackAction_)(GEMCallbackData), float callbackVal_)
  : title(title_)
  , linkedVariable(&linkedVariable_)
  , linkedType(GEM_VAL_BYTE)
  , type(GEM_ITEM_VAL)
  , callbackActionArg(callbackAction_)
  , callbackWithArgs(true)
  , callbackData{ .pMenuItem = this, { .valFloat = callbackVal_ }}
{ }

GEMItem::GEMItem(const char* title_, byte& linkedVariable_, void (*callbackAction_)(GEMCallbackData), double callbackVal_)
  : title(title_)
  , linkedVariable(&linkedVariable_)
  , linkedType(GEM_VAL_BYTE)
  , type(GEM_ITEM_VAL)
  , callbackActionArg(callbackAction_)
  , callbackWithArgs(true)
  , callbackData{ .pMenuItem = this, { .valDouble = callbackVal_ }}
{ }

GEMItem::GEMItem(const char* title_, byte& linkedVariable_, void (*callbackAction_)(GEMCallbackData), boolean callbackVal_)
  : title(title_)
  , linkedVariable(&linkedVariable_)
  , linkedType(GEM_VAL_BYTE)
  , type(GEM_ITEM_VAL)
  , callbackActionArg(callbackAction_)
  , callbackWithArgs(true)
  , callbackData{ .pMenuItem = this, { .valBool = callbackVal_ }}
{ }

GEMItem::GEMItem(const char* title_, byte& linkedVariable_, void (*callbackAction_)(GEMCallbackData), const char* callbackVal_)
  : title(title_)
  , linkedVariable(&linkedVariable_)
  , linkedType(GEM_VAL_BYTE)
  , type(GEM_ITEM_VAL)
  , callbackActionArg(callbackAction_)
  , callbackWithArgs(true)
  , callbackData{ .pMenuItem = this, { .valChar = callbackVal_ }}
{ }

GEMItem::GEMItem(const char* title_, byte& linkedVariable_, void (*callbackAction_)(GEMCallbackData), void* callbackVal_)
  : title(title_)
  , linkedVariable(&linkedVariable_)
  , linkedType(GEM_VAL_BYTE)
  , type(GEM_ITEM_VAL)
  , callbackActionArg(callbackAction_)
  , callbackWithArgs(true)
  , callbackData{ .pMenuItem = this, { .valPointer = callbackVal_ }}
{ }

//---

GEMItem::GEMItem(const char* title_, int& linkedVariable_, void (*callbackAction_)(GEMCallbackData))
  : title(title_)
  , linkedVariable(&linkedVariable_)
  , linkedType(GEM_VAL_INTEGER)
  , type(GEM_ITEM_VAL)
  , callbackActionArg(callbackAction_)
  , callbackWithArgs(true)
  , callbackData{ .pMenuItem = this }
{ }

GEMItem::GEMItem(const char* title_, int& linkedVariable_, void (*callbackAction_)(GEMCallbackData), byte callbackVal_)
  : title(title_)
  , linkedVariable(&linkedVariable_)
  , linkedType(GEM_VAL_INTEGER)
  , type(GEM_ITEM_VAL)
  , callbackActionArg(callbackAction_)
  , callbackWithArgs(true)
  , callbackData{ .pMenuItem = this, { .valByte = callbackVal_ }}
{ }

GEMItem::GEMItem(const char* title_, int& linkedVariable_, void (*callbackAction_)(GEMCallbackData), int callbackVal_)
  : title(title_)
  , linkedVariable(&linkedVariable_)
  , linkedType(GEM_VAL_INTEGER)
  , type(GEM_ITEM_VAL)
  , callbackActionArg(callbackAction_)
  , callbackWithArgs(true)
  , callbackData{ .pMenuItem = this, { .valInt = callbackVal_ }}
{ }

GEMItem::GEMItem(const char* title_, int& linkedVariable_, void (*callbackAction_)(GEMCallbackData), float callbackVal_)
  : title(title_)
  , linkedVariable(&linkedVariable_)
  , linkedType(GEM_VAL_INTEGER)
  , type(GEM_ITEM_VAL)
  , callbackActionArg(callbackAction_)
  , callbackWithArgs(true)
  , callbackData{ .pMenuItem = this, { .valFloat = callbackVal_ }}
{ }

GEMItem::GEMItem(const char* title_, int& linkedVariable_, void (*callbackAction_)(GEMCallbackData), double callbackVal_)
  : title(title_)
  , linkedVariable(&linkedVariable_)
  , linkedType(GEM_VAL_INTEGER)
  , type(GEM_ITEM_VAL)
  , callbackActionArg(callbackAction_)
  , callbackWithArgs(true)
  , callbackData{ .pMenuItem = this, { .valDouble = callbackVal_ }}
{ }

GEMItem::GEMItem(const char* title_, int& linkedVariable_, void (*callbackAction_)(GEMCallbackData), boolean callbackVal_)
  : title(title_)
  , linkedVariable(&linkedVariable_)
  , linkedType(GEM_VAL_INTEGER)
  , type(GEM_ITEM_VAL)
  , callbackActionArg(callbackAction_)
  , callbackWithArgs(true)
  , callbackData{ .pMenuItem = this, { .valBool = callbackVal_ }}
{ }

GEMItem::GEMItem(const char* title_, int& linkedVariable_, void (*callbackAction_)(GEMCallbackData), const char* callbackVal_)
  : title(title_)
  , linkedVariable(&linkedVariable_)
  , linkedType(GEM_VAL_INTEGER)
  , type(GEM_ITEM_VAL)
  , callbackActionArg(callbackAction_)
  , callbackWithArgs(true)
  , callbackData{ .pMenuItem = this, { .valChar = callbackVal_ }}
{ }

GEMItem::GEMItem(const char* title_, int& linkedVariable_, void (*callbackAction_)(GEMCallbackData), void* callbackVal_)
  : title(title_)
  , linkedVariable(&linkedVariable_)
  , linkedType(GEM_VAL_INTEGER)
  , type(GEM_ITEM_VAL)
  , callbackActionArg(callbackAction_)
  , callbackWithArgs(true)
  , callbackData{ .pMenuItem = this, { .valPointer = callbackVal_ }}
{ }

//---

GEMItem::GEMItem(const char* title_, char* linkedVariable_, void (*callbackAction_)(GEMCallbackData))
  : title(title_)
  , linkedVariable(&linkedVariable_)
  , linkedType(GEM_VAL_CHAR)
  , type(GEM_ITEM_VAL)
  , callbackActionArg(callbackAction_)
  , callbackWithArgs(true)
  , callbackData{ .pMenuItem = this }
{ }

GEMItem::GEMItem(const char* title_, char* linkedVariable_, void (*callbackAction_)(GEMCallbackData), byte callbackVal_)
  : title(title_)
  , linkedVariable(&linkedVariable_)
  , linkedType(GEM_VAL_CHAR)
  , type(GEM_ITEM_VAL)
  , callbackActionArg(callbackAction_)
  , callbackWithArgs(true)
  , callbackData{ .pMenuItem = this, { .valByte = callbackVal_ }}
{ }

GEMItem::GEMItem(const char* title_, char* linkedVariable_, void (*callbackAction_)(GEMCallbackData), int callbackVal_)
  : title(title_)
  , linkedVariable(&linkedVariable_)
  , linkedType(GEM_VAL_CHAR)
  , type(GEM_ITEM_VAL)
  , callbackActionArg(callbackAction_)
  , callbackWithArgs(true)
  , callbackData{ .pMenuItem = this, { .valInt = callbackVal_ }}
{ }

GEMItem::GEMItem(const char* title_, char* linkedVariable_, void (*callbackAction_)(GEMCallbackData), float callbackVal_)
  : title(title_)
  , linkedVariable(&linkedVariable_)
  , linkedType(GEM_VAL_CHAR)
  , type(GEM_ITEM_VAL)
  , callbackActionArg(callbackAction_)
  , callbackWithArgs(true)
  , callbackData{ .pMenuItem = this, { .valFloat = callbackVal_ }}
{ }

GEMItem::GEMItem(const char* title_, char* linkedVariable_, void (*callbackAction_)(GEMCallbackData), double callbackVal_)
  : title(title_)
  , linkedVariable(&linkedVariable_)
  , linkedType(GEM_VAL_CHAR)
  , type(GEM_ITEM_VAL)
  , callbackActionArg(callbackAction_)
  , callbackWithArgs(true)
  , callbackData{ .pMenuItem = this, { .valDouble = callbackVal_ }}
{ }

GEMItem::GEMItem(const char* title_, char* linkedVariable_, void (*callbackAction_)(GEMCallbackData), boolean callbackVal_)
  : title(title_)
  , linkedVariable(&linkedVariable_)
  , linkedType(GEM_VAL_CHAR)
  , type(GEM_ITEM_VAL)
  , callbackActionArg(callbackAction_)
  , callbackWithArgs(true)
  , callbackData{ .pMenuItem = this, { .valBool = callbackVal_ }}
{ }

GEMItem::GEMItem(const char* title_, char* linkedVariable_, void (*callbackAction_)(GEMCallbackData), const char* callbackVal_)
  : title(title_)
  , linkedVariable(&linkedVariable_)
  , linkedType(GEM_VAL_CHAR)
  , type(GEM_ITEM_VAL)
  , callbackActionArg(callbackAction_)
  , callbackWithArgs(true)
  , callbackData{ .pMenuItem = this, { .valChar = callbackVal_ }}
{ }

GEMItem::GEMItem(const char* title_, char* linkedVariable_, void (*callbackAction_)(GEMCallbackData), void* callbackVal_)
  : title(title_)
  , linkedVariable(&linkedVariable_)
  , linkedType(GEM_VAL_CHAR)
  , type(GEM_ITEM_VAL)
  , callbackActionArg(callbackAction_)
  , callbackWithArgs(true)
  , callbackData{ .pMenuItem = this, { .valPointer = callbackVal_ }}
{ }

//---

GEMItem::GEMItem(const char* title_, boolean& linkedVariable_, void (*callbackAction_)(GEMCallbackData))
  : title(title_)
  , linkedVariable(&linkedVariable_)
  , linkedType(GEM_VAL_BOOLEAN)
  , type(GEM_ITEM_VAL)
  , callbackActionArg(callbackAction_)
  , callbackWithArgs(true)
  , callbackData{ .pMenuItem = this }
{ }

GEMItem::GEMItem(const char* title_, boolean& linkedVariable_, void (*callbackAction_)(GEMCallbackData), byte callbackVal_)
  : title(title_)
  , linkedVariable(&linkedVariable_)
  , linkedType(GEM_VAL_BOOLEAN)
  , type(GEM_ITEM_VAL)
  , callbackActionArg(callbackAction_)
  , callbackWithArgs(true)
  , callbackData{ .pMenuItem = this, { .valByte = callbackVal_ }}
{ }

GEMItem::GEMItem(const char* title_, boolean& linkedVariable_, void (*callbackAction_)(GEMCallbackData), int callbackVal_)
  : title(title_)
  , linkedVariable(&linkedVariable_)
  , linkedType(GEM_VAL_BOOLEAN)
  , type(GEM_ITEM_VAL)
  , callbackActionArg(callbackAction_)
  , callbackWithArgs(true)
  , callbackData{ .pMenuItem = this, { .valInt = callbackVal_ }}
{ }

GEMItem::GEMItem(const char* title_, boolean& linkedVariable_, void (*callbackAction_)(GEMCallbackData), float callbackVal_)
  : title(title_)
  , linkedVariable(&linkedVariable_)
  , linkedType(GEM_VAL_BOOLEAN)
  , type(GEM_ITEM_VAL)
  , callbackActionArg(callbackAction_)
  , callbackWithArgs(true)
  , callbackData{ .pMenuItem = this, { .valFloat = callbackVal_ }}
{ }

GEMItem::GEMItem(const char* title_, boolean& linkedVariable_, void (*callbackAction_)(GEMCallbackData), double callbackVal_)
  : title(title_)
  , linkedVariable(&linkedVariable_)
  , linkedType(GEM_VAL_BOOLEAN)
  , type(GEM_ITEM_VAL)
  , callbackActionArg(callbackAction_)
  , callbackWithArgs(true)
  , callbackData{ .pMenuItem = this, { .valDouble = callbackVal_ }}
{ }

GEMItem::GEMItem(const char* title_, boolean& linkedVariable_, void (*callbackAction_)(GEMCallbackData), boolean callbackVal_)
  : title(title_)
  , linkedVariable(&linkedVariable_)
  , linkedType(GEM_VAL_BOOLEAN)
  , type(GEM_ITEM_VAL)
  , callbackActionArg(callbackAction_)
  , callbackWithArgs(true)
  , callbackData{ .pMenuItem = this, { .valBool = callbackVal_ }}
{ }

GEMItem::GEMItem(const char* title_, boolean& linkedVariable_, void (*callbackAction_)(GEMCallbackData), const char* callbackVal_)
  : title(title_)
  , linkedVariable(&linkedVariable_)
  , linkedType(GEM_VAL_BOOLEAN)
  , type(GEM_ITEM_VAL)
  , callbackActionArg(callbackAction_)
  , callbackWithArgs(true)
  , callbackData{ .pMenuItem = this, { .valChar = callbackVal_ }}
{ }

GEMItem::GEMItem(const char* title_, boolean& linkedVariable_, void (*callbackAction_)(GEMCallbackData), void* callbackVal_)
  : title(title_)
  , linkedVariable(&linkedVariable_)
  , linkedType(GEM_VAL_BOOLEAN)
  , type(GEM_ITEM_VAL)
  , callbackActionArg(callbackAction_)
  , callbackWithArgs(true)
  , callbackData{ .pMenuItem = this, { .valPointer = callbackVal_ }}
{ }

//---

GEMItem::GEMItem(const char* title_, float& linkedVariable_, void (*callbackAction_)(GEMCallbackData))
  : title(title_)
  , linkedVariable(&linkedVariable_)
  , linkedType(GEM_VAL_FLOAT)
  , type(GEM_ITEM_VAL)
  , callbackActionArg(callbackAction_)
  , callbackWithArgs(true)
  , callbackData{ .pMenuItem = this }
{ }

GEMItem::GEMItem(const char* title_, float& linkedVariable_, void (*callbackAction_)(GEMCallbackData), byte callbackVal_)
  : title(title_)
  , linkedVariable(&linkedVariable_)
  , linkedType(GEM_VAL_FLOAT)
  , type(GEM_ITEM_VAL)
  , callbackActionArg(callbackAction_)
  , callbackWithArgs(true)
  , callbackData{ .pMenuItem = this, { .valByte = callbackVal_ }}
{ }

GEMItem::GEMItem(const char* title_, float& linkedVariable_, void (*callbackAction_)(GEMCallbackData), int callbackVal_)
  : title(title_)
  , linkedVariable(&linkedVariable_)
  , linkedType(GEM_VAL_FLOAT)
  , type(GEM_ITEM_VAL)
  , callbackActionArg(callbackAction_)
  , callbackWithArgs(true)
  , callbackData{ .pMenuItem = this, { .valInt = callbackVal_ }}
{ }

GEMItem::GEMItem(const char* title_, float& linkedVariable_, void (*callbackAction_)(GEMCallbackData), float callbackVal_)
  : title(title_)
  , linkedVariable(&linkedVariable_)
  , linkedType(GEM_VAL_FLOAT)
  , type(GEM_ITEM_VAL)
  , callbackActionArg(callbackAction_)
  , callbackWithArgs(true)
  , callbackData{ .pMenuItem = this, { .valFloat = callbackVal_ }}
{ }

GEMItem::GEMItem(const char* title_, float& linkedVariable_, void (*callbackAction_)(GEMCallbackData), double callbackVal_)
  : title(title_)
  , linkedVariable(&linkedVariable_)
  , linkedType(GEM_VAL_FLOAT)
  , type(GEM_ITEM_VAL)
  , callbackActionArg(callbackAction_)
  , callbackWithArgs(true)
  , callbackData{ .pMenuItem = this, { .valDouble = callbackVal_ }}
{ }

GEMItem::GEMItem(const char* title_, float& linkedVariable_, void (*callbackAction_)(GEMCallbackData), boolean callbackVal_)
  : title(title_)
  , linkedVariable(&linkedVariable_)
  , linkedType(GEM_VAL_FLOAT)
  , type(GEM_ITEM_VAL)
  , callbackActionArg(callbackAction_)
  , callbackWithArgs(true)
  , callbackData{ .pMenuItem = this, { .valBool = callbackVal_ }}
{ }

GEMItem::GEMItem(const char* title_, float& linkedVariable_, void (*callbackAction_)(GEMCallbackData), const char* callbackVal_)
  : title(title_)
  , linkedVariable(&linkedVariable_)
  , linkedType(GEM_VAL_FLOAT)
  , type(GEM_ITEM_VAL)
  , callbackActionArg(callbackAction_)
  , callbackWithArgs(true)
  , callbackData{ .pMenuItem = this, { .valChar = callbackVal_ }}
{ }

GEMItem::GEMItem(const char* title_, float& linkedVariable_, void (*callbackAction_)(GEMCallbackData), void* callbackVal_)
  : title(title_)
  , linkedVariable(&linkedVariable_)
  , linkedType(GEM_VAL_FLOAT)
  , type(GEM_ITEM_VAL)
  , callbackActionArg(callbackAction_)
  , callbackWithArgs(true)
  , callbackData{ .pMenuItem = this, { .valPointer = callbackVal_ }}
{ }

//---

GEMItem::GEMItem(const char* title_, double& linkedVariable_, void (*callbackAction_)(GEMCallbackData))
  : title(title_)
  , linkedVariable(&linkedVariable_)
  , linkedType(GEM_VAL_DOUBLE)
  , type(GEM_ITEM_VAL)
  , callbackActionArg(callbackAction_)
  , callbackWithArgs(true)
  , callbackData{ .pMenuItem = this }
{ }

GEMItem::GEMItem(const char* title_, double& linkedVariable_, void (*callbackAction_)(GEMCallbackData), byte callbackVal_)
  : title(title_)
  , linkedVariable(&linkedVariable_)
  , linkedType(GEM_VAL_DOUBLE)
  , type(GEM_ITEM_VAL)
  , callbackActionArg(callbackAction_)
  , callbackWithArgs(true)
  , callbackData{ .pMenuItem = this, { .valByte = callbackVal_ }}
{ }

GEMItem::GEMItem(const char* title_, double& linkedVariable_, void (*callbackAction_)(GEMCallbackData), int callbackVal_)
  : title(title_)
  , linkedVariable(&linkedVariable_)
  , linkedType(GEM_VAL_DOUBLE)
  , type(GEM_ITEM_VAL)
  , callbackActionArg(callbackAction_)
  , callbackWithArgs(true)
  , callbackData{ .pMenuItem = this, { .valInt = callbackVal_ }}
{ }

GEMItem::GEMItem(const char* title_, double& linkedVariable_, void (*callbackAction_)(GEMCallbackData), float callbackVal_)
  : title(title_)
  , linkedVariable(&linkedVariable_)
  , linkedType(GEM_VAL_DOUBLE)
  , type(GEM_ITEM_VAL)
  , callbackActionArg(callbackAction_)
  , callbackWithArgs(true)
  , callbackData{ .pMenuItem = this, { .valFloat = callbackVal_ }}
{ }

GEMItem::GEMItem(const char* title_, double& linkedVariable_, void (*callbackAction_)(GEMCallbackData), double callbackVal_)
  : title(title_)
  , linkedVariable(&linkedVariable_)
  , linkedType(GEM_VAL_DOUBLE)
  , type(GEM_ITEM_VAL)
  , callbackActionArg(callbackAction_)
  , callbackWithArgs(true)
  , callbackData{ .pMenuItem = this, { .valDouble = callbackVal_ }}
{ }

GEMItem::GEMItem(const char* title_, double& linkedVariable_, void (*callbackAction_)(GEMCallbackData), boolean callbackVal_)
  : title(title_)
  , linkedVariable(&linkedVariable_)
  , linkedType(GEM_VAL_DOUBLE)
  , type(GEM_ITEM_VAL)
  , callbackActionArg(callbackAction_)
  , callbackWithArgs(true)
  , callbackData{ .pMenuItem = this, { .valBool = callbackVal_ }}
{ }

GEMItem::GEMItem(const char* title_, double& linkedVariable_, void (*callbackAction_)(GEMCallbackData), const char* callbackVal_)
  : title(title_)
  , linkedVariable(&linkedVariable_)
  , linkedType(GEM_VAL_DOUBLE)
  , type(GEM_ITEM_VAL)
  , callbackActionArg(callbackAction_)
  , callbackWithArgs(true)
  , callbackData{ .pMenuItem = this, { .valChar = callbackVal_ }}
{ }

GEMItem::GEMItem(const char* title_, double& linkedVariable_, void (*callbackAction_)(GEMCallbackData), void* callbackVal_)
  : title(title_)
  , linkedVariable(&linkedVariable_)
  , linkedType(GEM_VAL_DOUBLE)
  , type(GEM_ITEM_VAL)
  , callbackActionArg(callbackAction_)
  , callbackWithArgs(true)
  , callbackData{ .pMenuItem = this, { .valPointer = callbackVal_ }}
{ }

//---

GEMItem::GEMItem(const char* title_, byte& linkedVariable_, boolean readonly_)
  : title(title_)
  , linkedVariable(&linkedVariable_)
  , linkedType(GEM_VAL_BYTE)
  , type(GEM_ITEM_VAL)
  , readonly(readonly_)
{ }

GEMItem::GEMItem(const char* title_, int& linkedVariable_, boolean readonly_)
  : title(title_)
  , linkedVariable(&linkedVariable_)
  , linkedType(GEM_VAL_INTEGER)
  , type(GEM_ITEM_VAL)
  , readonly(readonly_)
{ }

GEMItem::GEMItem(const char* title_, char* linkedVariable_, boolean readonly_)
  : title(title_)
  , linkedVariable(linkedVariable_)
  , linkedType(GEM_VAL_CHAR)
  , type(GEM_ITEM_VAL)
  , readonly(readonly_)
{ }

GEMItem::GEMItem(const char* title_, boolean& linkedVariable_, boolean readonly_)
  : title(title_)
  , linkedVariable(&linkedVariable_)
  , linkedType(GEM_VAL_BOOLEAN)
  , type(GEM_ITEM_VAL)
  , readonly(readonly_)
{ }

GEMItem::GEMItem(const char* title_, float& linkedVariable_, boolean readonly_)
  : title(title_)
  , linkedVariable(&linkedVariable_)
  , linkedType(GEM_VAL_FLOAT)
  , type(GEM_ITEM_VAL)
  , precision(GEM_FLOAT_PREC)
  , readonly(readonly_)
{ }

GEMItem::GEMItem(const char* title_, double& linkedVariable_, boolean readonly_)
  : title(title_)
  , linkedVariable(&linkedVariable_)
  , linkedType(GEM_VAL_DOUBLE)
  , type(GEM_ITEM_VAL)
  , precision(GEM_DOUBLE_PREC)
  , readonly(readonly_)
{ }

//---

GEMItem::GEMItem(const char* title_, GEMPage& linkedPage_, boolean readonly_)
  : title(title_)
  , type(GEM_ITEM_LINK)
  , readonly(readonly_)
  , linkedPage(&linkedPage_)
{ }

GEMItem::GEMItem(const char* title_, GEMPage* linkedPage_, boolean readonly_)
  : title(title_)
  , type(GEM_ITEM_LINK)
  , readonly(readonly_)
  , linkedPage(linkedPage_)
{ }

GEMItem::GEMItem(const char* title_, void (*callbackAction_)(), boolean readonly_)
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
  , callbackData{ .pMenuItem = this }
{ }

GEMItem::GEMItem(const char* title_, void (*callbackAction_)(GEMCallbackData), byte callbackVal_, boolean readonly_)
  : title(title_)
  , type(GEM_ITEM_BUTTON)
  , readonly(readonly_)
  , callbackActionArg(callbackAction_)
  , callbackWithArgs(true)
  , callbackData{ .pMenuItem = this, { .valByte = callbackVal_ }}
{ }

GEMItem::GEMItem(const char* title_, void (*callbackAction_)(GEMCallbackData), int callbackVal_, boolean readonly_)
  : title(title_)
  , type(GEM_ITEM_BUTTON)
  , readonly(readonly_)
  , callbackActionArg(callbackAction_)
  , callbackWithArgs(true)
  , callbackData{ .pMenuItem = this, { .valInt = callbackVal_ }}
{ }

GEMItem::GEMItem(const char* title_, void (*callbackAction_)(GEMCallbackData), float callbackVal_, boolean readonly_)
  : title(title_)
  , type(GEM_ITEM_BUTTON)
  , readonly(readonly_)
  , callbackActionArg(callbackAction_)
  , callbackWithArgs(true)
  , callbackData{ .pMenuItem = this, { .valFloat = callbackVal_ }}
{ }

GEMItem::GEMItem(const char* title_, void (*callbackAction_)(GEMCallbackData), double callbackVal_, boolean readonly_)
  : title(title_)
  , type(GEM_ITEM_BUTTON)
  , readonly(readonly_)
  , callbackActionArg(callbackAction_)
  , callbackWithArgs(true)
  , callbackData{ .pMenuItem = this, { .valDouble = callbackVal_ }}
{ }

GEMItem::GEMItem(const char* title_, void (*callbackAction_)(GEMCallbackData), boolean callbackVal_, boolean readonly_)
  : title(title_)
  , type(GEM_ITEM_BUTTON)
  , readonly(readonly_)
  , callbackActionArg(callbackAction_)
  , callbackWithArgs(true)
  , callbackData{ .pMenuItem = this, { .valBool = callbackVal_ }}
{ }

GEMItem::GEMItem(const char* title_, void (*callbackAction_)(GEMCallbackData), const char* callbackVal_, boolean readonly_)
  : title(title_)
  , type(GEM_ITEM_BUTTON)
  , readonly(readonly_)
  , callbackActionArg(callbackAction_)
  , callbackWithArgs(true)
  , callbackData{ .pMenuItem = this, { .valChar = callbackVal_ }}
{ }

GEMItem::GEMItem(const char* title_, void (*callbackAction_)(GEMCallbackData), void* callbackVal_, boolean readonly_)
  : title(title_)
  , type(GEM_ITEM_BUTTON)
  , readonly(readonly_)
  , callbackActionArg(callbackAction_)
  , callbackWithArgs(true)
  , callbackData{ .pMenuItem = this, { .valPointer = callbackVal_ }}
{ }

//---

void GEMItem::setCallbackVal(byte callbackVal_) {
  callbackData.valByte = callbackVal_;
}

void GEMItem::setCallbackVal(int callbackVal_) {
  callbackData.valInt = callbackVal_;
}

void GEMItem::setCallbackVal(float callbackVal_) {
  callbackData.valFloat = callbackVal_;
}

void GEMItem::setCallbackVal(double callbackVal_) {
  callbackData.valDouble = callbackVal_;
}

void GEMItem::setCallbackVal(boolean callbackVal_) {
  callbackData.valBool = callbackVal_;
}

void GEMItem::setCallbackVal(const char* callbackVal_) {
  callbackData.valChar = callbackVal_;
}

void GEMItem::setCallbackVal(void* callbackVal_) {
  callbackData.valPointer = callbackVal_;
}

GEMCallbackData GEMItem::getCallbackData() {
  return callbackData;
}

//---

void GEMItem::setTitle(const char* title_) {
  title = title_;
}

const char* GEMItem::getTitle() {
  return title;
}

void GEMItem::setPrecision(byte prec) {
  precision = prec;
}

void GEMItem::setReadonly(boolean mode) {
  readonly = mode;
}

boolean GEMItem::getReadonly() {
  return readonly;
}

void GEMItem::hide(boolean hide) {
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
}

void GEMItem::show() {
  if (hidden) {
    if (parentPage != nullptr) {
      parentPage->showMenuItem(*this);
    } else {
      hidden = false;
    }
  }
}

boolean GEMItem::getHidden() {
  return hidden;
}

void* GEMItem::getLinkedVariablePointer() {
  return linkedVariable;
}

GEMItem* GEMItem::getMenuItemNext() {
  GEMItem* menuItemTmp = menuItemNext;
  while (menuItemTmp != nullptr && menuItemTmp->hidden) {
    menuItemTmp = menuItemTmp->menuItemNext;
  }
  return menuItemTmp;
}
