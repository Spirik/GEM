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

#include "config.h"
#include "constants.h"
#include "GEMPage.h"

#ifndef HEADER_GEMITEM
#define HEADER_GEMITEM

// Macro constants (aliases) for menu item types
#define GEM_ITEM_VAL 0     // Menu item represents associated variable
#define GEM_ITEM_LINK 1    // Menu item represents link to another menu page
#define GEM_ITEM_BACK 2    // Menu item represents Back button (that links to parent level menu page)
#define GEM_ITEM_BUTTON 3  // Menu item represents button (that leads to execution of user-defined routine in its own context)
#define GEM_ITEM_LABEL 4   // Menu item represents non-interactive label

// Macro constant (alias) for readonly modifier of associated with menu item variable
#define GEM_READONLY true

// Macro constant (alias) for hidden state of the menu item
#define GEM_HIDDEN true

// Forward declaration of necessary classes
class GEMItem;
class GEMPage;
class GEMSelect;
#ifdef GEM_SUPPORT_SPINNER
class GEMSpinner;
#endif

// Declaration of GEMCallbackData type
struct GEMCallbackData {
  GEMItem* pMenuItem;     // Pointer to current menu item
  union {                 // User-defined value for callback argument (as one of the following types listed in a union)
    byte valByte;
    int valInt;
    float valFloat;
    double valDouble;
    bool valBoolean;
    bool valBool;
    const char* valChar;
    void* valPointer;
  };
};

// Declaration of GEMItem class
class GEMItem {
  friend class GEM;
  friend class GEM_u8g2;
  friend class GEM_adafruit_gfx;
  friend class GEMPage;
  public:
    /* 
      Constructor for menu item that represents non-interactive label
    */
    GEMItem(const char* title_);
    /* 
      Constructors for menu item that represents option select, w/ callback (optionally w/ user-defined callback argument)
      @param 'title_' - title of the menu item displayed on the screen
      @param 'linkedVariable_' - reference to variable that menu item is associated with (either byte, int, char*, float, or double)
      @param 'select_' - reference to GEMSelect option select
      @param 'callbackAction_' - pointer to callback function executed when associated variable is successfully saved
      @param 'callbackVal_' - value of an argument that will be passed to callback within GEMCallbackData (either byte, int, bool, float, double, char or void pointer)
    */
    GEMItem(const char* title_, byte& linkedVariable_, GEMSelect& select_, void (*callbackAction_)());
    GEMItem(const char* title_, int& linkedVariable_, GEMSelect& select_, void (*callbackAction_)());
    GEMItem(const char* title_, char* linkedVariable_, GEMSelect& select_, void (*callbackAction_)());
    GEMItem(const char* title_, float& linkedVariable_, GEMSelect& select_, void (*callbackAction_)());
    GEMItem(const char* title_, double& linkedVariable_, GEMSelect& select_, void (*callbackAction_)());

    GEMItem(const char* title_, byte& linkedVariable_, GEMSelect& select_, void (*callbackAction_)(GEMCallbackData));
    GEMItem(const char* title_, byte& linkedVariable_, GEMSelect& select_, void (*callbackAction_)(GEMCallbackData), byte callbackVal_);
    GEMItem(const char* title_, byte& linkedVariable_, GEMSelect& select_, void (*callbackAction_)(GEMCallbackData), int callbackVal_);
    GEMItem(const char* title_, byte& linkedVariable_, GEMSelect& select_, void (*callbackAction_)(GEMCallbackData), float callbackVal_);
    GEMItem(const char* title_, byte& linkedVariable_, GEMSelect& select_, void (*callbackAction_)(GEMCallbackData), double callbackVal_);
    GEMItem(const char* title_, byte& linkedVariable_, GEMSelect& select_, void (*callbackAction_)(GEMCallbackData), bool callbackVal_);
    GEMItem(const char* title_, byte& linkedVariable_, GEMSelect& select_, void (*callbackAction_)(GEMCallbackData), const char* callbackVal_);
    GEMItem(const char* title_, byte& linkedVariable_, GEMSelect& select_, void (*callbackAction_)(GEMCallbackData), void* callbackVal_);

    GEMItem(const char* title_, int& linkedVariable_, GEMSelect& select_, void (*callbackAction_)(GEMCallbackData));
    GEMItem(const char* title_, int& linkedVariable_, GEMSelect& select_, void (*callbackAction_)(GEMCallbackData), byte callbackVal_);
    GEMItem(const char* title_, int& linkedVariable_, GEMSelect& select_, void (*callbackAction_)(GEMCallbackData), int callbackVal_);
    GEMItem(const char* title_, int& linkedVariable_, GEMSelect& select_, void (*callbackAction_)(GEMCallbackData), float callbackVal_);
    GEMItem(const char* title_, int& linkedVariable_, GEMSelect& select_, void (*callbackAction_)(GEMCallbackData), double callbackVal_);
    GEMItem(const char* title_, int& linkedVariable_, GEMSelect& select_, void (*callbackAction_)(GEMCallbackData), bool callbackVal_);
    GEMItem(const char* title_, int& linkedVariable_, GEMSelect& select_, void (*callbackAction_)(GEMCallbackData), const char* callbackVal_);
    GEMItem(const char* title_, int& linkedVariable_, GEMSelect& select_, void (*callbackAction_)(GEMCallbackData), void* callbackVal_);

    GEMItem(const char* title_, char* linkedVariable_, GEMSelect& select_, void (*callbackAction_)(GEMCallbackData));
    GEMItem(const char* title_, char* linkedVariable_, GEMSelect& select_, void (*callbackAction_)(GEMCallbackData), byte callbackVal_);
    GEMItem(const char* title_, char* linkedVariable_, GEMSelect& select_, void (*callbackAction_)(GEMCallbackData), int callbackVal_);
    GEMItem(const char* title_, char* linkedVariable_, GEMSelect& select_, void (*callbackAction_)(GEMCallbackData), float callbackVal_);
    GEMItem(const char* title_, char* linkedVariable_, GEMSelect& select_, void (*callbackAction_)(GEMCallbackData), double callbackVal_);
    GEMItem(const char* title_, char* linkedVariable_, GEMSelect& select_, void (*callbackAction_)(GEMCallbackData), bool callbackVal_);
    GEMItem(const char* title_, char* linkedVariable_, GEMSelect& select_, void (*callbackAction_)(GEMCallbackData), const char* callbackVal_);
    GEMItem(const char* title_, char* linkedVariable_, GEMSelect& select_, void (*callbackAction_)(GEMCallbackData), void* callbackVal_);

    GEMItem(const char* title_, float& linkedVariable_, GEMSelect& select_, void (*callbackAction_)(GEMCallbackData));
    GEMItem(const char* title_, float& linkedVariable_, GEMSelect& select_, void (*callbackAction_)(GEMCallbackData), byte callbackVal_);
    GEMItem(const char* title_, float& linkedVariable_, GEMSelect& select_, void (*callbackAction_)(GEMCallbackData), int callbackVal_);
    GEMItem(const char* title_, float& linkedVariable_, GEMSelect& select_, void (*callbackAction_)(GEMCallbackData), float callbackVal_);
    GEMItem(const char* title_, float& linkedVariable_, GEMSelect& select_, void (*callbackAction_)(GEMCallbackData), double callbackVal_);
    GEMItem(const char* title_, float& linkedVariable_, GEMSelect& select_, void (*callbackAction_)(GEMCallbackData), bool callbackVal_);
    GEMItem(const char* title_, float& linkedVariable_, GEMSelect& select_, void (*callbackAction_)(GEMCallbackData), const char* callbackVal_);
    GEMItem(const char* title_, float& linkedVariable_, GEMSelect& select_, void (*callbackAction_)(GEMCallbackData), void* callbackVal_);

    GEMItem(const char* title_, double& linkedVariable_, GEMSelect& select_, void (*callbackAction_)(GEMCallbackData));
    GEMItem(const char* title_, double& linkedVariable_, GEMSelect& select_, void (*callbackAction_)(GEMCallbackData), byte callbackVal_);
    GEMItem(const char* title_, double& linkedVariable_, GEMSelect& select_, void (*callbackAction_)(GEMCallbackData), int callbackVal_);
    GEMItem(const char* title_, double& linkedVariable_, GEMSelect& select_, void (*callbackAction_)(GEMCallbackData), float callbackVal_);
    GEMItem(const char* title_, double& linkedVariable_, GEMSelect& select_, void (*callbackAction_)(GEMCallbackData), double callbackVal_);
    GEMItem(const char* title_, double& linkedVariable_, GEMSelect& select_, void (*callbackAction_)(GEMCallbackData), bool callbackVal_);
    GEMItem(const char* title_, double& linkedVariable_, GEMSelect& select_, void (*callbackAction_)(GEMCallbackData), const char* callbackVal_);
    GEMItem(const char* title_, double& linkedVariable_, GEMSelect& select_, void (*callbackAction_)(GEMCallbackData), void* callbackVal_);
    /* 
      Constructors for menu item that represents option select, w/o callback
      @param 'title_' - title of the menu item displayed on the screen
      @param 'linkedVariable_' - reference to variable that menu item is associated with (either byte, int, char*, float, or double)
      @param 'select_' - reference to GEMSelect option select
      @param 'readonly_' (optional) - set readonly mode for variable that option select is associated with
      values GEM_READONLY (alias for true)
      default false
    */
    GEMItem(const char* title_, byte& linkedVariable_, GEMSelect& select_, bool readonly_ = false);
    GEMItem(const char* title_, int& linkedVariable_, GEMSelect& select_, bool readonly_ = false);
    GEMItem(const char* title_, char* linkedVariable_, GEMSelect& select_, bool readonly_ = false);
    GEMItem(const char* title_, float& linkedVariable_, GEMSelect& select_, bool readonly_ = false);
    GEMItem(const char* title_, double& linkedVariable_, GEMSelect& select_, bool readonly_ = false);
    #ifdef GEM_SUPPORT_SPINNER
    /* 
      Constructors for menu item that represents spinner, w/ callback (optionally w/ user-defined callback argument)
      @param 'title_' - title of the menu item displayed on the screen
      @param 'linkedVariable_' - reference to variable that menu item is associated with (either byte, int, float, or double)
      @param 'spinner_' - reference to GEMSpinner object
      @param 'callbackAction_' - pointer to callback function executed when associated variable is successfully saved
      @param 'callbackVal_' - value of an argument that will be passed to callback within GEMCallbackData (either byte, int, bool, float, double, char or void pointer)
    */
    GEMItem(const char* title_, byte& linkedVariable_, GEMSpinner& spinner_, void (*callbackAction_)());
    GEMItem(const char* title_, int& linkedVariable_, GEMSpinner& spinner_, void (*callbackAction_)());
    GEMItem(const char* title_, float& linkedVariable_, GEMSpinner& spinner_, void (*callbackAction_)());
    GEMItem(const char* title_, double& linkedVariable_, GEMSpinner& spinner_, void (*callbackAction_)());

    GEMItem(const char* title_, byte& linkedVariable_, GEMSpinner& spinner_, void (*callbackAction_)(GEMCallbackData));
    GEMItem(const char* title_, byte& linkedVariable_, GEMSpinner& spinner_, void (*callbackAction_)(GEMCallbackData), byte callbackVal_);
    GEMItem(const char* title_, byte& linkedVariable_, GEMSpinner& spinner_, void (*callbackAction_)(GEMCallbackData), int callbackVal_);
    GEMItem(const char* title_, byte& linkedVariable_, GEMSpinner& spinner_, void (*callbackAction_)(GEMCallbackData), float callbackVal_);
    GEMItem(const char* title_, byte& linkedVariable_, GEMSpinner& spinner_, void (*callbackAction_)(GEMCallbackData), double callbackVal_);
    GEMItem(const char* title_, byte& linkedVariable_, GEMSpinner& spinner_, void (*callbackAction_)(GEMCallbackData), bool callbackVal_);
    GEMItem(const char* title_, byte& linkedVariable_, GEMSpinner& spinner_, void (*callbackAction_)(GEMCallbackData), const char* callbackVal_);
    GEMItem(const char* title_, byte& linkedVariable_, GEMSpinner& spinner_, void (*callbackAction_)(GEMCallbackData), void* callbackVal_);

    GEMItem(const char* title_, int& linkedVariable_, GEMSpinner& spinner_, void (*callbackAction_)(GEMCallbackData));
    GEMItem(const char* title_, int& linkedVariable_, GEMSpinner& spinner_, void (*callbackAction_)(GEMCallbackData), byte callbackVal_);
    GEMItem(const char* title_, int& linkedVariable_, GEMSpinner& spinner_, void (*callbackAction_)(GEMCallbackData), int callbackVal_);
    GEMItem(const char* title_, int& linkedVariable_, GEMSpinner& spinner_, void (*callbackAction_)(GEMCallbackData), float callbackVal_);
    GEMItem(const char* title_, int& linkedVariable_, GEMSpinner& spinner_, void (*callbackAction_)(GEMCallbackData), double callbackVal_);
    GEMItem(const char* title_, int& linkedVariable_, GEMSpinner& spinner_, void (*callbackAction_)(GEMCallbackData), bool callbackVal_);
    GEMItem(const char* title_, int& linkedVariable_, GEMSpinner& spinner_, void (*callbackAction_)(GEMCallbackData), const char* callbackVal_);
    GEMItem(const char* title_, int& linkedVariable_, GEMSpinner& spinner_, void (*callbackAction_)(GEMCallbackData), void* callbackVal_);

    GEMItem(const char* title_, float& linkedVariable_, GEMSpinner& spinner_, void (*callbackAction_)(GEMCallbackData));
    GEMItem(const char* title_, float& linkedVariable_, GEMSpinner& spinner_, void (*callbackAction_)(GEMCallbackData), byte callbackVal_);
    GEMItem(const char* title_, float& linkedVariable_, GEMSpinner& spinner_, void (*callbackAction_)(GEMCallbackData), int callbackVal_);
    GEMItem(const char* title_, float& linkedVariable_, GEMSpinner& spinner_, void (*callbackAction_)(GEMCallbackData), float callbackVal_);
    GEMItem(const char* title_, float& linkedVariable_, GEMSpinner& spinner_, void (*callbackAction_)(GEMCallbackData), double callbackVal_);
    GEMItem(const char* title_, float& linkedVariable_, GEMSpinner& spinner_, void (*callbackAction_)(GEMCallbackData), bool callbackVal_);
    GEMItem(const char* title_, float& linkedVariable_, GEMSpinner& spinner_, void (*callbackAction_)(GEMCallbackData), const char* callbackVal_);
    GEMItem(const char* title_, float& linkedVariable_, GEMSpinner& spinner_, void (*callbackAction_)(GEMCallbackData), void* callbackVal_);

    GEMItem(const char* title_, double& linkedVariable_, GEMSpinner& spinner_, void (*callbackAction_)(GEMCallbackData));
    GEMItem(const char* title_, double& linkedVariable_, GEMSpinner& spinner_, void (*callbackAction_)(GEMCallbackData), byte callbackVal_);
    GEMItem(const char* title_, double& linkedVariable_, GEMSpinner& spinner_, void (*callbackAction_)(GEMCallbackData), int callbackVal_);
    GEMItem(const char* title_, double& linkedVariable_, GEMSpinner& spinner_, void (*callbackAction_)(GEMCallbackData), float callbackVal_);
    GEMItem(const char* title_, double& linkedVariable_, GEMSpinner& spinner_, void (*callbackAction_)(GEMCallbackData), double callbackVal_);
    GEMItem(const char* title_, double& linkedVariable_, GEMSpinner& spinner_, void (*callbackAction_)(GEMCallbackData), bool callbackVal_);
    GEMItem(const char* title_, double& linkedVariable_, GEMSpinner& spinner_, void (*callbackAction_)(GEMCallbackData), const char* callbackVal_);
    GEMItem(const char* title_, double& linkedVariable_, GEMSpinner& spinner_, void (*callbackAction_)(GEMCallbackData), void* callbackVal_);
    /* 
      Constructors for menu item that represents spinner, w/o callback
      @param 'title_' - title of the menu item displayed on the screen
      @param 'linkedVariable_' - reference to variable that menu item is associated with (either byte, int, float, or double)
      @param 'spinner_' - reference to GEMSpinner object
      @param 'readonly_' (optional) - set readonly mode for variable that spinner is associated with
      values GEM_READONLY (alias for true)
      default false
    */
    GEMItem(const char* title_, byte& linkedVariable_, GEMSpinner& spinner_, bool readonly_ = false);
    GEMItem(const char* title_, int& linkedVariable_, GEMSpinner& spinner_, bool readonly_ = false);
    GEMItem(const char* title_, float& linkedVariable_, GEMSpinner& spinner_, bool readonly_ = false);
    GEMItem(const char* title_, double& linkedVariable_, GEMSpinner& spinner_, bool readonly_ = false);
    #endif
    /* 
      Constructors for menu item that represents variable, w/ callback (optionally w/ user-defined callback argument)
      @param 'title_' - title of the menu item displayed on the screen
      @param 'linkedVariable_' - reference to variable that menu item is associated with (either byte, int, char*, bool, float, or double)
      @param 'callbackAction_' - pointer to callback function executed when associated variable is successfully saved
      @param 'callbackVal_' - value of an argument that will be passed to callback within GEMCallbackData (either byte, int, bool, float, double, char or void pointer)
    */
    GEMItem(const char* title_, byte& linkedVariable_, void (*callbackAction_)());
    GEMItem(const char* title_, int& linkedVariable_, void (*callbackAction_)());
    GEMItem(const char* title_, char* linkedVariable_, void (*callbackAction_)());
    GEMItem(const char* title_, bool& linkedVariable_, void (*callbackAction_)());
    GEMItem(const char* title_, float& linkedVariable_, void (*callbackAction_)());
    GEMItem(const char* title_, double& linkedVariable_, void (*callbackAction_)());

    GEMItem(const char* title_, byte& linkedVariable_, void (*callbackAction_)(GEMCallbackData));
    GEMItem(const char* title_, byte& linkedVariable_, void (*callbackAction_)(GEMCallbackData), byte callbackVal_);
    GEMItem(const char* title_, byte& linkedVariable_, void (*callbackAction_)(GEMCallbackData), int callbackVal_);
    GEMItem(const char* title_, byte& linkedVariable_, void (*callbackAction_)(GEMCallbackData), float callbackVal_);
    GEMItem(const char* title_, byte& linkedVariable_, void (*callbackAction_)(GEMCallbackData), double callbackVal_);
    GEMItem(const char* title_, byte& linkedVariable_, void (*callbackAction_)(GEMCallbackData), bool callbackVal_);
    GEMItem(const char* title_, byte& linkedVariable_, void (*callbackAction_)(GEMCallbackData), const char* callbackVal_);
    GEMItem(const char* title_, byte& linkedVariable_, void (*callbackAction_)(GEMCallbackData), void* callbackVal_);

    GEMItem(const char* title_, int& linkedVariable_, void (*callbackAction_)(GEMCallbackData));
    GEMItem(const char* title_, int& linkedVariable_, void (*callbackAction_)(GEMCallbackData), byte callbackVal_);
    GEMItem(const char* title_, int& linkedVariable_, void (*callbackAction_)(GEMCallbackData), int callbackVal_);
    GEMItem(const char* title_, int& linkedVariable_, void (*callbackAction_)(GEMCallbackData), float callbackVal_);
    GEMItem(const char* title_, int& linkedVariable_, void (*callbackAction_)(GEMCallbackData), double callbackVal_);
    GEMItem(const char* title_, int& linkedVariable_, void (*callbackAction_)(GEMCallbackData), bool callbackVal_);
    GEMItem(const char* title_, int& linkedVariable_, void (*callbackAction_)(GEMCallbackData), const char* callbackVal_);
    GEMItem(const char* title_, int& linkedVariable_, void (*callbackAction_)(GEMCallbackData), void* callbackVal_);

    GEMItem(const char* title_, char* linkedVariable_, void (*callbackAction_)(GEMCallbackData));
    GEMItem(const char* title_, char* linkedVariable_, void (*callbackAction_)(GEMCallbackData), byte callbackVal_);
    GEMItem(const char* title_, char* linkedVariable_, void (*callbackAction_)(GEMCallbackData), int callbackVal_);
    GEMItem(const char* title_, char* linkedVariable_, void (*callbackAction_)(GEMCallbackData), float callbackVal_);
    GEMItem(const char* title_, char* linkedVariable_, void (*callbackAction_)(GEMCallbackData), double callbackVal_);
    GEMItem(const char* title_, char* linkedVariable_, void (*callbackAction_)(GEMCallbackData), bool callbackVal_);
    GEMItem(const char* title_, char* linkedVariable_, void (*callbackAction_)(GEMCallbackData), const char* callbackVal_);
    GEMItem(const char* title_, char* linkedVariable_, void (*callbackAction_)(GEMCallbackData), void* callbackVal_);

    GEMItem(const char* title_, bool& linkedVariable_, void (*callbackAction_)(GEMCallbackData));
    GEMItem(const char* title_, bool& linkedVariable_, void (*callbackAction_)(GEMCallbackData), byte callbackVal_);
    GEMItem(const char* title_, bool& linkedVariable_, void (*callbackAction_)(GEMCallbackData), int callbackVal_);
    GEMItem(const char* title_, bool& linkedVariable_, void (*callbackAction_)(GEMCallbackData), float callbackVal_);
    GEMItem(const char* title_, bool& linkedVariable_, void (*callbackAction_)(GEMCallbackData), double callbackVal_);
    GEMItem(const char* title_, bool& linkedVariable_, void (*callbackAction_)(GEMCallbackData), bool callbackVal_);
    GEMItem(const char* title_, bool& linkedVariable_, void (*callbackAction_)(GEMCallbackData), const char* callbackVal_);
    GEMItem(const char* title_, bool& linkedVariable_, void (*callbackAction_)(GEMCallbackData), void* callbackVal_);

    GEMItem(const char* title_, float& linkedVariable_, void (*callbackAction_)(GEMCallbackData));
    GEMItem(const char* title_, float& linkedVariable_, void (*callbackAction_)(GEMCallbackData), byte callbackVal_);
    GEMItem(const char* title_, float& linkedVariable_, void (*callbackAction_)(GEMCallbackData), int callbackVal_);
    GEMItem(const char* title_, float& linkedVariable_, void (*callbackAction_)(GEMCallbackData), float callbackVal_);
    GEMItem(const char* title_, float& linkedVariable_, void (*callbackAction_)(GEMCallbackData), double callbackVal_);
    GEMItem(const char* title_, float& linkedVariable_, void (*callbackAction_)(GEMCallbackData), bool callbackVal_);
    GEMItem(const char* title_, float& linkedVariable_, void (*callbackAction_)(GEMCallbackData), const char* callbackVal_);
    GEMItem(const char* title_, float& linkedVariable_, void (*callbackAction_)(GEMCallbackData), void* callbackVal_);

    GEMItem(const char* title_, double& linkedVariable_, void (*callbackAction_)(GEMCallbackData));
    GEMItem(const char* title_, double& linkedVariable_, void (*callbackAction_)(GEMCallbackData), byte callbackVal_);
    GEMItem(const char* title_, double& linkedVariable_, void (*callbackAction_)(GEMCallbackData), int callbackVal_);
    GEMItem(const char* title_, double& linkedVariable_, void (*callbackAction_)(GEMCallbackData), float callbackVal_);
    GEMItem(const char* title_, double& linkedVariable_, void (*callbackAction_)(GEMCallbackData), double callbackVal_);
    GEMItem(const char* title_, double& linkedVariable_, void (*callbackAction_)(GEMCallbackData), bool callbackVal_);
    GEMItem(const char* title_, double& linkedVariable_, void (*callbackAction_)(GEMCallbackData), const char* callbackVal_);
    GEMItem(const char* title_, double& linkedVariable_, void (*callbackAction_)(GEMCallbackData), void* callbackVal_);
    /* 
      Constructors for menu item that represents variable, w/o callback
      @param 'title_' - title of the menu item displayed on the screen
      @param 'linkedVariable_' - reference to variable that menu item is associated with (either byte, int, char*, bool, float, or double)
      @param 'readonly_' (optional) - set readonly mode for variable that menu item is associated with
      values GEM_READONLY (alias for true)
      default false
    */
    GEMItem(const char* title_, byte& linkedVariable_, bool readonly_ = false);
    GEMItem(const char* title_, int& linkedVariable_, bool readonly_ = false);
    GEMItem(const char* title_, char* linkedVariable_, bool readonly_ = false);
    GEMItem(const char* title_, bool& linkedVariable_, bool readonly_ = false);
    GEMItem(const char* title_, float& linkedVariable_, bool readonly_ = false);
    GEMItem(const char* title_, double& linkedVariable_, bool readonly_ = false);
    /* 
      Constructor for menu item that represents link to another menu page (via reference)
      @param 'title_' - title of the menu item displayed on the screen
      @param 'linkedPage_' - reference to GEMPage menu page that menu item is associated with
      @param 'readonly_' (optional) - set readonly mode for the link (user won't be able to navigate to linked page)
      values GEM_READONLY (alias for true)
    */
    GEMItem(const char* title_, GEMPage& linkedPage_, bool readonly_ = false);
    /* 
      Constructor for menu item that represents link to another menu page (via pointer)
      @param 'title_' - title of the menu item displayed on the screen
      @param 'linkedPage_' - pointer to GEMPage menu page that menu item is associated with
      @param 'readonly_' (optional) - set readonly mode for the link (user won't be able to navigate to linked page)
      values GEM_READONLY (alias for true)
    */
    GEMItem(const char* title_, GEMPage* linkedPage_, bool readonly_ = false);
    /* 
      Constructor for menu item that represents button w/ callback (optionally w/ user-defined callback argument)
      @param 'title_' - title of the menu item displayed on the screen
      @param 'callbackAction_' - pointer to function that will be executed when menu item is activated
      @param 'callbackVal_' - value of an argument that will be passed to callback within GEMCallbackData (either byte, int, bool, float, double, char or void pointer)
      @param 'readonly_' (optional) - set readonly mode for the button (user won't be able to call action associated with it)
      values GEM_READONLY (alias for true)
    */
    GEMItem(const char* title_, void (*callbackAction_)(), bool readonly_ = false);
    GEMItem(const char* title_, void (*callbackAction_)(GEMCallbackData));
    GEMItem(const char* title_, void (*callbackAction_)(GEMCallbackData), byte callbackVal_, bool readonly_ = false);
    GEMItem(const char* title_, void (*callbackAction_)(GEMCallbackData), int callbackVal_, bool readonly_ = false);
    GEMItem(const char* title_, void (*callbackAction_)(GEMCallbackData), float callbackVal_, bool readonly_ = false);
    GEMItem(const char* title_, void (*callbackAction_)(GEMCallbackData), double callbackVal_, bool readonly_ = false);
    GEMItem(const char* title_, void (*callbackAction_)(GEMCallbackData), bool callbackVal_, bool readonly_ = false);
    GEMItem(const char* title_, void (*callbackAction_)(GEMCallbackData), const char* callbackVal_, bool readonly_ = false);
    GEMItem(const char* title_, void (*callbackAction_)(GEMCallbackData), void* callbackVal_, bool readonly_ = false);

    GEMItem& setCallbackVal(byte callbackVal_);         // Set value of an argument that will be passed to callback within GEMCallbackData (either byte, int, bool, float, double, char or void pointer)
    GEMItem& setCallbackVal(int callbackVal_);
    GEMItem& setCallbackVal(float callbackVal_);
    GEMItem& setCallbackVal(double callbackVal_);
    GEMItem& setCallbackVal(bool callbackVal_);
    GEMItem& setCallbackVal(const char* callbackVal_);
    GEMItem& setCallbackVal(void* callbackVal_);
    GEMCallbackData getCallbackData();                  // Get GEMCallbackData struct associated with menu item
    GEM_VIRTUAL GEMItem& setTitle(const char* title_);  // Set title of the menu item
    GEM_VIRTUAL const char* getTitle();                 // Get title of the menu item
    byte getLinkedType();                               // Get type of linked variable (see linkedType field description below for possible values)
    byte getType();                                     // Get type of menu item (see type field description below for possible values)
    GEMItem& setPrecision(byte prec);                   // Explicitly set precision for float or double variables as required by dtostrf() conversion,
                                                        // i.e. the number of digits after the decimal sign
    GEMItem& setAdjustedASCIIOrder(bool mode = true);   // Turn adjsuted order of characters when editing char[17] variables on (with space character followed by `a` and preceded by `) or off
    GEMItem& setReadonly(bool mode = true);             // Explicitly set or unset readonly mode for variable that menu item is associated with
                                                        // (relevant for GEM_VAL_INTEGER, GEM_VAL_BYTE, GEM_VAL_FLOAT, GEM_VAL_DOUBLE, GEM_VAL_CHAR,
                                                        // GEM_VAL_BOOL variable menu items, GEM_VAL_SELECT option select and GEM_VAL_SPINNER spinner), or menu button GEM_ITEM_BUTTON
                                                        // and menu link GEM_ITEM_LINK, pressing of which won't result in any action, associated with them
    bool getReadonly();                                 // Get readonly state of the variable that menu item is associated with (as well as menu link or button)
    GEMItem& hide(bool hide = true);                    // Explicitly hide or show menu item
    GEMItem& show();                                    // Explicitly show menu item
    bool getHidden();                                   // Get hidden state of the menu item
    GEMItem& remove();                                  // Remove menu item from parent menu page
    GEM_VIRTUAL void* getLinkedVariablePointer();       // Get pointer to a linked variable (relevant for menu items that represent variable)
    GEM_VIRTUAL GEMPage* getParentPage();               // Get pointer to menu page that holds this menu item
    GEM_VIRTUAL GEMPage* getLinkedPage();               // Get pointer to menu page that menu link GEM_ITEM_LINK or back button GEM_ITEM_BACK links to
    GEM_VIRTUAL GEMItem* getMenuItemNext(bool total = false); // Get next menu item (including hidden ones if total set to true)
  protected:
    const char* title;
    void* linkedVariable = nullptr;
    byte linkedType;                                    // GEM_VAL_INTEGER, GEM_VAL_BYTE, GEM_VAL_CHAR, GEM_VAL_BOOL, GEM_VAL_SELECT, GEM_VAL_FLOAT, GEM_VAL_DOUBLE, GEM_VAL_SPINNER
    byte type;                                          // GEM_ITEM_VAL, GEM_ITEM_LINK, GEM_ITEM_BACK, GEM_ITEM_BUTTON, GEM_ITEM_LABEL
    byte precision = GEM_FLOAT_PREC;
    bool adjustedAsciiOrder = false;
    bool readonly = false;
    bool hidden = false;
    GEMSelect* select = nullptr;
    #ifdef GEM_SUPPORT_SPINNER
    GEMSpinner* spinner = nullptr;
    #endif
    GEMPage* parentPage = nullptr;
    GEMPage* linkedPage = nullptr;
    GEMItem* menuItemNext = nullptr;
    union {
      void (*callbackAction)() = nullptr;
      void (*callbackActionArg)(GEMCallbackData);
    };
    bool callbackWithArgs = false;
    GEMCallbackData callbackData;
};
  
#endif
