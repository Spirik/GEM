/*
  GEMPage - menu page (or level) for GEM library. Consists of a list of menu items.

  GEM (a.k.a. Good Enough Menu) - Arduino library for creation of graphic multi-level menu with
  editable menu items, such as variables (supports int, byte, float, double, boolean, char[17] data types)
  and option selects. User-defined callback function can be specified to invoke when menu item is saved.
  
  Supports buttons that can invoke user-defined actions and create action-specific
  context, which can have its own enter (setup) and exit callbacks as well as loop function.

  Supports AltSerialGraphicLCD library by Jon Green (http://www.jasspa.com/serialGLCD.html)
  and U8g2 library by olikraus (https://github.com/olikraus/U8g2_Arduino).

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
  currentItemNum = (_menuItemBack.linkedPage != nullptr) ? 1 : 0;
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
  currentItemNum = (itemsCount > 1) ? 1 : 0;
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

void GEMPage::setTitle(char* title_) {
  title = title_;
}

char* GEMPage::getTitle() {
  return title;
}
