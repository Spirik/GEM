/*
  Using GEM to create Todo list utilizing (god-forbidden) dynamic memory allocation (`new` and `delete`).
  Using rotary encoder as an input source. Todo items can be dynamically added to the list,
  marked completed and cleared (removed from the list). Additional settings are provided
  (e.g. changing menu pointer style and order of characters in edit mode).

  Note, that generally it is not recommended to implement dynamic memory allocation in microcontroller-based
  projects for a number of reasons (mostly due to memory limitations and lack of supervisory OS to handle
  memory management). Consider this example as an experiment and merely demonstration of some of the GEM
  features, rather than a guide on how to manage dynamic memory in your project.

  AltSerialGraphicLCD library is used to draw menu.
  KeyDetector library (version 1.2.0 or later) is used to detect rotary encoder operation.

  Points of improvement to consider:
  - Prevent adding new Todo items if insufficient amount of RAM is available
  - Add button to Uncheck all and/or Check all Todo items
  - Use external storage (e.g. SD Card) to store data
  - Make portable by adding battery
  
  This example uses the same schematics/breadboard as Example-05_Encoder (supplied with GEM).

  Additional info (including the breadboard view) available on GitHub:
  https://github.com/Spirik/GEM
  
  This example code is in the public domain.
*/

#include <GEM.h>
#include <KeyDetector.h>

//====================== CLASSES

// Class representing Todo item
class TodoItem {
  public:
    /* 
      @param 'title_' - title of Todo item
    */
    TodoItem(char* title_){
      strcpy(title, title_);
      menuItem = new GEMItem(title, completed);
    };

    char title[GEM_STR_LEN];      // Title of Todo item
    bool completed = false;       // Checkbox status
    GEMItem* menuItem = nullptr;  // Pointer to corresponding menu item
};

//====================== MISC

// Custom splash
static const uint8_t splashBits [] PROGMEM = {
  27, 8,
  0x01, 0x01, 0x7d, 0x01, 0x01, 0x00, 0x00, 0x3e, 0x41, 0x41, 0x41, 0x3e, 0x00, 0x00, 0x7d, 0x41, 
	0x41, 0x41, 0x3e, 0x00, 0x00, 0x3e, 0x41, 0x41, 0x41, 0x3e, 0x00
};

//====================== WORKING WITH ENCODER

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

// Create KeyDetector object
// KeyDetector myKeyDetector(keys, sizeof(keys)/sizeof(Key));
// To account for switch bounce effect of the buttons (if occur) you may want to specify debounceDelay
// as the third argument to KeyDetector constructor.
// Make sure to adjust debounce delay to better fit your rotary encoder.
// Also it is possible to enable pull-up mode when buttons wired with pull-up resistors (as in this case).
// Analog threshold is not necessary for this example and is set to default value 16.
KeyDetector myKeyDetector(keys, sizeof(keys)/sizeof(Key), /* debounceDelay= */ 10, /* analogThreshold= */ 16, /* pullup= */ true);

bool secondaryPressed = false;  // If encoder rotated while key was being pressed; used to prevent unwanted triggers
bool cancelPressed = false;  // Flag indicating that Cancel action was triggered, used to prevent it from triggering multiple times
const int keyPressDelay = 1000; // How long to hold key in pressed state to trigger Cancel action, ms
long keyPressTime = 0; // Variable to hold time of the key press event
long now; // Variable to hold current time taken with millis() function at the beginning of loop()

//====================== OBTAINING RAM STATUS

// Variable to store free RAM. It is an int, so overflows and rollover may occur, in that case, free RAM won't be displayed
int freeRam;
GEMItem menuItemRam("Free RAM:", freeRam, GEM_READONLY); // Menu item associated with it

// Free RAM calculations
// (based on https://docs.arduino.cc/learn/programming/memory-guide and https://github.com/mpflaga/Arduino-MemoryFree/)
#if defined(__arm__) && !defined(ARDUINO_ARCH_RP2040)
// ARM (except RP2040, which won't display correct values, probably due to internal implementation)

extern "C" char* sbrk(int incr);

void calculateFreeRam() {
  freeRam = getFreeRam();
}

int getFreeRam() {
  char top;
  return &top - reinterpret_cast<char*>(sbrk(0));
}

#elif defined(ARDUINO_ARCH_AVR)
// ARM

void calculateFreeRam() {
  freeRam = getFreeRam();
}

int getFreeRam() {
  extern int __heap_start,*__brkval;
  int v;
  return (int)&v - (__brkval == 0 ? (int)&__heap_start : (int) __brkval);
}

#elif defined(ARDUINO_ARCH_ESP32)
// ESP32

void calculateFreeRam() {
  freeRam = ESP.getFreeHeap();
}

#else
// Correct detection of free RAM not implemented

void calculateFreeRam() {
  freeRam = -1;
}

#endif

//====================== DISPLAY

// Constants for the pins SparkFun Graphic LCD Serial Backpack is connected to and SoftwareSerial object
const byte rxPin = 8;
const byte txPin = 9;
SoftwareSerial serialLCD(rxPin, txPin);

// Create an instance of the GLCD class. This instance is used to call all the subsequent GLCD functions
// (internally from GEM library, or manually in your sketch if it is required)
GLCD glcd(serialLCD);

//====================== MENU AND CORRESPONDING ELEMENTS

// Create variable that will be editable through option select and create associated option select.
// This variable will be passed to menu.invertKeysDuringEdit(), and naturally can be presented as a boolean,
// but is declared as a byte type to be used in an option select rather than checkbox (for demonstration purposes)
byte invert = 1;
SelectOptionByte selectInvertOptions[] = {{"Invert", 1}, {"Normal", 0}};
GEMSelect selectInvert(sizeof(selectInvertOptions)/sizeof(SelectOptionByte), selectInvertOptions);

// Create menu item for option select with applyInvert() callback function
void applyInvert(); // Forward declaration
GEMItem menuItemInvert("Chars order:", invert, selectInvert, applyInvert);

// Create variable holding appearance of menu pointer that will be editable through option select and create associated option select.
byte menuPointer = GEM_POINTER_ROW;
SelectOptionByte selectMenuPointerOptions[] = {{"Row", GEM_POINTER_ROW}, {"Dash", GEM_POINTER_DASH}};
GEMSelect selectMenuPointer(sizeof(selectMenuPointerOptions)/sizeof(SelectOptionByte), selectMenuPointerOptions);

// Create menu item for option select with applyMenuPointer() callback function
void applyMenuPointer(); // Forward declaration
GEMItem menuItemMenuPointer("Menu pointer:", menuPointer, selectMenuPointer, applyMenuPointer);

// Create variable that will temporarily hold title of new Todo list item
char newItemTitle[GEM_STR_LEN];

// Create menu item for title of new Todo list item
void editTitle(); // Forward declaration
GEMItem menuItemTitle("Title:", newItemTitle, editTitle);

// Create menu button that will trigger addItem() function. It will add new record to Todo list.
// We will write (define) this function later. However, we should
// forward-declare it in order to pass to GEMItem constructor
void addItem(); // Forward declaration
GEMItem menuItemButtonAdd("Add", addItem);

// Create menu button that will trigger clearItems() function. It will remove completed items from Todo list.
// We will write (define) this function later. However, we should
// forward-declare it in order to pass to GEMItem constructor
void clearCompleted(); // Forward declaration
GEMItem menuItemButtonClear("Clear completed", clearCompleted);

// Create menu button that will trigger clearAll() function. It will remove all items from Todo list.
// We will write (define) this function later. However, we should
// forward-declare it in order to pass to GEMItem constructor
void clearAll(); // Forward declaration
GEMItem menuItemButtonClearAll("Clear all", clearAll);

// Create menu page object of class GEMPage. Menu page holds menu items (GEMItem) and represents menu level.
// Menu can have multiple menu pages (linked to each other) with multiple menu items each
GEMPage menuPageMain("Main Menu"); // Main page
GEMPage menuPageList("Todo", menuPageMain); // Todo list submenu
GEMPage menuPageAdd("Add Item", menuPageList); // Add item submenu
GEMPage menuPageManage("Manage", menuPageMain); // Manage submenu
GEMPage menuPageSettings("Settings", menuPageMain); // Settings submenu

// Create menu item links to submenu pages
GEMItem menuItemLinkList("List", menuPageList); // Create menu item linked to List menu page
GEMItem menuItemLinkAdd("Add+", menuPageAdd); // Create menu item linked to Add menu page
GEMItem menuItemLinkManage("Manage", menuPageManage); // Create menu item linked to Manage menu page
GEMItem menuItemLinkSettings("Settings", menuPageSettings); // Create menu item linked to Settings menu page

// Create GEMAppearance objects
GEMAppearance appearanceGeneral = { /* menuPointerType= */ menuPointer, /* menuItemsPerScreen= */ GEM_ITEMS_COUNT_AUTO, /* menuItemHeight= */ 10, /* menuPageScreenTopOffset= */ 10, /* menuValuesLeftOffset= */ 86};
GEMAppearance appearanceList = appearanceGeneral;
GEMAppearance appearanceAdd = appearanceGeneral;

// Create menu object of class GEM. Supply its constructor with reference to glcd object we created earlier
GEM menu(glcd, appearanceGeneral);

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

  menu
    // Turn inverted order of characters during edit mode on (feels more natural when using encoder)
    .invertKeysDuringEdit(invert)
    // Set custom splash
    .setSplash(splashBits)
    // Menu init, setup and draw
    .init();
  setupMenu();

  calculateFreeRam();
  if (freeRam < 0) {
    // Hide RAM counter if not available or rolled over an int value
    menuItemRam.hide();
  }

  menu.drawMenu();

  Serial.println(F("Initialized"));
}

void setupMenu() {
  // Add menu items to menu page
  menuPageMain
    .addMenuItem(menuItemLinkList)
    .addMenuItem(menuItemLinkManage)
    .addMenuItem(menuItemLinkSettings);

  appearanceList.menuValuesLeftOffset = 118;

  // Add menu items to List menu page
  menuPageList
    .setAppearance(&appearanceList)
    .addMenuItem(menuItemLinkAdd);
  
  // Turn on adjusted order of ASCII characters when editing title
  menuItemTitle.setAdjustedASCIIOrder();
  
  appearanceAdd.menuValuesLeftOffset = 46;
  
  // Add menu items to Add menu page
  menuPageAdd
    .setAppearance(&appearanceAdd)
    .addMenuItem(menuItemTitle)
    .addMenuItem(menuItemButtonAdd);
  
  // Add menu items to Manage menu page
  menuPageManage
    .addMenuItem(menuItemRam)
    .addMenuItem(menuItemButtonClear)
    .addMenuItem(menuItemButtonClearAll);
 
  // Add menu items to Settings menu page
  menuPageSettings
    .addMenuItem(menuItemInvert)
    .addMenuItem(menuItemMenuPointer);

  // Set List page as a starting one
  menu.setMenuPageCurrent(menuPageList);

  // Hide Add button by default (until Todo item title is entered)
  menuItemButtonAdd.hide();
}

// loop() is primarily used to manage rotary encoder operation,
// with six push-buttons instead it is much shorter
void loop() {
  // Get current time to use later on
  now = millis();

  // If menu is ready to accept button press...
  if (menu.readyForKey()) {
    // ...detect key press using KeyDetector library
    // and pass pressed button to menu
    myKeyDetector.detect();

    // Calculate RAM each loop iteration
    calculateFreeRam();
  
    switch (myKeyDetector.trigger) {
      case KEY_C:
        // Button was pressed
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
            menu.registerKeyPress(GEM_KEY_LEFT);
            // Button was in a pressed state during rotation of the knob, acting as a modifier to rotation action
            secondaryPressed = true;
          } else {
            // ...or GEM_KEY_UP otherwise
            menu.registerKeyPress(GEM_KEY_UP);
          }
        } else {
          // If channel B is high then the knob was rotated CW
          if (myKeyDetector.current == KEY_C) {
            // If push-button was pressed at that time, then treat this action as GEM_KEY_RIGHT,...
            menu.registerKeyPress(GEM_KEY_RIGHT);
            // Button was in a pressed state during rotation of the knob, acting as a modifier to rotation action
            secondaryPressed = true;
          } else {
            // ...or GEM_KEY_DOWN otherwise
            menu.registerKeyPress(GEM_KEY_DOWN);
          }
        }
        break;
      case KEY_C:
        // Button was released
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
            // Treat key that was pressed as Cancel button
            menu.registerKeyPress(GEM_KEY_CANCEL);
            cancelPressed = true;
          }
          break;
      }
    }
  }
}

void flashButtonTitle(const char* title, bool redraw = true) {
  GEMItem* menuItemButtonTmp = menu.getCurrentMenuPage()->getCurrentMenuItem();
  const char* titleOrig = menuItemButtonTmp->getTitle();
  menuItemButtonTmp->setTitle(title);
  menu.drawMenu();
  delay(1000);
  menuItemButtonTmp->setTitle(titleOrig);
  if (redraw) {
    menu.drawMenu();
  }
}

void applyInvert() {
  menu.invertKeysDuringEdit(invert);

  // Print invert variable to Serial
  Serial.print(F("Invert: "));
  Serial.println(invert);
}

void applyMenuPointer() {
  appearanceGeneral.menuPointerType = menuPointer;
  menu.setAppearance(appearanceGeneral); // Need to call setAppearance() when changing general appearance
  appearanceList.menuPointerType = menuPointer; // No need to call setAppearance() when changing apperance of menu pages, because it submitted as a pointer
  appearanceAdd.menuPointerType = menuPointer;

  // Print invert variable to Serial
  Serial.print(F("Menu pointer: "));
  Serial.println(menuPointer);
}

void editTitle() {
  menuItemButtonAdd.hide(newItemTitle[0] == '\0');
}

void addItem() {
  if (newItemTitle[0] != '\0') {
    Serial.print(F("Add Item: "));
    Serial.println(newItemTitle);
    
    // Creating new TodoItem object and adding corresponding menu item to menu page
    menuItemLinkAdd.hide(); // Temporarily hide Add button to add new item at the end of the list (but before hidden button)
    TodoItem* tempItem = new TodoItem(newItemTitle);
    tempItem->menuItem->setCallbackVal(tempItem); // Save pointer to Todo item in a GEMCallbackData struct inside corresponding menu item
    menuPageList.addMenuItem(*tempItem->menuItem, GEM_LAST_POS, GEM_ITEMS_VISIBLE);
    menuItemLinkAdd.show();
    memset(newItemTitle, '\0', GEM_STR_LEN - 1);

    // Temporarily change title of Add button, but w/o redrawing menu (because we will hide it)
    flashButtonTitle("Item added!", false);

    menuItemButtonAdd.hide();
  }
  calculateFreeRam();
  menu.drawMenu();
}

void clearItems(bool onlyCompleted = true) {
  GEMItem* menuItemTmp = menuPageList.getMenuItem(1); // Get first Todo item in a list to start traversing through menu items
  Serial.println(F("Clearing items:"));
  while (menuItemTmp->getLinkedVariablePointer() != nullptr) {
    GEMItem* nextItem = menuItemTmp->getMenuItemNext(); // Save pointer to a next item
    bool completed = *(bool*)menuItemTmp->getLinkedVariablePointer(); // Save completed status
    if (completed || !onlyCompleted) {
      // If linked boolean variable is true, then consider Todo item completed and ready to be removed 
      // (and remove it anyway in case if onlyCompleted set to false)
      Serial.print(completed ? "[x]" : "[ ]");
      Serial.println(menuItemTmp->getTitle());
      TodoItem* todoItemTmp = (TodoItem*)menuItemTmp->getCallbackData().valPointer; // Get pointer to corresponsing TodoItem object
      menuItemTmp->remove(); // Remove menu item from menu page
      delete menuItemTmp; // Delete GEMItem object
      delete todoItemTmp; // Delete TodoItem object
      /*
        Note 1: sometimes (e.g. on ARM-based MCUs, but not on AVR or ESP32) deleting completed Todo items
        doesn't immediately reflect on the amount of free RAM (as reported by getFreeRam()), however
        if new Todo item is created afterwards (after deleting completed one) free RAM counter won't change.
        That shows that deleteing objects is actually works (just not always reflected on the visible amount of free RAM,
        probably for reasons discussed here: https://forum.arduino.cc/t/memory-no-getting-cleaned-up-after-delete/894404),
        and new item presumably occupies previously freed memory. This may be related to so-called "buried heap space".
        Note 2: amount of displayed free RAM may change after moving cursor from button after clearing, for the same amount every
        time for some reason, even if no actual Todo items was deleted (it may something to do with processes needed to redraw
        menu and/or stack allocation for clearItems() call). However, previous statement (Note 1) still holds.
      */
    }
    menuItemTmp = nextItem;
  }
  calculateFreeRam();
  menu.drawMenu();
}

void clearCompleted() {
  clearItems();
  flashButtonTitle("Cleared!");
}

void clearAll() {
  clearItems(false);
  flashButtonTitle("Cleared!");
}
