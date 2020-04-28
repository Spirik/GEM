/*
  Blink menu example using GEM library. Explores creation of submenu, editable menu items
  with validation callbacks, button actions with custom context (with different values of context.allowExit flag).

  Two page menu with one editable menu item associated with int variable, one with char[17] variable,
  and two buttons, pressing of which will result in blinking of message on the screen: one button will launch
  delay() based routine and create context with context.allowExit set to true, and the second one - millis() based
  routine with context.allowExit set to false, that will require manual exit from the context's loop with
  explicit call to context.exit() routine. Callback function is attached to menu item linked to int variable,
  making sure that variable is within allowable range.

  U8g2lib library is used to draw menu and to detect push-buttons presses.
  
  Additional info (including the breadboard view) available on GitHub:
  https://github.com/Spirik/GEM
  
  This example code is in the public domain.
*/

#include <GEM_u8g2.h>

// Create an instance of the U8g2 library.
// Use constructor that matches your setup (see https://github.com/olikraus/u8g2/wiki/u8g2setupcpp for details).
// This instance is used to call all the subsequent U8g2 functions (internally from GEM library,
// or manually in your sketch if it is required).
// Please update the pin numbers according to your setup. Use U8X8_PIN_NONE if the reset pin is not connected
U8G2_KS0108_128X64_1 u8g2(U8G2_R0, 8, 9, 10, 11, 12, 13, 18, 19, /*enable=*/ A0, /*dc=*/ A1, /*cs0=*/ A3, /*cs1=*/ A2, /*cs2=*/ U8X8_PIN_NONE, /* reset=*/ U8X8_PIN_NONE);   // Set R/W to low!

// Create variables that will be editable through the menu and assign them initial values
int interval = 500;
char label[GEM_STR_LEN] = "Blink!"; // Maximum length of the string should not exceed 16 characters
                                    // (plus special terminating character)!

// Supplementary variable used in millis based version of Blink routine
unsigned long previousMillis = 0;

// Variable to hold current label state (visible or hidden)
boolean labelOn = false;

// Create two menu item objects of class GEMItem, linked to interval and label variables
// with validateInterval() callback function attached to interval menu item,
// that will make sure that interval variable is within allowable range (i.e. >= 50)
void validateInterval(); // Forward declaration
GEMItem menuItemInterval("Interval:", interval, validateInterval);
GEMItem menuItemLabel("Label:", label);

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

// Create menu object of class GEM_u8g2. Supply its constructor with reference to u8g2 object we created earlier
GEM_u8g2 menu(u8g2);

void setup() {
  // Serial communication setup
  Serial.begin(115200);

  // U8g2 library init. Pass pin numbers the buttons are connected to.
  // The push-buttons should be wired with pullup resistors (so the LOW means that the button is pressed)
  u8g2.begin(/*Select/OK=*/ 7, /*Right/Next=*/ 4, /*Left/Prev=*/ 3, /*Up=*/ 5, /*Down=*/ 2, /*Home/Cancel=*/ 6);

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
    // ...detect key press using U8g2 library
    // and pass pressed button to menu
    menu.registerKeyPress(u8g2.getMenuEvent());
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
  u8g2.clear();
  u8g2.firstPage();
  do {
    u8g2.setCursor(u8g2.getDisplayWidth()/2 - strlen(label)*3, u8g2.getDisplayHeight()/2 - 4);
    u8g2.print(label);
  } while (u8g2.nextPage());
}

// Toggle label on screen
void toggleLabel() {
  labelOn = !labelOn;
  if (labelOn) {
    printLabel();
  } else {
    u8g2.clear();
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
  // Detect key press manually using U8g2 library
  byte key = u8g2.getMenuEvent();
  if (key == GEM_KEY_CANCEL) {
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
