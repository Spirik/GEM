// AltSerialGraphicLCD support disabled by default (since GEM ver. 1.5).
// Can be enabled either by defining GEM_ENABLE_GLCD (via compiler flag or define) or manual edition here.
#define GEM_DISABLE_GLCD                    // Comment this line to enable AltSerialGraphicLCD support
#if !defined(GEM_DISABLE_GLCD) || defined(GEM_ENABLE_GLCD)
#include "config/enable-glcd.h"             // Enable AltSerialGraphicLCD version of GEM
#endif

// U8g2 support enabled by default.
// Can be disabled either by defining GEM_DISABLE_U8G2 (via compiler flag or define) or manual edition here.
#if !defined(GEM_DISABLE_U8G2)
#include "config/enable-u8g2.h"             // Enable U8g2 version of GEM
#endif

// Adafruit GFX support enabled by default.
// Can be disabled either by defining GEM_DISABLE_ADAFRUIT_GFX (via compiler flag or define) or manual edition here.
#if !defined(GEM_DISABLE_ADAFRUIT_GFX)
#include "config/enable-adafruit-gfx.h"     // Enable Adafruit GFX version of GEM
#endif

// Support for editable float variables enabled by default.
// Can be disabled either by defining GEM_DISABLE_FLOAT_EDIT (via compiler flag or define) or manual edition here.
#if !defined(GEM_DISABLE_FLOAT_EDIT)
#include "config/support-float-edit.h"      // Support for editable float and double variables (option selects support them regardless of this setting)
#endif

// Support for spinner (GEMSpinner) menu items enabled by default.
// Can be disabled either by defining GEM_DISABLE_SPINNER (via compiler flag or define) or manual edition here.
#if !defined(GEM_DISABLE_SPINNER)
#include "config/support-spinner.h"         // Support for increment/decrement spinner menu items
#endif

// Support for Advanced Mode is disabled by default.
// Advanced Mode provides additional means to modify, customize and extend functionality of GEM.
// Can be enabled either by defining GEM_ENABLE_ADVANCED_MODE (via compiler flag or define) or manual edition here.
#define GEM_DISABLE_ADVANCED_MODE           // Comment this line to enable Advanced Mode
#if !defined(GEM_DISABLE_ADVANCED_MODE) || defined(GEM_ENABLE_ADVANCED_MODE)
#include "config/enable-advanced-mode.h"    // Enable Advanced Mode
#endif
