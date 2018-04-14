/*
  GEMSelect - option select for GEM library.

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

#ifndef HEADER_GEMSELECT
#define HEADER_GEMSELECT

// Declaration of SelectOptionInt type
struct SelectOptionInt {
  char* name;    // Text label of the option as displayed in select
  int val_int;   // Value of the option that is assigned to linked variable upon option selection
};

// Declaration of SelectOptionByte type
struct SelectOptionByte {
  char* name;
  byte val_byte;
};

// Declaration of SelectOptionChar type
struct SelectOptionChar {
  char* name;
  char* val_char;
};

// Declaration of GEMSelect class
class GEMSelect {
  friend class GEM;
  public:
    /* 
      @param 'length_' - length of the 'options_' array
      @param 'options_' - array of the available options
    */
    GEMSelect(byte length_, SelectOptionInt* options_);
    GEMSelect(byte length_, SelectOptionByte* options_);
    GEMSelect(byte length_, SelectOptionChar* options_);
  private:
    byte _type;
    byte _length;
    void* _options;
    byte getType();
    byte getLength();
    int getSelectedOptionNum(void* variable);
    char* getSelectedOptionName(void* variable);
    char* getOptionNameByIndex(int index);
    void setValue(void* variable, int index);  // Assign value of the selected option to supplied variable
};
  
#endif
