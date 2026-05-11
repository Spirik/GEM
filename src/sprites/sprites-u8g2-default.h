/*
  This file contains collection of default sprites of the UI elements (icons) used in U8g2 version of GEM library.

  It can be used as a template for specifying custom icons: copy it to the sketch, replace sprites
  and pass resulting sprites array to GEM_u8g2 constructor (or set as a property of GEMAppearance object).
  
  Additional info available on GitHub:
  https://github.com/Spirik/GEM
  
  This code is in the public domain.
*/

#define arrowRight_width  6
#define arrowRight_height 8
static const unsigned char arrowRight_bits [] U8X8_PROGMEM = {
  0xc0,0xc4,0xcc,0xdc,0xcc,0xc4,0xc0,0xc0
};

#define arrowLeft_width  6
#define arrowLeft_height 8
static const unsigned char arrowLeft_bits [] U8X8_PROGMEM = {
  0xc0,0xc4,0xc6,0xc7,0xc6,0xc4,0xc0,0xc0
};

#define arrowBtn_width  6
#define arrowBtn_height 8
static const unsigned char arrowBtn_bits [] U8X8_PROGMEM = {
  0xc0,0xc3,0xc5,0xc9,0xc5,0xc3,0xc0,0xc0
};

#define checkboxUnchecked_width  7
#define checkboxUnchecked_height 8
static const unsigned char checkboxUnchecked_bits [] U8X8_PROGMEM = {
  0x80,0xbf,0xa1,0xa1,0xa1,0xa1,0xbf,0x80
};

#define checkboxChecked_width  7
#define checkboxChecked_height 8
static const unsigned char checkboxChecked_bits [] U8X8_PROGMEM = {
  0xc0,0xaf,0xb1,0xab,0xa5,0xa1,0xbf,0x80
};

#define selectArrows_width  6
#define selectArrows_height 8
static const unsigned char selectArrows_bits [] U8X8_PROGMEM = {
  0xc0,0xc4,0xce,0xc0,0xce,0xc4,0xc0,0xc0
};

const GEMSprite arrowRight = {arrowRight_width, arrowRight_height, arrowRight_bits};

const GEMSprite arrowLeft = {arrowLeft_width, arrowLeft_height, arrowLeft_bits};

const GEMSprite arrowBtn = {arrowBtn_width, arrowBtn_height, arrowBtn_bits};

const GEMSprite checkboxUnchecked = {checkboxUnchecked_width, checkboxUnchecked_height, checkboxUnchecked_bits};

const GEMSprite checkboxChecked = {checkboxChecked_width, checkboxChecked_height, checkboxChecked_bits};

const GEMSprite selectArrows = {selectArrows_width, selectArrows_height, selectArrows_bits};

const GEMSprite* spritesDefault_u8g2[] = {
  &arrowRight,         // GEM_ICON_ARROW_RIGHT
  &arrowLeft,          // GEM_ICON_ARROW_LEFT
  &arrowBtn,           // GEM_ICON_ARROW_BTN
  &checkboxUnchecked,  // GEM_ICON_CHECKBOX_UNCHECKED
  &checkboxChecked,    // GEM_ICON_CHECKBOX_CHECKED
  &selectArrows,       // GEM_ICON_SELECT_ARROWS
  &selectArrows        // GEM_ICON_SPINNER_ARROWS
};
