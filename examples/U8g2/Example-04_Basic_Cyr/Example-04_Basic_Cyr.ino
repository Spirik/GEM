/*
  Basic menu example using GEM library. Cyrillic version.

  Simple one page menu with one editable menu item associated with int variable, one with boolean variable,
  and a button, pressing of which will result in int variable value printed to Serial monitor if boolean variable is set to true.

  Note, there might be some problems with displaying Cyrillic characters in Serial monitor, in which case adjusting baud rate may help.

  U8g2lib library is used to draw menu and to detect push-buttons presses.
  
  Additional info (including the breadboard view) available on GitHub:
  https://github.com/Spirik/GEM
  
  This example code is in the public domain.

  ======================

  Пример использования библиотеки GEM с поддержкой кириллицы в элементах интерфейса.

  Простое одностраничное меню, состояющее из пунктов с int переменной, boolean переменной и кнопки, нажатие на которую приведёт
  к печати значения int переменной в Serial monitor в случае, если значение boolean переменной установлено в true.

  Обратите внимание, что для стабильного отображения кириллицы в Serial monitor может потребоваться дополнительная настройка
  скорости передачи последовательного порта.

  Библиотека U8g2lib используется для вывода меню на LCD экран и для определения нажатий клавиш управления.

  Подробная документация доступна на GitHub:
  https://github.com/Spirik/GEM

  Этот пример кода находится в свободном доступе.
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
boolean enablePrint = false;

// Create two menu item objects of class GEMItem, linked to number and enablePrint variables 
GEMItem menuItemInt("Число:", number);
GEMItem menuItemBool("Вкл. печать:", enablePrint);

// Create menu button that will trigger printData() function. It will print value of our number variable
// to Serial monitor if enablePrint is true. We will write (define) this function later. However, we should
// forward-declare it in order to pass to GEMItem constructor
void printData(); // Forward declaration
GEMItem menuItemButton("Вывести на печать", printData);

// Create menu page object of class GEMPage. Menu page holds menu items (GEMItem) and represents menu level.
// Menu can have multiple menu pages (linked to each other) with multiple menu items each
GEMPage menuPageMain("Главное меню");

// Create menu object of class GEM_u8g2. Supply its constructor with reference to u8g2 object we created earlier
GEM_u8g2 menu(u8g2);

void setup() {
  // Serial communication setup
  Serial.begin(115200);

  // U8g2 library init. Pass pin numbers the buttons are connected to.
  // The push-buttons should be wired with pullup resistors (so the LOW means that the button is pressed)
  u8g2.begin(/*Select/OK=*/ 7, /*Right/Next=*/ 4, /*Left/Prev=*/ 3, /*Up=*/ 5, /*Down=*/ 2, /*Home/Cancel=*/ 6);

  // Enable Cyrillic typeset
  menu.enableCyrillic();
  
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
    // Note, there might be some problems w/ displaying Cyrillic characters in Serial monitor,
    // in which case adjusting baud rate may help
    Serial.print("Число: ");
    Serial.println(number);
  } else {
    Serial.println("Печать не разрешена:(");
  }
}
