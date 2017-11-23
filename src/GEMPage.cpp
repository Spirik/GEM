/*
  GEMPage - menu page (or level) for GEM library. Consists of a list of menu items.

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
#include "GEMPage.h"

GEMPage::GEMPage(char* title_)
  : title(title_)
{ }

void GEMPage::addMenuItem(GEMItem& menuItem) {
  if (itemsCount == 0) {
    // If menu page is empty, link supplied menu item from within page directly (this will be the first menu item in a page)
    _menuItem = &menuItem;
  } else {
    // If menu page is not empty, link supplied menu item from within the last menu item of the page
    getMenuItem(itemsCount-1)->menuItemNext = &menuItem;
  }
  itemsCount++;
}

void GEMPage::setParentMenuPage(GEMPage& parentMenuPage) {
  _menuItemBack.type = GEM_ITEM_BACK;
  _menuItemBack.linkedPage = &parentMenuPage;
  // Back button menu item should be always inserted at first position in list
  GEMItem* menuItemTmp = _menuItem;
  _menuItem = &_menuItemBack;
  if (menuItemTmp != 0) {
    _menuItemBack.menuItemNext = menuItemTmp;
  }
  itemsCount++;
  currentItemNum = 1;
}

GEMItem* GEMPage::getMenuItem(byte index) {
  GEMItem* menuItemTmp = _menuItem;
  for (byte i=0; i<index; i++) {
    menuItemTmp = menuItemTmp->menuItemNext;
  }
  return menuItemTmp;
}

GEMItem* GEMPage::getCurrentMenuItem() {
  return getMenuItem(currentItemNum);
}
