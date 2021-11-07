/*
  GEMPage - menu page (or level) for GEM library. Consists of a list of menu items.

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
#include "GEMPage.h"

GEMPage::GEMPage(const char* title_, void (*exitAction_)())
  : title(title_)
  , exitAction(exitAction_)
{ }

void GEMPage::addMenuItem(GEMItem& menuItem) {
  // Prevent adding menu item that was already added to another (or the same) page
  if (menuItem.parentPage == nullptr) {
    if (itemsCountTotal == 0) {
      // If menu page is empty, link supplied menu item from within page directly (this will be the first menu item in a page)
      _menuItem = &menuItem;
    } else {
      // If menu page is not empty, link supplied menu item from within the last menu item of the page
      getMenuItem(itemsCountTotal-1, true)->menuItemNext = &menuItem;
    }
    menuItem.parentPage = this;
    if (!menuItem.hidden) {
      itemsCount++;
    }
    itemsCountTotal++;
    currentItemNum = (_menuItemBack.linkedPage != nullptr) ? 1 : 0;
  }
}

void GEMPage::setParentMenuPage(GEMPage& parentMenuPage) {
  _menuItemBack.type = GEM_ITEM_BACK;
  _menuItemBack.linkedPage = &parentMenuPage;
  // Back button menu item should be always inserted at first position in list
  GEMItem* menuItemTmp = _menuItem;
  _menuItem = &_menuItemBack;
  if (menuItemTmp != nullptr) {
    _menuItemBack.menuItemNext = menuItemTmp;
  }
  itemsCount++;
  itemsCountTotal++;
  currentItemNum = (itemsCount > 1) ? 1 : 0;
}

void GEMPage::setTitle(const char* title_) {
  title = title_;
}

const char* GEMPage::getTitle() {
  return title;
}

GEMItem* GEMPage::getMenuItem(byte index, boolean total) {
  GEMItem* menuItemTmp = (!total && _menuItem->hidden) ? _menuItem->getMenuItemNext() : _menuItem;
  for (byte i=0; i<index; i++) {
    menuItemTmp = (total) ? menuItemTmp->menuItemNext : menuItemTmp->getMenuItemNext();
  }
  return menuItemTmp;
}

GEMItem* GEMPage::getCurrentMenuItem() {
  return getMenuItem(currentItemNum);
}

int GEMPage::getMenuItemNum(GEMItem& menuItem) {
  GEMItem* menuItemTmp = (_menuItem->hidden) ? _menuItem->getMenuItemNext() : _menuItem;
  for (byte i=0; i<itemsCount; i++) {
    if (menuItemTmp == &menuItem) {
      return i;
    }
    menuItemTmp = menuItemTmp->getMenuItemNext();
  }
  return -1;
}

void GEMPage::hideMenuItem(GEMItem& menuItem) {
  int menuItemNum = getMenuItemNum(menuItem);
  menuItem.hidden = true;
  itemsCount--;
  if (menuItemNum <= currentItemNum) {
    if (currentItemNum > 0) {
      currentItemNum--;
    }
  }
  if (_menuItemBack.linkedPage != nullptr && itemsCount == 1) {
    currentItemNum = 0;
  }
}

void GEMPage::showMenuItem(GEMItem& menuItem) {
  menuItem.hidden = false;
  itemsCount++;
  int menuItemNum = getMenuItemNum(menuItem);
  if (menuItemNum <= currentItemNum) {
    if (currentItemNum < itemsCount-1) {
      currentItemNum++;
    }
  }
  if (_menuItemBack.linkedPage != nullptr && itemsCount > 1) {
    currentItemNum = 1;
  }
}
