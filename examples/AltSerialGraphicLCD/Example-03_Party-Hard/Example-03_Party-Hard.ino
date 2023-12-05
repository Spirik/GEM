/*
  Party Hard menu example using GEM library. Explores creation of option select with callback (for preset selection),
  editable menu items with validation callbacks, setting readonly mode, creation of context with context.allowExit set to false
  in order to use push-buttons to control scene within context's loop routine.

  Simple one page menu with one editable menu item associated with int variable, one with bool variable, one option select,
  and a button, pressing of which will launch an animation sequence drawn to the screen. Delay between frames
  is determined by value of int variable, setting of which to 0 will enable manual control of the frames through
  navigation push-buttons.

  AltSerialGraphicLCD library is used to draw menu.
  KeyDetector library is used to detect push-buttons presses.
  
  Additional info (including the breadboard view) available on GitHub:
  https://github.com/Spirik/GEM
  
  This example code is in the public domain.
*/

#include <GEM.h>
#include <KeyDetector.h>

// Include sprites of animation frames
// (moved to separate file to save space)
#include "frames.h"

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
int interval = 200;
bool strobe = false;

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
void drawSprite(const uint8_t PROGMEM *_splash, byte _mode) {
  glcd.bitblt_P(glcd.xdim/2-(pgm_read_byte(_splash)+1)/2, glcd.ydim/2-(pgm_read_byte(_splash+1)+1)/2, _mode, _splash);
}

// Draw frame based on direction of animation
void drawFrame(bool forward) {
  if (forward) {
    // Next frame
    currentFrame = (currentFrame == framesCount ? 1 : currentFrame+1);
  } else {
    // Previous frame
    currentFrame = (currentFrame == 1 ? framesCount : currentFrame-1);
  }
  // Set mode based on strobe effect and frame number
  byte mode = strobe && (currentFrame % 2  == 0) ? GLCD_MODE_REVERSE : GLCD_MODE_NORMAL;
  // Draw frame on screen
  drawSprite(frames[currentFrame-1], mode);
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
  glcd.clearScreen();
  // Draw initial frame for the case of manual navigation ("Manual" tempo preset)
  if (interval == 0) {
    drawFrame(true);
  }
  Serial.println("Partying hard is in progress!");
}

// Invoked every loop iteration
void rockContextLoop() {
  // Detect key press manually using KeyDetector library
  myKeyDetector.detect();
  if (myKeyDetector.trigger == GEM_KEY_CANCEL) {
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
      switch (myKeyDetector.trigger) {
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
