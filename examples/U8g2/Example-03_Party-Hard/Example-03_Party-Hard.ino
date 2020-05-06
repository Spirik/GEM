/*
  Party Hard menu example using GEM library. Explores creation of option select with callback (for preset selection),
  editable menu items with validation callbacks, setting readonly mode, creation of context with context.allowExit set to false
  in order to use push-buttons to control scene within context's loop routine.

  Simple one page menu with one editable menu item associated with int variable, one with boolean variable, one option select,
  and a button, pressing of which will launch an animation sequence drawn to the screen. Delay between frames
  is determined by value of int variable, setting of which to 0 will enable manual control of the frames through
  navigation push-buttons.

  U8g2lib library is used to draw menu and to detect push-buttons presses.
  
  Additional info (including the breadboard view) available on GitHub:
  https://github.com/Spirik/GEM
  
  This example code is in the public domain.
*/

#include <GEM_u8g2.h>

// Include sprites of animation frames
// (moved to separate file to save space)
#include "frames.h"

// Create an instance of the U8g2 library.
// Use constructor that matches your setup (see https://github.com/olikraus/u8g2/wiki/u8g2setupcpp for details).
// This instance is used to call all the subsequent U8g2 functions (internally from GEM library,
// or manually in your sketch if it is required).
// Please update the pin numbers according to your setup. Use U8X8_PIN_NONE if the reset pin is not connected
U8G2_KS0108_128X64_2 u8g2(U8G2_R0, 8, 9, 10, 11, 12, 13, 18, 19, /*enable=*/ A0, /*dc=*/ A1, /*cs0=*/ A3, /*cs1=*/ A2, /*cs2=*/ U8X8_PIN_NONE, /* reset=*/ U8X8_PIN_NONE);   // Set R/W to low!

// Create variables that will be editable through the menu and assign them initial values
int interval = 200;
boolean strobe = false;

// Create variable that will be editable through option select and create associated option select
byte tempo = 0;
SelectOptionByte selectTempoOptions[] = {{"Meh:(", 0}, {"Smooth", 1}, {"Hard", 2}, {"XTREME", 3}, {"Manual", 4}, {"Custom", 5}};
GEMSelect selectTempo(sizeof(selectTempoOptions)/sizeof(SelectOptionByte), selectTempoOptions);

// Values of interval variable associated with each select option
int tempoInterval[] = {400, 250, 120, 100, 0, 200};

// Supplementary variables used for animation control
unsigned long previousMillis = 0;
byte framesCount = 5;
byte currentFrame = framesCount;

// Create menu item objects of class GEMItem, linked to interval and strobe variables
// with validateInterval() callback function attached to interval menu item,
// that will make sure that interval variable is within allowable range (i.e. >= 0)
void validateInterval(); // Forward declaration
GEMItem menuItemInt("Interval:", interval, validateInterval);
GEMItem menuItemStrobe("Strobe:", strobe);

// Create menu item for option select with applyTempo() callback function
void applyTempo(); // Forward declaration
GEMItem menuItemTempo("Tempo:", tempo, selectTempo, applyTempo);

// Create menu button that will trigger rock() function. It will run animation sequence.
// We will write (define) this function later. However, we should
// forward-declare it in order to pass to GEMItem constructor
void rock(); // Forward declaration
GEMItem menuItemButton("Let's Rock!", rock);

// Create menu page object of class GEMPage. Menu page holds menu items (GEMItem) and represents menu level.
// Menu can have multiple menu pages (linked to each other) with multiple menu items each
GEMPage menuPageMain("Party Hard");

// Create menu object of class GEM_u8g2. Supply its constructor with reference to u8g2 object we created earlier
GEM_u8g2 menu(u8g2);

void setup() {
  // Serial communication setup
  Serial.begin(115200);

  // U8g2 library init. Pass pin numbers the buttons are connected to.
  // The push-buttons should be wired with pullup resistors (so the LOW means that the button is pressed)
  u8g2.begin(/*Select/OK=*/ 7, /*Right/Next=*/ 4, /*Left/Prev=*/ 3, /*Up=*/ 5, /*Down=*/ 2, /*Home/Cancel=*/ 6);

  // Load initial preset selected through tempo option select
  applyTempo();

  // Menu init, setup and draw
  menu.init();
  setupMenu();
  menu.drawMenu();
}

void setupMenu() {
  // Add menu items to menu page
  menuPageMain.addMenuItem(menuItemTempo);
  menuPageMain.addMenuItem(menuItemInt);
  menuPageMain.addMenuItem(menuItemStrobe);
  menuPageMain.addMenuItem(menuItemButton);

  // Add menu page to menu and set it as current
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
  // Check if interval variable is within allowable range (i.e. >= 0)
  if (interval < 0) {
    interval = 0;
  }
  // Print interval variable to Serial
  Serial.print("Interval set: ");
  Serial.println(interval);
}

// Apply preset based on tempo variable value
void applyTempo() {
  if (tempo != 5) {
    // Set readonly mode for interval menu item
    menuItemInt.setReadonly();
    // Apply interval value based on preset selection
    interval = tempoInterval[tempo];
    // Turn on strobe effect for "XTREME" preset
    strobe = tempo == 3;
  } else {
    // Disable readonly mode of interval menu item for "Custom" preset
    menuItemInt.setReadonly(false);
  }
  // Print tempo variable to Serial
  Serial.print("Tempo option set: ");
  Serial.println(tempo);
}

// --- Animation draw routines

// Draw sprite on screen
// (note that Splash is the custom type used internally in GEM library for convenient way of storing XBM graphics of Splash screen)
void drawSprite(Splash _splash) {
  u8g2.drawXBMP((u8g2.getDisplayWidth() - _splash.width) / 2, (u8g2.getDisplayHeight() - _splash.height) / 2, _splash.width, _splash.height, _splash.image);
}

// Draw frame based on direction of animation
void drawFrame(boolean forward) {
  if (forward) {
    // Next frame
    currentFrame = (currentFrame == framesCount ? 1 : currentFrame+1);
  } else {
    // Previous frame
    currentFrame = (currentFrame == 1 ? framesCount : currentFrame-1);
  }
  // Set inversed mode based on strobe effect and frame number
  byte mode = strobe && (currentFrame % 2  == 0) ? 0 : 1;
  // Draw on screen
  u8g2.firstPage();
  do {
    u8g2.setDrawColor(mode);
    // Draw frame on screen
    drawSprite(frames[currentFrame-1]);
  } while (u8g2.nextPage());
}

// --- Animation context routines

// Setup context
void rock() {
  menu.context.loop = rockContextLoop;
  menu.context.enter = rockContextEnter;
  menu.context.exit = rockContextExit;
  menu.context.allowExit = false; // Setting to false will require manual exit from the loop
  menu.context.enter();
}

// Invoked once when the button is pressed
void rockContextEnter() {
  // Clear sreen
  u8g2.clear();
  // Draw initial frame for the case of manual navigation ("Manual" tempo preset)
  if (interval == 0) {
    drawFrame(true);
  }
  Serial.println("Partying hard is in progress!");
}

// Invoked every loop iteration
void rockContextLoop() {
  // Detect key press manually using U8g2 library
  byte key = u8g2.getMenuEvent();
  if (key == GEM_KEY_CANCEL) {
    // Exit animation routine if GEM_KEY_CANCEL key was pressed
    menu.context.exit();
  } else {
    if (interval > 0) {
      // Autoplay mode.
      // Test millis timer and draw frame accordingly
      unsigned long currentMillis = millis();
      if (currentMillis - previousMillis >= interval) {
        previousMillis = currentMillis;
        drawFrame(true);
      }
    } else {
      // Manual mode.
      // Check pressed keys and navigate through frames accordingly
      switch (key) {
        case GEM_KEY_RIGHT:
          Serial.println("Next frame");
          drawFrame(true);
          break;
        case GEM_KEY_LEFT:
          Serial.println("Previous frame");
          drawFrame(false);
          break;
      }
    }
  }
}

// Invoked once when the GEM_KEY_CANCEL key is pressed
void rockContextExit() {
  // Reset variables
  previousMillis = 0;
  currentFrame = framesCount;

  // Draw menu back on screen and clear context
  menu.reInit();
  menu.drawMenu();
  menu.clearContext();
}
