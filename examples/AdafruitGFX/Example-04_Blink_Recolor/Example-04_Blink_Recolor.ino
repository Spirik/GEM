/*
  Blink menu example using GEM library. Explores creation of submenu, editable menu items
  with validation callbacks, button actions with custom context (with different values of context.allowExit flag).
  This Adafruit GFX modification also features creation of option select with callback (for changing color scheme).

  Two page menu with one editable menu item associated with int variable, one with char[17] variable,
  and two buttons, pressing of which will result in blinking of message on the screen: one button will launch
  delay() based routine and create context with context.allowExit set to true, and the second one - millis() based
  routine with context.allowExit set to false, that will require manual exit from the context's loop with
  explicit call to context.exit() routine. Callback function is attached to menu item linked to int variable,
  making sure that variable is within allowable range. Additional option select (specific for Adafruit GFX version
  of this example) will allow to select one of 5 predefined color schemes.

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
int interval = 500;
char label[GEM_STR_LEN] = "Blink!"; // Maximum length of the string should not exceed 16 characters
                                    // (plus special terminating character)!

// Supplementary variable used in millis based version of Blink routine
unsigned long previousMillis = 0;

// Variable to hold current label state (visible or hidden)
boolean labelOn = false;

// Variables to hold currently set foreground and background colors
int foreColor = ST77XX_WHITE;
int backColor = ST77XX_BLACK;

// Create variable that will be editable through option select and create associated option select
byte colorScheme = 0;
SelectOptionByte selectColorSchemeOptions[] = {{"Default", 0}, {"Inverted", 1}, {"Matrix", 2}, {"Amber", 3}, {"BSOD", 4}};
GEMSelect selectColorScheme(sizeof(selectColorSchemeOptions)/sizeof(SelectOptionByte), selectColorSchemeOptions);

// Create two menu item objects of class GEMItem, linked to interval and label variables
// with validateInterval() callback function attached to interval menu item,
// that will make sure that interval variable is within allowable range (i.e. >= 50)
void validateInterval(); // Forward declaration
GEMItem menuItemInterval("Interval:", interval, validateInterval);
GEMItem menuItemLabel("Label:", label);

// Create menu item for option select with applyScheme() callback function
void applyScheme(); // Forward declaration
GEMItem menuItemColorScheme("Color Scheme:", colorScheme, selectColorScheme, applyScheme);

// Create menu button that will trigger blinkDelay() function. It will blink the label on the screen with delay()
// set to the value of interval variable. We will write (define) this function later. However, we should
// forward-declare it in order to pass to GEMItem constructor
void blinkDelay(); // Forward declaration
GEMItem menuItemDelayButton1("Blink v1", blinkDelay);
// Likewise, create menu button that will trigger blinkMillis() function. It will blink the label on the screen with millis based
// delay set to the value of interval variable. We will write (define) this function later. However, we should
// forward-declare it in order to pass to GEMItem constructor
void blinkMillis(); // Forward declaration
GEMItem menuItemDelayButton2("Blink v2", blinkMillis);

// Create menu page object of class GEMPage. Menu page holds menu items (GEMItem) and represents menu level.
// Menu can have multiple menu pages (linked to each other) with multiple menu items each
GEMPage menuPageMain("Main Menu"); // Main page
GEMPage menuPageSettings("Settings"); // Settings submenu

// Create menu item linked to Settings menu page
GEMItem menuItemMainSettings("Settings", menuPageSettings);

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
  // Add menu items to Settings menu page
  menuPageSettings.addMenuItem(menuItemInterval);
  menuPageSettings.addMenuItem(menuItemLabel);
  menuPageSettings.addMenuItem(menuItemColorScheme);

  // Add menu items to Main Menu page
  menuPageMain.addMenuItem(menuItemMainSettings);
  menuPageMain.addMenuItem(menuItemDelayButton1);
  menuPageMain.addMenuItem(menuItemDelayButton2);

  // Specify parent menu page for the Settings menu page
  menuPageSettings.setParentMenuPage(menuPageMain);
  
  // Add Main Menu page to menu and set it as current
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

// ---

// Validation routine of interval variable
void validateInterval() {
  // Check if interval variable is within allowable range (i.e. >= 50)
  if (interval < 50) {
    interval = 50;
  }
  // Print interval variable to Serial
  Serial.print("Interval set: ");
  Serial.println(interval);
}

// Apply preset based on colorScheme variable value
void applyScheme() {
  // Print color scheme name to Serial and set menu foreground and background colors.
  // Note that actual color appearance may differ based on screen initialization parameters passed to tft.initR()
  Serial.print("Color scheme set: ");
  switch (colorScheme) {
    case 0:
      foreColor = ST77XX_WHITE;
      backColor = ST77XX_BLACK;
      Serial.println("Default");
      break;
    case 1:
      foreColor = ST77XX_BLACK;
      backColor = ST77XX_WHITE;
      Serial.println("Inverted");
      break;
    case 2:
      foreColor = ST77XX_GREEN;
      backColor = ST77XX_BLACK;
      Serial.println("Matrix");
      break;
    case 3:
      foreColor = ST77XX_ORANGE;
      backColor = ST77XX_BLACK;
      Serial.println("Amber");
      break;
    case 4:
      foreColor = ST77XX_WHITE;
      backColor = ST77XX_BLUE;
      Serial.println("BSOD");
      break;
  }
  menu.setForegroundColor(foreColor);
  menu.setBackgroundColor(backColor);
}

// --- Common Blink routines

// Clear screen and print label at the center
void printLabel() {
  tft.setTextColor(foreColor);
  tft.fillScreen(backColor);
  byte x = tft.width() / 2 - strlen(label) * 3;
  byte y = tft.height() / 2 - 4;
  tft.setCursor(x, y);
  tft.print(label);
}

// Toggle label on screen
void toggleLabel() {
  labelOn = !labelOn;
  if (labelOn) {
    printLabel();
  } else {
    tft.fillScreen(backColor);
  }
}

// --- Delay based Blink context routines

// Setup context for the delay based Blink routine
void blinkDelay() {
  menu.context.loop = blinkDelayContextLoop;
  menu.context.enter = blinkDelayContextEnter;
  menu.context.exit = blinkDelayContextExit;
  menu.context.enter();
}

// Invoked once when the button is pressed
void blinkDelayContextEnter() {
  Serial.println("Delay based Blink is in progress");
}

// Invoked every loop iteration
void blinkDelayContextLoop() {
  // Blink the label on the screen.
  // Delay based Blink makes it harder to exit the loop
  // due to the blocking nature of the delay() function - carefully match timing of
  // exit key presses with the blink cycles; millis based blink has no such restriction
  toggleLabel();
  delay(interval);
}

// Invoked once when the GEM_KEY_CANCEL key is pressed
void blinkDelayContextExit() {
  // Reset variables
  labelOn = false;
  
  // Draw menu back on screen and clear context
  menu.reInit();
  menu.drawMenu();
  menu.clearContext();

  Serial.println("Exit delay based Blink");
}

// --- Millis based Blink context routines

// Setup context for the millis based Blink routine
void blinkMillis() {
  menu.context.loop = blinkMillisContextLoop;
  menu.context.enter = blinkMillisContextEnter;
  menu.context.exit = blinkMillisContextExit;
  menu.context.allowExit = false; // Setting to false will require manual exit from the loop
  menu.context.enter();
}

// Invoked once when the button is pressed
void blinkMillisContextEnter() {
  Serial.println("Millis based Blink is in progress");
}

// Invoked every loop iteration
void blinkMillisContextLoop() {
  // Detect key press manually using KeyDetector library
  myKeyDetector.detect();
  if (myKeyDetector.trigger == GEM_KEY_CANCEL) {
    // Exit Blink routine if GEM_KEY_CANCEL key was pressed
    menu.context.exit();
  } else {
    // Test millis timer and toggle label accordingly.
    // Program flow is not paused and key press allows to exit Blink routine immediately
    unsigned long currentMillis = millis();
    if (currentMillis - previousMillis >= interval) {
      previousMillis = currentMillis;
      toggleLabel();
    }
  }
}

// Invoked once when the GEM_KEY_CANCEL key is pressed
void blinkMillisContextExit() {
  // Reset variables
  previousMillis = 0;
  labelOn = false;
  
  // Draw menu back on screen and clear context
  menu.reInit();
  menu.drawMenu();
  menu.clearContext();

  Serial.println("Exit millis based Blink");
}
