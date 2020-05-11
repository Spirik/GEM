/*
  GEMItem - menu item for GEM library.

  GEM (a.k.a. Good Enough Menu) - Arduino library for creation of graphic multi-level menu with
  editable menu items, such as variables (supports int, byte, boolean, char[17] data types) and
  option selects. User-defined callback function can be specified to invoke when menu item is saved.
  
  Supports buttons that can invoke user-defined actions and create action-specific
  context, which can have its own enter (setup) and exit callbacks as well as loop function.

  Supports AltSerialGraphicLCD library by Jon Green (http://www.jasspa.com/serialGLCD.html)
  and U8g2 library by olikraus (https://github.com/olikraus/U8g2_Arduino).

  For documentation visit:
  https://github.com/Spirik/GEM

  Copyright (c) 2018 Alexander 'Spirik' Spiridonov

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

GEMItem::GEMItem(char* title_, byte& linkedVariable_, GEMSelect& select_, void (*saveAction_)())
  : title(title_)
  , linkedVariable(&linkedVariable_)
  , linkedType(GEM_VAL_SELECT)
  , select(&select_)
  , saveAction(saveAction_)
  , type(GEM_ITEM_VAL)
{ }

GEMItem::GEMItem(char* title_, int& linkedVariable_, GEMSelect& select_, void (*saveAction_)())
  : title(title_)
  , linkedVariable(&linkedVariable_)
  , linkedType(GEM_VAL_SELECT)
  , select(&select_)
  , saveAction(saveAction_)
  , type(GEM_ITEM_VAL)
{ }

GEMItem::GEMItem(char* title_, char* linkedVariable_, GEMSelect& select_, void (*saveAction_)())
  : title(title_)
  , linkedVariable(linkedVariable_)
  , linkedType(GEM_VAL_SELECT)
  , select(&select_)
  , saveAction(saveAction_)
  , type(GEM_ITEM_VAL)
{ }

//---

GEMItem::GEMItem(char* title_, byte& linkedVariable_, GEMSelect& select_, boolean readonly_)
  : title(title_)
  , linkedVariable(&linkedVariable_)
  , linkedType(GEM_VAL_SELECT)
  , select(&select_)
  , readonly(readonly_)
  , type(GEM_ITEM_VAL)
{ }

GEMItem::GEMItem(char* title_, int& linkedVariable_, GEMSelect& select_, boolean readonly_)
  : title(title_)
  , linkedVariable(&linkedVariable_)
  , linkedType(GEM_VAL_SELECT)
  , select(&select_)
  , readonly(readonly_)
  , type(GEM_ITEM_VAL)
{ }

GEMItem::GEMItem(char* title_, char* linkedVariable_, GEMSelect& select_, boolean readonly_)
  : title(title_)
  , linkedVariable(linkedVariable_)
  , linkedType(GEM_VAL_SELECT)
  , select(&select_)
  , readonly(readonly_)
  , type(GEM_ITEM_VAL)
{ }

//---

GEMItem::GEMItem(char* title_, byte& linkedVariable_, void (*saveAction_)())
  : title(title_)
  , linkedVariable(&linkedVariable_)
  , linkedType(GEM_VAL_BYTE)
  , type(GEM_ITEM_VAL)
  , saveAction(saveAction_)
{ }

GEMItem::GEMItem(char* title_, int& linkedVariable_, void (*saveAction_)())
  : title(title_)
  , linkedVariable(&linkedVariable_)
  , linkedType(GEM_VAL_INTEGER)
  , type(GEM_ITEM_VAL)
  , saveAction(saveAction_)
{ }

GEMItem::GEMItem(char* title_, char* linkedVariable_, void (*saveAction_)())
  : title(title_)
  , linkedVariable(linkedVariable_)
  , linkedType(GEM_VAL_CHAR)
  , type(GEM_ITEM_VAL)
  , saveAction(saveAction_)
{ }

GEMItem::GEMItem(char* title_, boolean& linkedVariable_, void (*saveAction_)())
  : title(title_)
  , linkedVariable(&linkedVariable_)
  , linkedType(GEM_VAL_BOOLEAN)
  , type(GEM_ITEM_VAL)
  , saveAction(saveAction_)
{ }

//---

GEMItem::GEMItem(char* title_, byte& linkedVariable_, boolean readonly_)
  : title(title_)
  , linkedVariable(&linkedVariable_)
  , linkedType(GEM_VAL_BYTE)
  , readonly(readonly_)
  , type(GEM_ITEM_VAL)
{ }

GEMItem::GEMItem(char* title_, int& linkedVariable_, boolean readonly_)
  : title(title_)
  , linkedVariable(&linkedVariable_)
  , linkedType(GEM_VAL_INTEGER)
  , readonly(readonly_)
  , type(GEM_ITEM_VAL)
{ }

GEMItem::GEMItem(char* title_, char* linkedVariable_, boolean readonly_)
  : title(title_)
  , linkedVariable(linkedVariable_)
  , linkedType(GEM_VAL_CHAR)
  , readonly(readonly_)
  , type(GEM_ITEM_VAL)
{ }

GEMItem::GEMItem(char* title_, boolean& linkedVariable_, boolean readonly_)
  : title(title_)
  , linkedVariable(&linkedVariable_)
  , linkedType(GEM_VAL_BOOLEAN)
  , readonly(readonly_)
  , type(GEM_ITEM_VAL)
{ }

//---

GEMItem::GEMItem(char* title_, GEMPage& linkedPage_, boolean readonly_)
  : title(title_)
  , linkedPage(&linkedPage_)
  , readonly(readonly_)
  , type(GEM_ITEM_LINK)
{ }

GEMItem::GEMItem(char* title_, GEMPage* linkedPage_, boolean readonly_)
  : title(title_)
  , linkedPage(linkedPage_)
  , readonly(readonly_)
  , type(GEM_ITEM_LINK)
{ }

GEMItem::GEMItem(char* title_, void (*buttonAction_)(), boolean readonly_)
  : title(title_)
  , buttonAction(buttonAction_)
  , readonly(readonly_)
  , type(GEM_ITEM_BUTTON)
{ }

void GEMItem::setReadonly(boolean mode) {
  readonly = mode;
}

boolean GEMItem::getReadonly() {
  return readonly;
}
