/*
  GEMPage - menu page (or level) for GEM library. Consists of a list of menu items.

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
#include "GEMPage.h"

GEMPage::GEMPage(const char* title_)
  : title(title_)
{ }

GEMPage::GEMPage(const char* title_, void (*exitAction_)())
  : title(title_)
  , exitAction(exitAction_)
{ }

GEMPage::GEMPage(const char* title_, GEMPage& parentMenuPage_)
  : title(title_)
{
  setParentMenuPage(parentMenuPage_);
}

GEMPage& GEMPage::addMenuItem(GEMItem& menuItem, byte pos, bool total) {
  // Prevent adding menu item that was already added to another (or the same) page
  if (menuItem.parentPage == nullptr) {
    byte itemsMax = total ? itemsCountTotal : itemsCount;
    if (pos >= itemsMax) {
      // Cap maximum pos at number of items
      pos = itemsMax;
    } else if (_menuItemBack.linkedPage != nullptr && pos == 0) {
      // Prevent adding supplied menu item in place of Back button
      pos = 1;
    }
    if (pos > 0) {
      // If custom position is defined (and is within range), link supplied menu item from within preceding menu item
      GEMItem* menuItemTmp = getMenuItem(pos-1, total);
      menuItem.menuItemNext = menuItemTmp->menuItemNext;
      menuItemTmp->menuItemNext = &menuItem;
    } else {
      // Link supplied menu item as a first menu item on a page
      menuItem.menuItemNext = _menuItem;
      _menuItem = &menuItem;
    }
    menuItem.parentPage = this;
    itemsCountTotal++;
    if (!menuItem.hidden) {
      itemsCount++;
      currentItemNum = (_menuItemBack.linkedPage != nullptr) ? 1 : 0;
    }
  }
  return *this;
}

GEMPage& GEMPage::setParentMenuPage(GEMPage& parentMenuPage) {
  if (_menuItemBack.linkedPage == nullptr) {
    _menuItemBack.type = GEM_ITEM_BACK;
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
  _menuItemBack.linkedPage = &parentMenuPage;
  return *this;
}

GEMPage& GEMPage::setTitle(const char* title_) {
  title = title_;
  return *this;
}

const char* GEMPage::getTitle() {
  return title;
}

GEMPage& GEMPage::setAppearance(GEMAppearance* appearance) {
  _appearance = appearance;
  return *this;
}

GEMItem* GEMPage::getMenuItem(byte index, bool total) {
  GEMItem* menuItemTmp = (!total && _menuItem->hidden) ? _menuItem->getMenuItemNext() : _menuItem;
  for (byte i=0; i<index; i++) {
    menuItemTmp = menuItemTmp->getMenuItemNext(total);
    if (menuItemTmp == nullptr) {
      return nullptr;
    }
  }
  return menuItemTmp;
}

GEMItem* GEMPage::getCurrentMenuItem() {
  return getMenuItem(currentItemNum);
}

byte GEMPage::getCurrentMenuItemIndex() {
  return currentItemNum;
}

GEMPage& GEMPage::setCurrentMenuItemIndex(byte index) {
  currentItemNum = index;
  return *this;
}

byte GEMPage::getItemsCount(bool total) {
  return total ? itemsCountTotal : itemsCount;
}

int GEMPage::getMenuItemNum(GEMItem& menuItem, bool total) {
  GEMItem* menuItemTmp = (!total && _menuItem->hidden) ? _menuItem->getMenuItemNext() : _menuItem;
  for (byte i=0; i<(total ? itemsCountTotal : itemsCount); i++) {
    if (menuItemTmp == &menuItem) {
      return i;
    }
    menuItemTmp = menuItemTmp->getMenuItemNext(total);
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

void GEMPage::removeMenuItem(GEMItem& menuItem) {
  int menuItemNum = getMenuItemNum(menuItem);
  int menuItemNumTotal = getMenuItemNum(menuItem, true);
  itemsCountTotal--;
  if (!menuItem.hidden) {
    itemsCount--;
    if (menuItemNum <= currentItemNum) {
      if (currentItemNum > 0) {
        currentItemNum--;
      }
    }
  }
  if (_menuItemBack.linkedPage != nullptr && itemsCount == 1) {
    currentItemNum = 0;
  }
  if (menuItemNumTotal > 0) {
    getMenuItem(menuItemNumTotal-1, true)->menuItemNext = menuItem.menuItemNext;
  } else {
    _menuItem = menuItem.menuItemNext;
  }
  menuItem.parentPage = nullptr;
  menuItem.menuItemNext = nullptr;
}