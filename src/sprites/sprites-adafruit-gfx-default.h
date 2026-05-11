/*
  This file contains collection of default sprites of the UI elements (icons) used in Adafruit GFX version of GEM library.

  It can be used as a template for specifying custom icons: copy it to the sketch, replace sprites
  and pass resulting sprites array to GEM_adafruit_gfx constructor (or set as a property of GEMAppearance object).
  
  Additional info available on GitHub:
  https://github.com/Spirik/GEM
  
  This code is in the public domain.
*/

// Default size

#define arrowRight_width  6
#define arrowRight_height 8
static const uint8_t arrowRight_bits [] PROGMEM = {
  0x00, 0x20, 0x30, 0x38, 0x30, 0x20, 0x00, 0x00
};

#define arrowLeft_width  6
#define arrowLeft_height 8
static const uint8_t arrowLeft_bits [] PROGMEM = {
  0x00, 0x20, 0x60, 0xe0, 0x60, 0x20, 0x00, 0x00
};

#define arrowBtn_width  6
#define arrowBtn_height 8
static const uint8_t arrowBtn_bits [] PROGMEM = {
  0x00, 0xc0, 0xa0, 0x90, 0xa0, 0xc0, 0x00, 0x00
};

#define checkboxUnchecked_width  7
#define checkboxUnchecked_height 8
static const uint8_t checkboxUnchecked_bits [] PROGMEM = {
  0x00, 0xfc, 0x84, 0x84, 0x84, 0x84, 0xfc, 0x00
};

#define checkboxChecked_width  7
#define checkboxChecked_height 8
static const uint8_t checkboxChecked_bits [] PROGMEM = {
  0x02, 0xf4, 0x8c, 0xd4, 0xa4, 0x84, 0xfc, 0x00
};

#define selectArrows_width  6
#define selectArrows_height 8
static const uint8_t selectArrows_bits [] PROGMEM = {
  0x00, 0x20, 0x70, 0x00, 0x70, 0x20, 0x00, 0x00
};

// Scaled 2x

#define arrowRight_width_scaled  12
#define arrowRight_height_scaled 16
static const uint8_t arrowRight_bits_scaled [] PROGMEM = {
  0x00, 0x00, 0x00, 0x00, 0x0c, 0x00, 0x0e, 0x00, 0x0f, 0x00, 0x0f, 0x80, 0x0f, 0xc0, 0x0f, 0xc0, 
  0x0f, 0x80, 0x0f, 0x00, 0x0e, 0x00, 0x0c, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
};

#define arrowLeft_width_scaled  12
#define arrowLeft_height_scaled 16
static const uint8_t arrowLeft_bits_scaled [] PROGMEM = {
  0x00, 0x00, 0x00, 0x00, 0x0c, 0x00, 0x1c, 0x00, 0x3c, 0x00, 0x7c, 0x00, 0xfc, 0x00, 0xfc, 0x00, 
  0x7c, 0x00, 0x3c, 0x00, 0x1c, 0x00, 0x0c, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
};

#define arrowBtn_width_scaled  12
#define arrowBtn_height_scaled 16
static const uint8_t arrowBtn_bits_scaled [] PROGMEM = {
  0x00, 0x00, 0x00, 0x00, 0xf0, 0x00, 0xf8, 0x00, 0xdc, 0x00, 0xce, 0x00, 0xc7, 0x00, 0xc7, 0x00, 
  0xce, 0x00, 0xdc, 0x00, 0xf8, 0x00, 0xf0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
};

#define checkboxUnchecked_width_scaled  14
#define checkboxUnchecked_height_scaled 16
static const uint8_t checkboxUnchecked_bits_scaled [] PROGMEM = {
  0x00, 0x00, 0x00, 0x00, 0xff, 0xf0, 0xff, 0xf0, 0xc0, 0x30, 0xc0, 0x30, 0xc0, 0x30, 0xc0, 0x30, 
  0xc0, 0x30, 0xc0, 0x30, 0xc0, 0x30, 0xc0, 0x30, 0xff, 0xf0, 0xff, 0xf0, 0x00, 0x00, 0x00, 0x00,
};

#define checkboxChecked_width_scaled  14
#define checkboxChecked_height_scaled 16
static const uint8_t checkboxChecked_bits_scaled [] PROGMEM = {
  0x00, 0x0c, 0x00, 0x1c, 0xff, 0x38, 0xfe, 0x70, 0xc0, 0xf0, 0xc1, 0xf0, 0xf3, 0xb0, 0xff, 0x30, 
  0xde, 0x30, 0xcc, 0x30, 0xc0, 0x30, 0xc0, 0x30, 0xff, 0xf0, 0xff, 0xf0, 0x00, 0x00, 0x00, 0x00,
};

#define selectArrows_width_scaled  12
#define selectArrows_height_scaled 16
static const uint8_t selectArrows_bits_scaled [] PROGMEM = {
  0x00, 0x00, 0x00, 0x00, 0x0c, 0x00, 0x1e, 0x00, 0x3f, 0x00, 0x3f, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x3f, 0x00, 0x3f, 0x00, 0x1e, 0x00, 0x0c, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

const GEMSprite arrowRight[] = {
  {arrowRight_width, arrowRight_height, arrowRight_bits},
  {arrowRight_width_scaled, arrowRight_height_scaled, arrowRight_bits_scaled}
};

const GEMSprite arrowLeft[] = {
  {arrowLeft_width, arrowLeft_height, arrowLeft_bits},
  {arrowLeft_width_scaled, arrowLeft_height_scaled, arrowLeft_bits_scaled}
};

const GEMSprite arrowBtn[] = {
  {arrowBtn_width, arrowBtn_height, arrowBtn_bits},
  {arrowBtn_width_scaled, arrowBtn_height_scaled, arrowBtn_bits_scaled}
};

const GEMSprite checkboxUnchecked[] = {
  {checkboxUnchecked_width, checkboxUnchecked_height, checkboxUnchecked_bits},
  {checkboxUnchecked_width_scaled, checkboxUnchecked_height_scaled, checkboxUnchecked_bits_scaled}
};

const GEMSprite checkboxChecked[] = {
  {checkboxChecked_width, checkboxChecked_height, checkboxChecked_bits},
  {checkboxChecked_width_scaled, checkboxChecked_height_scaled, checkboxChecked_bits_scaled}
};

const GEMSprite selectArrows[] = {
  {selectArrows_width, selectArrows_height, selectArrows_bits},
  {selectArrows_width_scaled, selectArrows_height_scaled, selectArrows_bits_scaled}
};

const GEMSprite* spritesDefault_adafruit_gfx[][2] = {
  { &arrowRight[0], &arrowRight[1] },               // GEM_ICON_ARROW_RIGHT
  { &arrowLeft[0], &arrowLeft[1] },                 // GEM_ICON_ARROW_LEFT,
  { &arrowBtn[0], &arrowBtn[1] },                   // GEM_ICON_ARROW_BTN,
  { &checkboxUnchecked[0], &checkboxUnchecked[1] }, // GEM_ICON_CHECKBOX_UNCHECKED,
  { &checkboxChecked[0], &checkboxChecked[1] },     // GEM_ICON_CHECKBOX_CHECKED,
  { &selectArrows[0], &selectArrows[1] },           // GEM_ICON_SELECT_ARROWS,
  { &selectArrows[0], &selectArrows[1] }            // GEM_ICON_SPINNER_ARROWS
};
