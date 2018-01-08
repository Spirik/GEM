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

#ifndef HEADER_GEMITEM
#define HEADER_GEMITEM

// Macro constants (aliases) for menu item types
#define GEM_ITEM_VAL 0     // Menu item represents associated variable
#define GEM_ITEM_LINK 1    // Menu item represents link to another menu page
#define GEM_ITEM_BACK 2    // Menu item represents Back button (that links to parent level menu page)
#define GEM_ITEM_BUTTON 3  // Menu item represents button (that leads to execution of user-defined routine in its own context)

// Macro constant (alias) for readonly modifier of associated with menu item variable
#define GEM_READONLY true

// Forward declaration of necessary classes
class GEM;
class GEMPage;
class GEMSelect;

// Declaration of GEMItem class
class GEMItem {
  friend class GEM;
  friend class GEMPage;
  public:
    /* 
      Constructors for menu item that represents option select, w/ callback
      @param 'title_' - title of the menu item displayed on the screen
      @param 'linkedVariable_' - reference to variable that menu item is associated with (either byte, int, or char*)
      @param 'select_' - reference to GEMSelect option select
      @param 'saveAction_' - pointer to callback function executed when associated variable is successfully saved
    */
    GEMItem(char* title_, byte& linkedVariable_, GEMSelect& select_, void (*saveAction_)());
    GEMItem(char* title_, int& linkedVariable_, GEMSelect& select_, void (*saveAction_)());
    GEMItem(char* title_, char* linkedVariable_, GEMSelect& select_, void (*saveAction_)());
    /* 
      Constructors for menu item that represents option select, w/o callback
      @param 'title_' - title of the menu item displayed on the screen
      @param 'linkedVariable_' - reference to variable that menu item is associated with (either byte, int, or char*)
      @param 'select_' - reference to GEMSelect option select
      @param 'readonly_' (optional) - set readonly mode for variable that option select is associated with
      values GEM_READONLY (alias for true)
      default false
    */
    GEMItem(char* title_, byte& linkedVariable_, GEMSelect& select_, boolean readonly_ = false);
    GEMItem(char* title_, int& linkedVariable_, GEMSelect& select_, boolean readonly_ = false);
    GEMItem(char* title_, char* linkedVariable_, GEMSelect& select_, boolean readonly_ = false);
    /* 
      Constructors for menu item that represents variable, w/ callback
      @param 'title_' - title of the menu item displayed on the screen
      @param 'linkedVariable_' - reference to variable that menu item is associated with (either byte, int, char*, or boolean)
      @param 'saveAction_' - pointer to callback function executed when associated variable is successfully saved
    */
    GEMItem(char* title_, byte& linkedVariable_, void (*saveAction_)());
    GEMItem(char* title_, int& linkedVariable_, void (*saveAction_)());
    GEMItem(char* title_, char* linkedVariable_, void (*saveAction_)());
    GEMItem(char* title_, boolean& linkedVariable_, void (*saveAction_)());
    /* 
      Constructors for menu item that represents variable, w/o callback
      @param 'title_' - title of the menu item displayed on the screen
      @param 'linkedVariable_' - reference to variable that menu item is associated with (either byte, int, char*, or boolean)
      @param 'readonly_' (optional) - set readonly mode for variable that menu item is associated with
      values GEM_READONLY (alias for true)
      default false
    */
    GEMItem(char* title_, byte& linkedVariable_, boolean readonly_ = false);
    GEMItem(char* title_, int& linkedVariable_, boolean readonly_ = false);
    GEMItem(char* title_, char* linkedVariable_, boolean readonly_ = false);
    GEMItem(char* title_, boolean& linkedVariable_, boolean readonly_ = false);
    /* 
      Constructor for menu item that represents link to another menu page (via reference)
      @param 'title_' - title of the menu item displayed on the screen
      @param 'linkedPage_' - reference to GEMPage menu page that menu item is associated with
    */
    GEMItem(char* title_, GEMPage& linkedPage_);
    /* 
      Constructor for menu item that represents link to another menu page (via pointer)
      @param 'title_' - title of the menu item displayed on the screen
      @param 'linkedPage_' - pointer to GEMPage menu page that menu item is associated with
    */
    GEMItem(char* title_, GEMPage* linkedPage_);
    /* 
      Constructor for menu item that represents button
      @param 'title_' - title of the menu item displayed on the screen
      @param 'buttonAction_' - reference to function that will be executed when menu item is activated
    */
    GEMItem(char* title_, void (&buttonAction_)());
    void setReadonly(boolean mode = true);  // Explicitly set or unset readonly mode for variable that menu item is associated with
                                            // (relevant for GEM_VAL_INTEGER, GEM_VAL_BYTE, GEM_VAL_CHAR, GEM_VAL_BOOLEAN variable
                                            // menu items and GEM_VAL_SELECT option select)
    boolean getReadonly();                  // Get readonly state for variable that menu item is associated with
  private:
    char* title;
    byte type;
    void* linkedVariable;
    byte linkedType;
    boolean readonly = false;
    GEMSelect* select;
    GEMPage* linkedPage;
    GEMItem* menuItemNext;
    void (&buttonAction)();
    void (*saveAction)();
};
  
#endif
