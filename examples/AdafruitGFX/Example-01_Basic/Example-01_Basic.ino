/*
  Basic menu example using GEM library.

  Simple one page menu with one editable menu item associated with int variable, one with boolean variable,
  and a button, pressing of which will result in int variable value printed to Serial monitor if boolean variable is set to true.

  Adafruit GFX library is used to draw menu.
  KeyDetector library is used to detect push-buttons presses.
  
  Additional info (including the breadboard view) available on GitHub:
  https://github.com/Spirik/GEM
  
  This example code is in the public domain.
*/

#include <GEM_adafruit_gfx.h>
#include <KeyDetector.h>

// Hardware-specific library for ST7735.
// Include library that matches your setup (see https://learn.adafruit.com/adafruit-gfx-graphics-library for details)
#include <Adafruit_ST7735.h>

// Pins the buttons are connected to
const byte downPin = 2;
const byte leftPin = 3;
const byte rightPin = 4;
const byte upPin = 5;
const byte cancelPin = 6;
const byte okPin = 7;

// Array of Key objects that will link GEM key identifiers with dedicated pins
Key keys[] = {{GEM_KEY_UP, upPin}, {GEM_KEY_RIGHT, rightPin}, {GEM_KEY_DOWN, downPin}, {GEM_KEY_LEFT, leftPin}, {GEM_KEY_CANCEL, cancelPin}, {GEM_KEY_OK, okPin}};

// Create KeyDetector object
KeyDetector myKeyDetector(keys, sizeof(keys)/sizeof(Key));
// To account for switch bounce effect of the buttons (if occur) you may want to specify debounceDelay
// as the third argument to KeyDetector constructor:
// KeyDetector myKeyDetector(keys, sizeof(keys)/sizeof(Key), 10);

// Macro constants (aliases) for the pins TFT display is connected to. Please update the pin numbers according to your setup
#define TFT_CS    A2
#define TFT_RST   -1 // Set to -1 and connect to Arduino RESET pin
#define TFT_DC    A3

// Create an instance of the Adafruit GFX library.
// Use constructor that matches your setup (see https://learn.adafruit.com/adafruit-gfx-graphics-library for details).
// ST7735 based display is used in the example.
// This instance is used to call all the subsequent Adafruit GFX functions (internally from GEM library,
// or manually in your sketch if it is required)
Adafruit_ST7735 tft = Adafruit_ST7735(TFT_CS, TFT_DC, TFT_RST);

// Create variables that will be editable through the menu and assign them initial values
int number = -512;
boolean enablePrint = false;

// Create two menu item objects of class GEMItem, linked to number and enablePrint variables 
GEMItem menuItemInt("Number:", number);
GEMItem menuItemBool("Enable print:", enablePrint);

// Create menu button that will trigger printData() function. It will print value of our number variable
// to Serial monitor if enablePrint is true. We will write (define) this function later. However, we should
// forward-declare it in order to pass to GEMItem constructor
void printData(); // Forward declaration
GEMItem menuItemButton("Print", printData);

// Create menu page object of class GEMPage. Menu page holds menu items (GEMItem) and represents menu level.
// Menu can have multiple menu pages (linked to each other) with multiple menu items each
GEMPage menuPageMain("Main Menu");

// Create menu object of class GEM_adafruit_gfx. Supply its constructor with reference to tft object we created earlier
GEM_adafruit_gfx menu(tft, GEM_POINTER_ROW, GEM_ITEMS_COUNT_AUTO);

void setup() {
  // Push-buttons pin modes
  pinMode(downPin, INPUT);
  pinMode(leftPin, INPUT);
  pinMode(rightPin, INPUT);
  pinMode(upPin, INPUT);
  pinMode(cancelPin, INPUT);
  pinMode(okPin, INPUT);

  // Serial communications setup
  Serial.begin(115200);

  // Use this initializer if using a 1.8" TFT screen:
  tft.initR(INITR_BLACKTAB);      // Init ST7735S chip, black tab
  // OR use this initializer if using a 1.8" TFT screen with offset such as WaveShare:
  // tft.initR(INITR_GREENTAB);   // Init ST7735S chip, green tab
  // See more options in Adafruit GFX library documentation

  // Optionally, rotate display
  // tft.setRotation(3); // See Adafruit GFX library documentation for details

  // Menu init, setup and draw
  menu.init();
  setupMenu();
  menu.drawMenu();
}

void setupMenu() {
  // Add menu items to menu page
  menuPageMain.addMenuItem(menuItemInt);
  menuPageMain.addMenuItem(menuItemBool);
  menuPageMain.addMenuItem(menuItemButton);

  // Add menu page to menu and set it as current
  menu.setMenuPageCurrent(menuPageMain);
}

void loop() {
  // If menu is ready to accept button press...
  if (menu.readyForKey()) {
    // ...detect key press using KeyDetector library
    myKeyDetector.detect();
    // Pass pressed button to menu
    // (pressed button ID is stored in trigger property of KeyDetector object)
    menu.registerKeyPress(myKeyDetector.trigger);
  }
}

void printData() {
  // If enablePrint flag is set to true (checkbox on screen is checked)...
  if (enablePrint) {
    // ...print the number to Serial
    Serial.print("Number is: ");
    Serial.println(number);
  } else {
    Serial.println("Printing is disabled, sorry:(");
  }
}
