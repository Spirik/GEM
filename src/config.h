// AltSerialGraphicLCD support enabled by default.
// Can be disabled either by defining GEM_DISABLE_GLCD (via compiler flag or define) or manual edition here.
#ifndef GEM_DISABLE_GLCD
#include "config/enable-glcd.h"         // Enable AltSerialGraphicLCD version of GEM
#endif

// U8g2 support enabled by default.
// Can be disabled either by defining GEM_DISABLE_U8G2 (via compiler flag or define) or manual edition here.
#ifndef GEM_DISABLE_U8G2
#include "config/enable-u8g2.h"         // Enable U8g2 version of GEM
#endif

// Adafruit GFX support enabled by default.
// Can be disabled either by defining GEM_DISABLE_ADAFRUIT_GFX (via compiler flag or define) or manual edition here.
#ifndef GEM_DISABLE_ADAFRUIT_GFX
#include "config/enable-adafruit-gfx.h" // Enable Adafruit GFX version of GEM
#endif

// Support for editable float variables enabled by default.
// Can be disabled either by defining GEM_DISABLE_FLOAT_EDIT (via compiler flag or define) or manual edition here.
#ifndef GEM_DISABLE_FLOAT_EDIT
#include "config/support-float-edit.h"  // Support for editable float and double variables (option selects support them regardless of this setting)
#endif
