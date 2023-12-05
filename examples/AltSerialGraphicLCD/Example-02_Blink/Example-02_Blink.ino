/*
  Blink menu example using GEM library. Explores creation of submenu, editable menu items
  with validation callbacks, button actions with custom context (with different values of context.allowExit flag).

  Two page menu with one editable menu item associated with int variable, one with char[17] variable,
  and two buttons, pressing of which will result in blinking with internal LED: one button will launch
  delay() based routine and create context with context.allowExit set to true, and the second one - millis() based
  routine with context.allowExit set to false, that will require manual exit from the context's loop with
  explicit call to context.exit() routine. Callback function is attached to menu item linked to int variable,
  making sure that variable is within allowable range.

  AltSerialGraphicLCD library is used to draw menu.
  KeyDetector library is used to detect push-buttons presses.
  
  Additional info (including the breadboard view) available on GitHub:
  https://github.com/Spirik/GEM
  
  This example code is in the public domain.
*/

#include <GEM.h>
#include <KeyDetector.h>

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
// KeyDetector myKeyDetector(keys, sizeof(keys)/sizeof(Key), /* debounceDelay= */ 10);

// Constants for the pins SparkFun Graphic LCD Serial Backpack is connected to and SoftwareSerial object
const byte rxPin = 8;
const byte txPin = 9;
SoftwareSerial serialLCD(rxPin, txPin);

// Create an instance of the GLCD class. This instance is used to call all the subsequent GLCD functions
// (internally from GEM library, or manually in your sketch if it is required)
GLCD glcd(serialLCD);

// Create variables that will be editable through the menu and assign them initial values
int interval = 500;
char label[GEM_STR_LEN] = "Blink!"; // Maximum length of the string should not exceed 16 characters
                                    // (plus special terminating character)!

// Supplementary variable used in millis based version of Blink routine
unsigned long previousMillis = 0;

// Variable to hold current LED state
bool ledOn = false;

// Create two menu item objects of class GEMItem, linked to interval and label variables
// with validateInterval() callback function attached to interval menu item,
// that will make sure that interval variable is within allowable range (i.e. >= 50)
void validateInterval(); // Forward declaration
GEMItem menuItemInterval("Interval:", interval, validateInterval);
GEMItem menuItemLabel("Label:", label);

// Create menu button that will trigger blinkDelay() function. It will blink with built-in LED with delay()
// set to the value of interval variable. We will write (define) this function later. However, we should
// forward-declare it in order to pass to GEMItem constructor
void blinkDelay(); // Forward declaration
GEMItem menuItemDelayButton1("Blink v1", blinkDelay);
// Likewise, create menu button that will trigger blinkMillis() function. It will blink with built-in LED with millis based
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

// Create menu object of class GEM. Supply its constructor with reference to glcd object we created earlier
GEM menu(glcd);
// Which is equivalent to the following call (you can adjust parameters to better fit your screen if necessary):
// GEM menu(glcd, /* menuPointerType= */ GEM_POINTER_ROW, /* menuItemsPerScreen= */ 5, /* menuItemHeight= */ 10, /* menuPageScreenTopOffset= */ 10, /* menuValuesLeftOffset= */ 86);

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
  serialLCD.begin(115200);

  // LCD reset
  delay(500);
  glcd.reset();
  delay(1000);
  // Uncomment the following lines in dire situations
  // (e.g. when screen becomes unresponsive after shutdown)
  glcd.reset();
  delay(1000);

  // Menu init, setup and draw
  menu.init();
  setupMenu();
  menu.drawMenu();
}

void setupMenu() {
  // Add menu items to Settings menu page
  menuPageSettings.addMenuItem(menuItemInterval);
  menuPageSettings.addMenuItem(menuItemLabel);

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

// --- Common Blink routines

// Clear screen and print label at the center
void printLabel() {
  glcd.clearScreen();
  glcd.setX(glcd.xdim/2 - strlen(label)*3);
  glcd.setY(glcd.ydim/2 - 4);
  glcd.putstr(label);
}

// Toggle built-in LED on or off
void toggleLED() {
  ledOn = !ledOn;
  digitalWrite(LED_BUILTIN, ledOn ? HIGH : LOW);
}

// Toggle label on screen
void toggleLabel() {
  if (ledOn) {
    printLabel();
  } else {
    glcd.clearScreen();
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
  // Set pin mode of the LED_BUILTIN pin (equals to pin 13 on Arduino UNO board)
  pinMode(LED_BUILTIN, OUTPUT);

  Serial.println("Delay based Blink is in progress");
}

// Invoked every loop iteration
void blinkDelayContextLoop() {
  // Blink with LED attached to the LED_BUILTIN pin.
  // Delay based Blink makes it harder to exit the loop
  // due to the blocking nature of the delay() function - carefully match timing of
  // exit key presses with the blink cycles; millis based blink has no such restriction
  toggleLED();
  toggleLabel();
  delay(interval);
}

// Invoked once when the GEM_KEY_CANCEL key is pressed
void blinkDelayContextExit() {
  // Reset variables
  ledOn = false;
  
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
  // Set pin mode of the LED_BUILTIN pin (equals to pin 13 on Arduino board)
  pinMode(LED_BUILTIN, OUTPUT);

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
    // Test millis timer and toggle LED accordingly.
    // Program flow is not paused and key press allows to exit Blink routine immediately
    unsigned long currentMillis = millis();
    if (currentMillis - previousMillis >= interval) {
      previousMillis = currentMillis;
      toggleLED();
      toggleLabel();
    }
  }
}

// Invoked once when the GEM_KEY_CANCEL key is pressed
void blinkMillisContextExit() {
  // Reset variables
  previousMillis = 0;
  ledOn = false;
  
  // Draw menu back on screen and clear context
  menu.reInit();
  menu.drawMenu();
  menu.clearContext();

  Serial.println("Exit millis based Blink");
}
