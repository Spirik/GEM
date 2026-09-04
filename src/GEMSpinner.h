/*
  GEMSpinner - increment/decrement spinner for GEM library (similar to GEMSelect).

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

  Copyright (c) 2018-2026 Alexander 'Spirik' Spiridonov

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

#ifndef HEADER_GEMSPINNER
#define HEADER_GEMSPINNER

#include "config.h"
#include "constants.h"

// Forward declaration of necessary classes
class GEMItem;

// Declaration of GEMSpinnerBoundariesByte type
struct GEMSpinnerBoundariesByte {
  byte step;  // Step of the increment/decrement
  byte min;   // Min allowed value
  byte max;   // Max allowed value
};

// Declaration of GEMSpinnerBoundariesInt type
struct GEMSpinnerBoundariesInt {
  int step;
  int min;
  int max;
};

#ifdef GEM_SUPPORT_FLOAT_EDIT
// Declaration of GEMSpinnerBoundariesFloat type
struct GEMSpinnerBoundariesFloat {
  float step;
  float min;
  float max;
};

// Declaration of GEMSpinnerBoundariesDouble type
struct GEMSpinnerBoundariesDouble {
  double step;
  double min;
  double max;
};
#endif

// Declaration of GEMSpinnerBoundaries type
struct GEMSpinnerBoundaries {
  union {
    GEMSpinnerBoundariesByte boundariesByte;
    GEMSpinnerBoundariesInt boundariesInt;
    #ifdef GEM_SUPPORT_FLOAT_EDIT
    GEMSpinnerBoundariesFloat boundariesFloat;
    GEMSpinnerBoundariesDouble boundariesDouble;
    #endif
  };
};

// Declaration of GEMSpinnerValue type
struct GEMSpinnerValue {
  union {
    byte valByte;
    int valInt;
    #ifdef GEM_SUPPORT_FLOAT_EDIT
    float valFloat;
    double valDouble;
    #endif
  };
};

// Declaration of GEMSpinner class
class GEMSpinner {
  friend class GEM;
  friend class GEM_u8g2;
  friend class GEM_adafruit_gfx;
  public:
    /* 
      @param 'boundaries_' - boundaries of the spinner of corresponding type
      @param 'loop_' (optional) - whether iteration over options should be looped
      values GEM_LOOP (alias for true)
      @param 'produceOptionNameByIndexCallback_' (optional) - pointer to callback function executed before option name is printed on the screen, allows to override option name being printed
    */
    GEMSpinner(GEMSpinnerBoundariesByte boundaries_, bool loop_ = false, const char* (*produceOptionNameByIndexCallback_)(GEMSpinner*, int, GEMItem*) = nullptr);
    GEMSpinner(GEMSpinnerBoundariesInt boundaries_, bool loop_ = false, const char* (*produceOptionNameByIndexCallback_)(GEMSpinner*, int, GEMItem*) = nullptr);
    #ifdef GEM_SUPPORT_FLOAT_EDIT
    GEMSpinner(GEMSpinnerBoundariesFloat boundaries_, bool loop_ = false, const char* (*produceOptionNameByIndexCallback_)(GEMSpinner*, int, GEMItem*) = nullptr);
    GEMSpinner(GEMSpinnerBoundariesDouble boundaries_, bool loop_ = false, const char* (*produceOptionNameByIndexCallback_)(GEMSpinner*, int, GEMItem*) = nullptr);
    #endif
    GEMSpinner& setLoop(bool mode = true);  // Explicitly set or unset loop mode
    bool getLoop();                         // Get current value of loop mode
    byte getType();                         // Get type of variable this spinner is suitable for
    int getLength();                        // Get total number of available options
    GEM_VIRTUAL int getSelectedOptionNum(void* variable); // Get selected option index for supplied variable (make sure that variable value is compatible with range of a spinner)
    GEM_VIRTUAL GEMSpinnerValue getOptionNameByIndex(void* variable, int index);  // Get option by its index
    GEM_VIRTUAL void optionNameToCharBuffer(GEMSpinnerValue& value, char* stringBuffer, int valuePrecision = GEM_FLOAT_PREC);  // Copy option name to provided char buffer
    GEMSpinner& setProduceOptionNameByIndexCallback(const char* (*produceOptionNameByIndexCallback_)(GEMSpinner* pSpinner, int index, GEMItem* menuItem)); // Set callback that will be called before option name is printed on the screen
    GEMSpinner& removeProduceOptionNameByIndexCallback(); // Remove callback that was called before option name is printed on the screen
  protected:
    GEMSpinnerBoundaries _boundaries;
    byte _type;
    int _length;
    bool _loop = false;
    GEM_VIRTUAL void setValue(void* variable, int index, void* referenceVariable = nullptr);  // Assign value of the selected option to supplied variable, optionally setting reference variable for counting options
    const char* (*produceOptionNameByIndexCallback)(GEMSpinner* pSpinner, int index, GEMItem* menuItem) = nullptr;
};
  
#endif
