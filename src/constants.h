/*
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

// Macro constant (alias) for current version of GEM library, printed on _splash screen
#define GEM_VER "1.3"

// Macro constant (alias) for supported length of the string (character sequence) variable of type char[GEM_STR_LEN]
#define GEM_STR_LEN 17

// Macro constant (alias) for default precision of the float and double variables (the number of digits after the decimal sign as required by dtostrf())
#define GEM_FLOAT_PREC 6
#define GEM_DOUBLE_PREC 6

// Macro constants (aliases) for menu pointer visual appearance
#define GEM_POINTER_DASH 0  // Current menu item is marked with pointer (filled square) to the left of its name
#define GEM_POINTER_ROW 1   // Current menu item is marked with filled background

// Macro constant (alias) for auto menu items per screen option
#define GEM_ITEMS_COUNT_AUTO 0

// Macro constants (aliases) for supported types of associated with menu item variable
#define GEM_VAL_INTEGER 0  // Associated variable is of type int
#define GEM_VAL_BYTE 1     // Associated variable is of type byte
#define GEM_VAL_CHAR 2     // Associated variable is of type char[GEM_STR_LEN]
#define GEM_VAL_BOOLEAN 3  // Associated variable is of type boolean
#define GEM_VAL_SELECT 4   // Associated variable is either of type int, byte or char[] with option select used to pick a predefined value from the list
                           // (note that char[] array should be big enough to hold select option with the longest value)
#define GEM_VAL_FLOAT 5    // Associated variable is of type float
#define GEM_VAL_DOUBLE 6   // Associated variable is of type double
