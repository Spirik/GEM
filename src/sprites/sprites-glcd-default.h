/*
  This file contains collection of default sprites of the UI elements (icons) used in AltSerialGraphicLCD version of GEM library.

  It can be used as a template for specifying custom icons: copy it to the sketch, replace sprites
  and pass resulting sprites array to GEM constructor (or set as a property of GEMAppearance object).
  
  Additional info available on GitHub:
  https://github.com/Spirik/GEM
  
  This code is in the public domain.
*/

#define arrowRight_width  6
#define arrowRight_height 8
static const uint8_t arrowRight_bits [] PROGMEM = {
  arrowRight_width, arrowRight_height,
  0,0,62,28,8,0
};

#define arrowLeft_width  6
#define arrowLeft_height 8
static const uint8_t arrowLeft_bits [] PROGMEM = {
  arrowLeft_width, arrowLeft_height,
  8,28,62,0,0,0
};

#define arrowBtn_width  6
#define arrowBtn_height 8
static const uint8_t arrowBtn_bits [] PROGMEM = {
  arrowBtn_width, arrowBtn_height,
  62,34,20,8,0,0
};

#define checkboxUnchecked_width  7
#define checkboxUnchecked_height 8
static const uint8_t checkboxUnchecked_bits [] PROGMEM = {
  checkboxUnchecked_width, checkboxUnchecked_height,
  126,66,66,66,66,126,0
};

#define checkboxChecked_width  7
#define checkboxChecked_height 8
static const uint8_t checkboxChecked_bits [] PROGMEM = {
  checkboxChecked_width, checkboxChecked_height,
  126,74,82,74,68,126,1
};

#define selectArrows_width  6
#define selectArrows_height 8
static const uint8_t selectArrows_bits [] PROGMEM = {
  selectArrows_width, selectArrows_height,
  0,20,54,20,0,0
};

const GEMSprite arrowRight = {arrowRight_width, arrowRight_height, arrowRight_bits};

const GEMSprite arrowLeft = {arrowLeft_width, arrowLeft_height, arrowLeft_bits};

const GEMSprite arrowBtn = {arrowBtn_width, arrowBtn_height, arrowBtn_bits};

const GEMSprite checkboxUnchecked = {checkboxUnchecked_width, checkboxUnchecked_height, checkboxUnchecked_bits};

const GEMSprite checkboxChecked = {checkboxChecked_width, checkboxChecked_height, checkboxChecked_bits};

const GEMSprite selectArrows = {selectArrows_width, selectArrows_height, selectArrows_bits};

const GEMSprite* spritesDefault_glcd[] = {
  &arrowRight,         // GEM_ICON_ARROW_RIGHT
  &arrowLeft,          // GEM_ICON_ARROW_LEFT
  &arrowBtn,           // GEM_ICON_ARROW_BTN
  &checkboxUnchecked,  // GEM_ICON_CHECKBOX_UNCHECKED
  &checkboxChecked,    // GEM_ICON_CHECKBOX_CHECKED
  &selectArrows        // GEM_ICON_SELECT_ARROWS (AltSerialGraphicLCD version of GEM does not support separate GEM_ICON_SPINNER_ARROWS icon)
};
