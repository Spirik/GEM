/*
  GEMItem - menu item for GEM library.

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

  Created by Alexander 'Spirik' Spiridonov, 6 August 2017
  
  This is free software. You can redistribute it and/or modify it under
  the terms of Creative Commons Attribution-ShareAlike 4.0 International License.
  To view a copy of this license, visit https://creativecommons.org/licenses/by-sa/4.0/
*/

#include <Arduino.h>
#include "GEMItem.h"

GEMItem::GEMItem(char* title_, void* linkedVariable_, byte linkedType_, GEMSelect* select_, void (*saveAction_)())
  : title(title_)
  , linkedVariable(linkedVariable_)
  , linkedType(linkedType_)
  , type(GEM_ITEM_VAL)
  , select(select_)
  , saveAction(saveAction_)
{ }

GEMItem::GEMItem(char* title_, void* linkedVariable_, byte linkedType_, GEMSelect* select_)
  : title(title_)
  , linkedVariable(linkedVariable_)
  , linkedType(linkedType_)
  , type(GEM_ITEM_VAL)
  , select(select_)
{ }

GEMItem::GEMItem(char* title_, void* linkedVariable_, byte linkedType_, void (*saveAction_)())
  : title(title_)
  , linkedVariable(linkedVariable_)
  , linkedType(linkedType_)
  , type(GEM_ITEM_VAL)
  , saveAction(saveAction_)
{ }


GEMItem::GEMItem(char* title_, void* linkedVariable_, byte linkedType_, boolean readonly_)
  : title(title_)
  , linkedVariable(linkedVariable_)
  , linkedType(linkedType_)
  , readonly(readonly_)
  , type(GEM_ITEM_VAL)
{ }

GEMItem::GEMItem(char* title_, GEMPage* linkedPage_)
  : title(title_)
  , linkedPage(linkedPage_)
  , type(GEM_ITEM_LINK)
{ }

GEMItem::GEMItem(char* title_, void (*buttonAction_)())
  : title(title_)
  , buttonAction(buttonAction_)
  , type(GEM_ITEM_BUTTON)
{ }

void GEMItem::setReadonly(boolean mode) {
  readonly = mode;
}

boolean GEMItem::getReadonly() {
  return readonly;
}
