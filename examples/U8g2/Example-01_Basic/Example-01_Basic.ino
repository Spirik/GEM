/*
  Basic menu example using GEM library.

  Simple one page menu with one editable menu item associated with int variable, one with bool variable,
  and a button, pressing of which will result in int variable value printed to Serial monitor if bool variable is set to true.

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
int number = -512;
bool enablePrint = false;

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

// Create menu object of class GEM_u8g2. Supply its constructor with reference to u8g2 object we created earlier
GEM_u8g2 menu(u8g2, GEM_POINTER_ROW, GEM_ITEMS_COUNT_AUTO);
// Which is equivalent to the following call (you can adjust parameters to better fit your screen if necessary):
// GEM_u8g2 menu(u8g2, /* menuPointerType= */ GEM_POINTER_ROW, /* menuItemsPerScreen= */ GEM_ITEMS_COUNT_AUTO, /* menuItemHeight= */ 10, /* menuPageScreenTopOffset= */ 10, /* menuValuesLeftOffset= */ 86);

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
    // ...detect key press using U8g2 library
    // and pass pressed button to menu
    menu.registerKeyPress(u8g2.getMenuEvent());
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
