#ifndef GEM_EXCLUDE_GLCD
#include "config/enable-glcd.h"         // Enable AltSerialGraphicLCD version of GEM
#endif

#ifndef GEM_EXCLUDE_U8G2
#include "config/enable-u8g2.h"         // Enable U8g2 version of GEM
#endif

#ifndef GEM_EXCLUDE_ADAFRUIT
#include "config/enable-adafruit-gfx.h" // Enable Adafruit GFX version of GEM
#endif

#ifndef GEM_EXCLUDE_FLOAT_EDITS
#include "config/support-float-edit.h"  // Support for editable float and double variables (option selects support them regardless of this setting)
#endif
