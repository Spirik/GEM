/*
  Basic menu example using GEM library. Using rotary encoder as an input source.

  Simple two page menu with one editable menu item associated with int variable, one with bool variable,
  and a button, pressing of which will result in int variable value printed to Serial monitor if bool variable is set to true.

  Second menu page with option select is added to better demonstrate operation of the menu with rotary encoder.

  AltSerialGraphicLCD library is used to draw menu.
  KeyDetector library (version 1.2.0 or later) is used to detect rotary encoder operation.
  
  Additional info (including the breadboard view) available on GitHub:
  https://github.com/Spirik/GEM
  
  This example code is in the public domain.
*/

#include <GEM.h>
#include <KeyDetector.h>

// Define signal identifiers for three outputs of encoder (channel A, channel B and a push-button)
#define KEY_A 1
#define KEY_B 2
#define KEY_C 3

// Pins encoder is connected to
const byte channelA = 2;
const byte channelB = 3;
const byte buttonPin = 4;

// Array of Key objects that will link GEM key identifiers with dedicated pins
// (it is only necessary to detect signal change on a single channel of the encoder, either A or B;
// order of the channel and push-button Key objects in an array is not important)
Key keys[] = {{KEY_A, channelA}, {KEY_C, buttonPin}};
//Key keys[] = {{KEY_C, buttonPin}, {KEY_A, channelA}};

// Create KeyDetector object
// KeyDetector myKeyDetector(keys, sizeof(keys)/sizeof(Key));
// To account for switch bounce effect of the buttons (if occur) you may want to specify debounceDelay
// as the third argument to KeyDetector constructor.
// Make sure to adjust debounce delay to better fit your rotary encoder.
// Also it is possible to enable pull-up mode when buttons wired with pull-up resistors (as in this case).
// Analog threshold is not necessary for this example and is set to default value 16.
KeyDetector myKeyDetector(keys, sizeof(keys)/sizeof(Key), /* debounceDelay= */ 5, /* analogThreshold= */ 16, /* pullup= */ true);

bool secondaryPressed = false;  // If encoder rotated while key was being pressed; used to prevent unwanted triggers
bool cancelPressed = false;  // Flag indicating that Cancel action was triggered, used to prevent it from triggering multiple times
const int keyPressDelay = 1000; // How long to hold key in pressed state to trigger Cancel action, ms
long keyPressTime = 0; // Variable to hold time of the key press event
long now; // Variable to hold current time taken with millis() function at the beginning of loop()

// Constants for the pins SparkFun Graphic LCD Serial Backpack is connected to and SoftwareSerial object
const byte rxPin = 8;
const byte txPin = 9;
SoftwareSerial serialLCD(rxPin, txPin);

// Create an instance of the GLCD class. This instance is used to call all the subsequent GLCD functions
// (internally from GEM library, or manually in your sketch if it is required)
GLCD glcd(serialLCD);

// Create variables that will be editable through the menu and assign them initial values
int number = -512;
bool enablePrint = false;

// Create variable that will be editable through option select and create associated option select.
// This variable will be passed to menu.invertKeysDuringEdit(), and naturally can be presented as a bool,
// but is declared as a byte type to be used in an option select rather than checkbox (for demonstration purposes)
byte invert = 1;
SelectOptionByte selectInvertOptions[] = {{"Invert", 1}, {"Normal", 0}};
GEMSelect selectInvert(sizeof(selectInvertOptions)/sizeof(SelectOptionByte), selectInvertOptions);

// Create menu item for option select with applyInvert() callback function
void applyInvert(); // Forward declaration
GEMItem menuItemInvert("Chars order:", invert, selectInvert, applyInvert);

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
GEMPage menuPageMain("Main Menu"); // Main page
GEMPage menuPageSettings("Settings"); // Settings submenu

// Create menu item linked to Settings menu page
GEMItem menuItemMainSettings("Settings", menuPageSettings);

// Create menu object of class GEM. Supply its constructor with reference to glcd object we created earlier
GEM menu(glcd);
// Which is equivalent to the following call (you can adjust parameters to better fit your screen if necessary):
// GEM menu(glcd, /* menuPointerType= */ GEM_POINTER_ROW, /* menuItemsPerScreen= */ 5, /* menuItemHeight= */ 10, /* menuPageScreenTopOffset= */ 10, /* menuValuesLeftOffset= */ 86);

void setup() {
  // Pin modes
  pinMode(channelA, INPUT_PULLUP);
  pinMode(channelB, INPUT_PULLUP);
  pinMode(buttonPin, INPUT_PULLUP);

  // Serial communication setup
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

  // Turn inverted order of characters during edit mode on (feels more natural when using encoder)
  menu.invertKeysDuringEdit(invert);
  
  // Menu init, setup and draw
  menu.init();
  setupMenu();
  menu.drawMenu();

  Serial.println("Initialized");
}

void setupMenu() {
  // Add menu items to Settings menu page
  menuPageSettings.addMenuItem(menuItemInvert);

  // Add menu items to menu page
  menuPageMain.addMenuItem(menuItemMainSettings);
  menuPageMain.addMenuItem(menuItemInt);
  menuPageMain.addMenuItem(menuItemBool);
  menuPageMain.addMenuItem(menuItemButton);

  // Specify parent menu page for the Settings menu page
  menuPageSettings.setParentMenuPage(menuPageMain);

  // Add menu page to menu and set it as current
  menu.setMenuPageCurrent(menuPageMain);
}

void loop() {
  // Get current time to use later on
  now = millis();

  // If menu is ready to accept button press...
  if (menu.readyForKey()) {
    // ...detect key press using KeyDetector library
    // and pass pressed button to menu
    myKeyDetector.detect();
  
    switch (myKeyDetector.trigger) {
      case KEY_C:
        // Button was pressed
        Serial.println("Button pressed");
        // Save current time as a time of the key press event
        keyPressTime = now;
        break;
    }
    /* Detecting rotation of the encoder on release rather than push
    (i.e. myKeyDetector.triggerRelease rather myKeyDetector.trigger)
    may lead to more stable readings (without excessive signal ripple) */
    switch (myKeyDetector.triggerRelease) {
      case KEY_A:
        // Signal from Channel A of encoder was detected
        if (digitalRead(channelB) == LOW) {
          // If channel B is low then the knob was rotated CCW
          if (myKeyDetector.current == KEY_C) {
            // If push-button was pressed at that time, then treat this action as GEM_KEY_LEFT,...
            Serial.println("Rotation CCW with button pressed (release)");
            menu.registerKeyPress(GEM_KEY_LEFT);
            // Button was in a pressed state during rotation of the knob, acting as a modifier to rotation action
            secondaryPressed = true;
          } else {
            // ...or GEM_KEY_UP otherwise
            Serial.println("Rotation CCW (release)");
            menu.registerKeyPress(GEM_KEY_UP);
          }
        } else {
          // If channel B is high then the knob was rotated CW
          if (myKeyDetector.current == KEY_C) {
            // If push-button was pressed at that time, then treat this action as GEM_KEY_RIGHT,...
            Serial.println("Rotation CW with button pressed (release)");
            menu.registerKeyPress(GEM_KEY_RIGHT);
            // Button was in a pressed state during rotation of the knob, acting as a modifier to rotation action
            secondaryPressed = true;
          } else {
            // ...or GEM_KEY_DOWN otherwise
            Serial.println("Rotation CW (release)");
            menu.registerKeyPress(GEM_KEY_DOWN);
          }
        }
        break;
      case KEY_C:
        // Button was released
        Serial.println("Button released");
        if (!secondaryPressed) {
          // If button was not used as a modifier to rotation action...
          if (now <= keyPressTime + keyPressDelay) {
            // ...and if not enough time passed since keyPressTime,
            // treat key that was pressed as Ok button
            menu.registerKeyPress(GEM_KEY_OK);
          }
        }
        secondaryPressed = false;
        cancelPressed = false;
        break;
    }
    // After keyPressDelay passed since keyPressTime
    if (now > keyPressTime + keyPressDelay) {
      switch (myKeyDetector.current) {
        case KEY_C:
          if (!secondaryPressed && !cancelPressed) {
            // If button was not used as a modifier to rotation action, and Cancel action was not triggered yet
            Serial.println("Button remained pressed");
            // Treat key that was pressed as Cancel button
            menu.registerKeyPress(GEM_KEY_CANCEL);
            cancelPressed = true;
          }
          break;
      }
    }
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

void applyInvert() {
  menu.invertKeysDuringEdit(invert);
  // Print invert variable to Serial
  Serial.print("Invert: ");
  Serial.println(invert);
}