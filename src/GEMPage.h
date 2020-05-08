/*
  GEMPage - menu page (or level) for GEM library. Consists of a list of menu items.

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

#ifndef HEADER_GEMPAGE
#define HEADER_GEMPAGE

#include <Arduino.h>
#include "GEMItem.h"

// Declaration of GEMPage class
class GEMPage {
  friend class GEM;
  friend class GEM_u8g2;
  public:
    /* 
      @param 'title_' - title of the menu page displayed at top of the screen
    */
    GEMPage(char* title_ = "");
    void addMenuItem(GEMItem& menuItem);              // Add menu item to menu page
    void setParentMenuPage(GEMPage& parentMenuPage);  // Specify parent level menu page (to know where to go back to when pressing Back button)
  private:
    char* title;
    byte currentItemNum = 0;                          // Currently selected (focused) menu item of the page
    byte itemsCount = 0;
    GEMItem* getMenuItem(byte index);
    GEMItem* getCurrentMenuItem();
    GEMItem* _menuItem;                                         // First menu item of the page (the following ones are linked from within one another)
    GEMItem _menuItemBack {"", static_cast<GEMPage*>(nullptr)}; // Local instance of Back button (created when parent level menu page is specified through
                                                                // setParentMenuPage(); always becomes the first menu item in a list)
};
  
#endif
