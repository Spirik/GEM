
![GEM](http://spirik.ru/downloads/misc/gem/gem-logo.svg)
===========

GEM (a.k.a. *Good Enough Menu*) - Arduino library for creation of graphic multi-level menu with editable menu items, such as variables (supports `int`, `byte`, `boolean`, `char[17]` data types) and option selects. User-defined callback function can be specified to invoke when menu item is saved.
  
Supports buttons that can invoke user-defined actions and create action-specific context, which can have its own enter (setup) and exit callbacks as well as loop function.

<p align="center">
<img src="https://github.com/Spirik/GEM/wiki/images/party-hard-lcd_full-demo_p12_640x360_256c_mask.gif" width="640" height="360" alt="Party hard!" />
</p>

Requires [AltSerialGraphicLCD](http://www.jasspa.com/serialGLCD.html) library by Jon Green. LCD screen must be equipped with [SparkFun Graphic LCD Serial Backpack](https://www.sparkfun.com/products/9352) and properly set up to operate using firmware provided with aforementioned library.

* [When to use](#when-to-use)
* [Structure](#structure)
* [How to use](#how-to-use)
* [Reference](#reference)
  * [GEM](#gem-1)
  * [GEMPage](#gempage)
  * [GEMItem](#gemitem)
  * [GEMSelect](#gemselect)
  * [AppContext](#appcontext)
* [Examples](#examples)
* [License](#license)
* [**Wiki**](https://github.com/Spirik/GEM/wiki)

When to use
-----------
If you want to equip your project with graphical LCD display and let user to choose different options and settings to configure its operation. Whether it is control panel of smart home or simple configurable LED strip, GEM will provide all necessary controls for editing variables and navigating through submenus, as well as running user-defined functions.

Structure
-----------
Menu created with GEM library comprises of three base elements:

 - menu item (`GEMItem` class) - represents associated variable, button, or link to the next menu level (menu page);
 - menu page (`GEMPage` class) - consists of list of menu items and represents menu level;
 - menu object itself (`GEM` class) - can have multiple menu pages (linked to each other) with multiple menu items each.

![GEM structure](https://github.com/Spirik/GEM/wiki/images/gem-structure.png)

How to use
-----------

### Requirements

GEM library requires [AltSerialGraphicLCD](http://www.jasspa.com/serialGLCD.html) library to operate. LCD screen must be equipped with [SparkFun Graphic LCD Serial Backpack](https://www.sparkfun.com/products/9352) and properly set up to operate using firmware provided with AltSerialGraphicLCD. Installation and configuration of it is covered in great detail in AltSerialGraphicLCD manual.

In theory GEM is compatible with any display, that is supported by SparkFun Graphic LCD Serial Backpack. Guaranteed to work with [128x64](https://www.sparkfun.com/products/710) pixel displays. [160x128](https://www.sparkfun.com/products/8799) pixel ones should work fine as well, although that is yet to be tested.

### Install

Library format is compatible with Arduino IDE 1.5.x+. There are two ways to install the library:

- Download ZIP-archive directly from [Releases](https://github.com/Spirik/GEM/releases) section (or Master branch) and extract it into GEM folder inside your Library folder.
- Using Library Manager (since Arduino IDE 1.6.2): navigate to `Sketch > Include Library > Manage Libraries` inside your Arduino IDE and search for GEM library, then click `Install`. (Alternatively you can add previously downloaded ZIP through `Sketch > Include Library > Add .ZIP Library` menu).

Whichever option you choose you may need to reload IDE afterwards.

### Import

To include GEM library add the following line at the top of your sketch:

```cpp
#include <GEM.h>
```

AltSerialGraphicLCD library will be included automatically through GEM library, so no need to include it explicitly in your sketch (although it still needs to be installed in your system, of course).

In order to communicate with your SparkFun Graphic LCD Serial Backpack, AltSerialGraphicLCD library uses [SoftwareSerial](https://www.arduino.cc/en/Reference/SoftwareSerial) (that ships with Arduino IDE). There is no need to explicitly include it in your sketch as well, because it is already included through AltSerialGraphicLCD library. So the following line is completely optional (although note that second inclusion won't affect the size of you sketch) - you may omit it and use SoftwareSerial as though it was included.

```cpp
#include <SoftwareSerial.h>
```
> Note that it is possible to use hardware serial instead (e.g. if you're planning to use it with Arduino Leonardo's `Serial1` class), however some modifications of AltSerialGraphicLCD library would be required in that case.

One more additional library that may come in handy (although is not necessary) is [KeyDetector](https://github.com/Spirik/KeyDetector) - it is small and lightweight library for key press events detection. It is used in some of the supplied examples (as well as the following) to detect button presses for navigation through menu. To include KeyDetector library, install it first and then add the following line:

```cpp
#include <KeyDetector.h>
```
### Use

Assume you have a simple setup as follows:

 - 128x64 LCD screen equipped with SparkFun Graphic LCD Serial Backpack, which is properly connected to the power source and to digital pins 8 and 9 of your Arduino for serial communication via SoftwareSerial library;
 - also you have 6 push-buttons (momentary switches) connected to the digital pins 2 to 7, wired with pulldown resistors (so the HIGH means that the button is pressed).

![Basic example breadboard](https://github.com/Spirik/GEM/wiki/images/ex_GEM_01_basic_bb_edited_1776.png)

Let's create a simple one page menu with one editable menu item associated with `int` variable, one with `boolean` variable, and a button, pressing of which will result in `int` variable value printed to Serial monitor if `boolean` variable is set to `true`. To navigate through menu we will use 6 push-buttons connected to the Arduino (for four directional controls, one Cancel, and one Ok). For the sake of simplicity we will use KeyDetector library to detect single button presses (as we need a way to prevent continuously pressed button from triggering press event multiple times in a row).

> For more detailed examples and tutorials please visit GEM [wiki](https://github.com/Spirik/GEM/wiki).

#### Navigation buttons initial setup (via KeyDetector library)

Create constants for the pins you want to detect signals on (these are the pins the push-buttons are connected to):

```cpp
const byte downPin = 2;
const byte leftPin = 3;
const byte rightPin = 4;
const byte upPin = 5;
const byte cancelPin = 6;
const byte okPin = 7;
```

Create an array of `Key` objects. It will hold information about which button press event should be detected on which input pin:

```cpp
Key keys[] = {{GEM_KEY_UP, upPin}, {GEM_KEY_RIGHT, rightPin}, {GEM_KEY_DOWN, downPin}, {GEM_KEY_LEFT, leftPin}, {GEM_KEY_CANCEL, cancelPin}, {GEM_KEY_OK, okPin}};
```

> **Note:** aliases `GEM_KEY_UP`, `GEM_KEY_RIGHT`, `GEM_KEY_DOWN`, `GEM_KEY_LEFT`, `GEM_KEY_CANCEL`, and `GEM_KEY_OK` are predefined and come with the GEM library. They represent identifiers of buttons that menu listens and responds to. E.g. sending menu `GEM_KEY_DOWN` will trigger it to move cursor down and highlight the next menu item, etc.

Create `KeyDetector` object called `myKeyDetector` and supply its constructor with `keys` array created at the previous step and explicitly pass the size of the array:

```cpp
KeyDetector myKeyDetector(keys, sizeof(keys)/sizeof(Key));
```

Navigation buttons initial setup is now complete.

#### LCD initial setup (via SoftwareSerial and AltSerialGraphicLCD libraries)

Create constants for the pins SparkFun Graphic LCD Serial Backpack is connected to:

```cpp
const byte rxPin = 8;
const byte txPin = 9;
```

Initialize an instance of the SoftwareSerial library called `serialLCD`:

```cpp
SoftwareSerial serialLCD(rxPin, txPin);
```

Create an instance of the `GLCD` class named `glcd`. This instance is used to call all the subsequent GLCD functions (internally from GEM library, or manually in your sketch if it is required). Instance is created with a reference to the software serial object:

```cpp
GLCD glcd(serialLCD);
```

LCD initial setup is now complete.

#### Menu initial setup

Create variables that you would like to be editable through the menu. Assign them initial values:

```cpp
int number = -512;
boolean enablePrint = false;
```

Create two menu item objects of class `GEMItem`, linked to `number` and `enablePrint` variables. Let's name them simply "Number" and "Enable print" accordingly - these names will be printed on screen:

```cpp
GEMItem menuItemInt("Number:", number);
GEMItem menuItemBool("Enable print:", enablePrint);
```

Create menu button that will trigger `printData()` function. It will print value of our `number` variable to Serial monitor if `enablePrint` is `true`. We will write (define) this function later. However we should forward-declare it in order to pass its reference to `GEMItem` constructor. Let's name our button "Print":

```cpp
void printData(); //Forward declaration
GEMItem menuItemButton("Print", printData);
```

Create menu page object of class `GEMPage`. Menu page holds menu items (`GEMItem`) and, in fact, represents menu level. Menu can have multiple menu pages (linked to each other) with multiple menu items each. Let's call our only menu page "Main Menu":

```cpp
GEMPage menuPageMain("Main Menu");
```

And finally, create menu object of class `GEM`. Supply its constructor with a reference to `glcd` object we created earlier:

```cpp
GEM menu(glcd);
```

> **Note:** `GEM` constructor supports additional optional parameters that can customize look of the menu. See [Reference](#reference) and [wiki](https://github.com/Spirik/GEM/wiki) for details.

We will link menu items to menu pages to menu in `setup()` function. For now, menu initial setup is complete.

#### setup() function

In `setup()` function of the sketch define modes of the pins push-buttons are connected to:

```cpp
pinMode(downPin, INPUT);
pinMode(leftPin, INPUT);
pinMode(rightPin, INPUT);
pinMode(upPin, INPUT);
pinMode(cancelPin, INPUT);
pinMode(okPin, INPUT);
```

Init serial communications (both native `Serial` and software `serialLCD`):

```cpp
Serial.begin(115200);
serialLCD.begin(115200);
```

Reset LCD to its initial state. Using delays here is optional, but I've found that it actually may help certain LCDs to regain their initial state more effectively after being previously unexpectedly shut down. Adjust the delays to what best works for your display.

```cpp
delay(500);
glcd.reset();
delay(1000);
```

If you still encounter spontaneous freezing on load (especially right after sketch uploads), you may want to add the second instance of reset instructions. This (almost) bulletproof solution will ensure that screen boots and properly resets no matter what.

```cpp
// Uncomment the following lines in dire situations
// (e.g. when screen becomes unresponsive after shutdown)
glcd.reset();
delay(1000);
```

Init menu. That will run some initialization routines (e.g. load sprites into LCD Serial Backpack's internal memory), then show splash screen (which can be customized).

```cpp
menu.init();
```

The next step is to gather all of the previously declared menu items and pages together and assign them to our menu. It is convenient to do that in a separate function. Let's call it `setupMenu()`. We will define it later.

```cpp
setupMenu();
```

And finally, draw menu to the screen:

```cpp
menu.drawMenu();
```

`setup()` function of the sketch is now complete:

```cpp
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
  SerialLCD.begin(115200);

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
```

#### setupMenu() function

Let's assemble our menu. First, add menu items to menu page:

```cpp
menuPageMain.addMenuItem(menuItemInt);
menuPageMain.addMenuItem(menuItemBool);
menuPageMain.addMenuItem(menuItemButton);
```

Because we don't have multiple menu levels, all we left to do now is to add our only menu page to menu and set it as initial menu page (loaded when menu first drawn):

```cpp
menu.setMenuPageCurrent(menuPageMain);
```

`setupMenu()` is now complete:

```cpp
void setupMenu() {
  // Add menu items to menu page
  menuPageMain.addMenuItem(menuItemInt);
  menuPageMain.addMenuItem(menuItemBool);
  menuPageMain.addMenuItem(menuItemButton);

  // Add menu page to menu and set it as current
  menu.setMenuPageCurrent(menuPageMain);
}
```

#### loop() function

In the `loop()` function of the sketch we'll be listening to push-buttons presses (using `KeyDetector`) and delegate pressed button to menu: 

```cpp
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
```

#### Button action

Let's define `printData()` function that we declared earlier. It will be invoked each time the "Print" button in our menu is pressed.  It should print value of our `number` variable to Serial monitor if `enablePrint` is `true`.

```cpp
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
```

> This is the simplest action that menu item button can have. More elaborate versions make use of custom "[context](#appcontext)" that can be created when button is pressed. In that case, button action can have its own setup and loop functions (named `context.enter()` and `context.loop()`) that run similarly to how sketch operates. It allows you to initialize variables and e.g. prepare screen (if needed for the task that function performs), and then run through loop function, waiting for user input, or sensor reading, or command to terminate and exit back to the menu eventually. In the latter case additional `context.exit()` function will be called, that can be used to clean up your context and e.g. to free some memory and draw menu back to screen.

#### Sketch

Full version of this basic example is shipped with the library and can be found at "examples/Example-01_Basic/[Example-01_Basic.ino](https://github.com/Spirik/GEM/blob/master/examples/Example-01_Basic/Example-01_Basic.ino)".

#### Run

After compiling and uploading sketch to Arduino, wait while LCD screen boots and menu is being initialized and drawn to the screen. Then start pressing the push-buttons and navigate through the menu. Pressing "Ok" button (attached to pin 7) will trigger edit mode of the "Number" variable, or change state of "Enable print" option, or invoke action associated with "Print" menu button (depending on which menu item is currently selected). If "Enable print" option is checked, then pressing "Print" button will result in `number` variable printed to the Serial Monitor.

![Basic example](https://github.com/Spirik/GEM/wiki/images/gem-ex-01-basic-run.gif)

To learn more about GEM library, see the following Reference section and visit [wiki](https://github.com/Spirik/GEM/wiki).

Reference
-----------

### GEM

Primary class of library. Responsible for appearance of the menu, communication with LCD screen (via supplied `GLCD` object) connected to SparkFun Graphic LCD Serial Backpack, integration of all menu items `GEMItem` and pages `GEMPage` into one menu. Object of class `GEM` defines as follows:

```cpp
GEM menu(glcd[, menuPointerType[, menuItemsPerScreen[, menuItemHeight[, menuPageScreenTopOffset[, menuValuesLeftOffset]]]]]);
```

* **glcd**  
  *Type*: `GLCD`  
  Holds the reference to a `GLCD` object created with AltSerialGraphicLCD library and used for communication with SparkFun Graphic LCD Serial Backpack.

* **menuPointerType**  [*optional*]  
  *Type*: `byte`  
  *Values*: `GEM_POINTER_ROW`, `GEM_POINTER_DASH`  
  *Default*: `GEM_POINTER_ROW`  
  Type of menu pointer visual appearance: either highlighted row or pointer to the left of the row.

* **menuItemsPerScreen** [*optional*]  
  *Type*: `byte`   
  *Default*: `5`  
  Count of the menu items per screen. Suitable for 128x64 screen with other variables at their default values.

* **menuItemHeight** [*optional*]  
  *Type*: `byte`  
  *Units*: dots  
  *Default*: `10`  
  Height of the menu item. Suitable for 128x64 screen with other variables at their default values.

* **menuPageScreenTopOffset** [*optional*]  
  *Type*: `byte`  
  *Units*: dots  
  *Default*: `10`  
  Offset from the top of the screen to accommodate title of the menu page. Suitable for 128x64 screen with other variables at their default values.

* **menuValuesLeftOffset** [*optional*]  
  *Type*: `byte`  
  *Units*: dots  
  *Default*: `86`  
  Offset from the left of the screen to the value of the associated with menu item variable (effectively the space left for the title of the menu item to be printed on screen). Suitable for 128x64 screen with other variables at their default values; 86 - maximum value for 128x64 screen.

![GEM customization](https://github.com/Spirik/GEM/wiki/images/customization.gif)

> **Note:** carefully choose values of `menuItemsPerScreen`, `menuItemHeight`, `menuPageScreenTopOffset`, `menuValuesLeftOffset` in accordance to the actual size of your LCD screen. Default values of these options are suitable for 128x64 screens. But that is not the only possible option: the other combination of values you set may also be suitable - just calculate them correctly and see what works best for you.

For more details on customization see corresponding section of the [wiki](https://github.com/Spirik/GEM/wiki).

#### Constants

* **GEM_POINTER_DASH**  
  *Type*: macro `#define GEM_POINTER_DASH 0`  
  *Value*: `0`  
  Alias for the type of menu pointer visual appearance (submitted as **menuPointerType** setting to `GEM` constructor): pointer to the left of the row.

* **GEM_POINTER_ROW**  
  *Type*: macro `#define GEM_POINTER_ROW 1`  
  *Value*: `1`  
  Alias for the type of menu pointer visual appearance (submitted as **menuPointerType** setting to `GEM` constructor): highlighted row.

* **GEM_KEY_NONE**  
  *Type*: macro `#define GEM_KEY_NONE 0`  
  *Value*: `0`  
  Alias for the keys (buttons) used to navigate and interact with menu. Submitted to `GEM::registerKeyPress()` method. Indicates that no key presses were detected.

* **GEM_KEY_UP**  
  *Type*: macro `#define GEM_KEY_UP 1`  
  *Value*: `1`  
  Alias for the keys (buttons) used to navigate and interact with menu. Submitted to `GEM::registerKeyPress()` method. Indicates that Up key is pressed (navigate up through the menu items list, select next value of the digit/char of editable variable, or previous option in select).

* **GEM_KEY_RIGHT**  
  *Type*: macro `#define GEM_KEY_RIGHT 2`  
  *Value*: `2`  
  Alias for the keys (buttons) used to navigate and interact with menu. Submitted to `GEM::registerKeyPress()` method. Indicates that Right key is pressed (navigate through the link to another (child) menu page, select next digit/char of editable variable, execute code associated with button).

* **GEM_KEY_DOWN**  
  *Type*: macro `#define GEM_KEY_DOWN 3`  
  *Value*: `3`  
  Alias for the keys (buttons) used to navigate and interact with menu. Submitted to `GEM::registerKeyPress()` method. Indicates that Down key is pressed (navigate down through the menu items list, select previous value of the digit/char of editable variable, or next option in select).

* **GEM_KEY_LEFT**  
  *Type*: macro `#define GEM_KEY_LEFT 4`  
  *Value*: `4`  
  Alias for the keys (buttons) used to navigate and interact with menu. Submitted to `GEM::registerKeyPress()` method. Indicates that Left key is pressed (navigate through the Back button to the previous menu page, select previous digit/char of editable variable).

* **GEM_KEY_CANCEL**  
  *Type*: macro `#define GEM_KEY_CANCEL 5`  
  *Value*: `5`  
  Alias for the keys (buttons) used to navigate and interact with menu. Submitted to `GEM::registerKeyPress()` method. Indicates that Cancel key is pressed (navigate to the previous (parent) menu page, exit edit mode without saving the variable, exit context loop if allowed within context's settings).

* **GEM_KEY_OK**  
  *Type*: macro `#define GEM_KEY_OK 6`  
  *Value*: `6`  
  Alias for the keys (buttons) used to navigate and interact with menu. Submitted to `GEM::registerKeyPress()` method. Indicates that Ok/Apply key is pressed (toggle boolean menu item, enter edit mode of the associated non-boolean variable, exit edit mode with saving the variable, execute code associated with button).

#### Methods

* **setSplash(** _uint8_t PROGMEM_ *sprite **)**  
  *Returns*: nothing  
  Set custom sprite displayed as the splash screen when GEM is being initialized. Should be called before `GEM::init()`. The following is the format of the sprite as described in AltSerialGraphicLCD library documentation:
  > The sprite commences with two bytes which are the width and height of the image in pixels. The pixel data is organised as rows of 8 vertical pixels per byte where the least significant bit (LSB) is the top-left pixel and the most significant bit (MSB) tends towards the bottom-left pixel. A complete row of 8 vertical pixels across the image width comprises the first row, this is then followed by the next row of 8 vertical pixels and so on. Where the image height is not an exact multiple of 8 bits then any unused bits are typically set to zero (although this does not matter).

  For more details on splash customization see corresponding section of the [wiki](https://github.com/Spirik/GEM/wiki).

* **hideVersion(** _boolean_ flag = true **)**  
  *Accepts*: `boolean`  
  *Returns*: nothing  
  Turn printing of the current GEM library version on splash screen off (`hideVersion()`) or back on (`hideVersion(false)`). By default the version is printed. Should be called before `GEM::init()`.

* **init()**  
  *Returns*: nothing  
  Init the menu: load necessary sprites into RAM of the SparkFun Graphic LCD Serial Backpack, display GEM splash screen, etc.
  > The following `GLCD` object settings will be adjusted during `init()`: 
  > * `glcd.drawMode(GLCD_MODE_NORMAL)`;
  > * `glcd.fontMode(GLCD_MODE_NORMAL)`;
  > * `glcd.set(GLCD_ID_CRLF, 0)`;
  > * `glcd.set(GLCD_ID_SCROLL, 0)`.
  > 
  > Keep this in mind if you are planning to use the same object in your own routines.

* **setMenuPageCurrent(** _GEMPage&_ menuPageCurrent **)**  
  *Accepts*: `GEMPage`  
  *Returns*: nothing  
  Set supplied menu page as current. Accepts `GEMPage` object.

* **drawMenu()**  
  *Returns*: nothing  
  Draw menu on screen, with menu page set earlier in `GEM::setMenuPageCurrent()`.

* *boolean* **readyForKey()**  
  *Returns*: `boolean`  
  Check that menu is waiting for the key press.

* **registerKeyPress(** _byte_ keyCode **)**  
  *Accepts*: `byte` (*Values*: `GEM_KEY_NONE`, `GEM_KEY_UP`, `GEM_KEY_RIGHT`, `GEM_KEY_DOWN`, `GEM_KEY_LEFT`, `GEM_KEY_CANCEL`, `GEM_KEY_OK`)  
  *Returns*: nothing  
  Register the key press and trigger corresponding action (navigation through the menu, editing values, pressing menu buttons).

* **clearContext()**  
  *Returns*: nothing  
  Clear context. Assigns `nullptr` values to function pointers of the `context` property and sets `allowExit` flag of the `context` to `true`.

#### Properties

* **context**  
  *Type*: `AppContext`  
  Currently set [context](#appcontext).


----------


### GEMPage

Menu page holds menu items `GEMItem` and represents menu level. Menu can have multiple menu pages (linked to each other) with multiple menu items each. Object of class `GEMPage` defines as follows:

```cpp
GEMPage menuPage(title);
```

* **title**  
  *Type*: `char*`  
  Title of the menu page displayed at top of the screen.
  
  > **Note:** there is no explicit restriction on the length of the title: if it's more than 30 characters, it will overflow to the next line. It is possible to accommodate multiline menu titles by enlarging `menuPageScreenTopOffset` when initializing `GEM` object.

#### Methods

* **addMenuItem(** _GEMItem&_ menuItem **)**  
  *Accepts*: `GEMItem`  
  *Returns*: nothing  
  Add menu item to menu page. Accepts `GEMItem` object.

* **setParentMenuPage(** _GEMPage&_ parentMenuPage **)**  
  *Accepts*: `GEMPage`  
  *Returns*: nothing  
  Specify parent level menu page (to know where to go back to when pressing Back button, that will be added automatically). Accepts `GEMPage` object.


----------


### GEMItem

Menu item of the menu. Can represent editable or read-only variable of type `int`, `byte`, `boolean`, `char[17]` (or `char[GEM_STR_LEN]`, to be exact); option select of type `int`, `byte`, `char[n]`; link to another menu page; or button that can invoke user-defined actions and create action-specific context, which can have its own enter (setup) and exit callbacks as well as loop function. User-defined callback function can be specified to invoke when editable menu item is saved or option is selected. Exact definition of `GEMItem` object depends on its type.

#### Variable

```cpp
GEMItem menuItemVar(title, linkedVariable[, readonly]);
```
or
```cpp
GEMItem menuItemVar(title, linkedVariable[, saveCallback]);
```

* **title**  
  *Type*: `char*`  
  Title of the menu item displayed on the screen.

* **linkedVariable**  
  *Type*: `int`, `byte`, `char[17]` (or `char[GEM_STR_LEN]`, to be exact), `boolean`  
  Reference to variable that menu item is associated with.

* **readonly** [*optional*]  
  *Type*: `boolean`  
  *Values*: `GEM_READONLY` (alias for `true`), `false`  
  *Default*: `false`  
  Sets readonly mode for variable that menu item is associated with.

* **saveCallback** [*optional*]  
  *Type*: `pointer to function`  
  Pointer to callback function executed when associated variable is successfully saved.

> **Note:** you cannot specify both readonly mode and callback in the same constructor. However, you can set readonly mode for menu item with callback explicitly later using `GEMItem::setReadonly()` method.

#### Option select

```cpp
GEMItem menuItemSelect(title, linkedVariable, select[, readonly]);
```
or
```cpp
GEMItem menuItemSelect(title, linkedVariable, select[, saveCallback]);
```

* **title**  
  *Type*: `char*`  
  Title of the menu item displayed on the screen.

* **linkedVariable**  
  *Type*: `int`, `byte`, `char[n]`  
  Reference to variable that menu item is associated with. Note that in case of `char[n]` variable, character array should be big enough to hold select option with the longest value to avoid overflows. It can be greater then `GEM_STR_LEN` (i.e. it is possible to have `n` > 17) limit set for non-select menu item variable.

* **select**  
  *Type*: `GEMSelect`  
  Reference to [`GEMSelect`](#gemselect) option select object that represents a list of available values.

* **readonly** [*optional*]  
  *Type*: `boolean`  
  *Values*: `GEM_READONLY` (alias for `true`), `false`  
  *Default*: `false`  
  Sets readonly mode for variable that menu item is associated with.

* **saveCallback** [*optional*]  
  *Type*: `pointer to function`  
  Pointer to callback function executed when associated variable is successfully saved.

> **Note:** you cannot specify both readonly mode and callback in the same constructor. However, you can set readonly mode for menu item with callback explicitly later using `GEMItem::setReadonly()` method.

#### Link to menu page

```cpp
GEMItem menuItemLink(title, linkedPage);
```

* **title**  
  *Type*: `char*`  
  Title of the menu item displayed on the screen.

* **linkedPage**  
  *Type*: `GEMPage`  
  Menu page `GEMPage` that menu item is associated with.

#### Button

```cpp
GEMItem menuItemButton(title, buttonAction);
```

* **title**  
  *Type*: `char*`  
  Title of the menu item displayed on the screen (in this case - name of the button).

* **buttonAction**  
  *Type*: `reference to function`  
  Reference to function that will be executed when menu item is activated. Action-specific [context](#appcontext) can be created, which can have its own enter (setup) and exit callbacks as well as loop function.

#### Constants

* **GEM_READONLY**  
  *Type*: macro `#define GEM_READONLY true`  
  *Value*: `true`  
  Alias for readonly modifier of associated with menu item variable. Submitted as **readonly** setting to `GEMItem` constructor of variable menu items and option select.

* **GEM_STR_LEN**  
  *Type*: macro `#define GEM_STR_LEN 17`  
  *Value*: `17`  
  Alias for supported length of the string (character sequence) variable of type `char[GEM_STR_LEN]`. Note, that this limits the length of the string that can be used with editable character menu item variable, but option select variable doesn't have this restriction. But you still have to make sure that in the latter case character array should be big enough to hold select option with the longest value to avoid overflows.

#### Methods

* **setReadonly(** _boolean_ mode = true **)**  
  *Accepts*: `boolean`  
  *Returns*: nothing  
  Explicitly set (`setReadonly(true)`, or `setReadonly(GEM_READONLY)`, or `setReadonly()`) or unset (`setReadonly(false)`) readonly mode for variable that menu item is associated with. Relevant for `GEM_VAL_INTEGER`, `GEM_VAL_BYTE`, `GEM_VAL_CHAR`, `GEM_VAL_BOOLEAN` variable menu items and `GEM_VAL_SELECT` option select.

* *boolean* **getReadonly()**  
  *Returns*: `boolean`  
  Get readonly state for variable that menu item is associated with: `true` for readonly state, `false` otherwise.


----------


### GEMSelect

List of values available for option select. Supplied to `GEMItem` constructor. Object of class `GEMSelect` defines as follows:

```cpp
GEMSelect mySelect(length, optionsArray);
```

* **length**  
  *Type*: `byte`  
  Length of `optionsArray`. Should be explicitly supplied because array is passed by reference. Easy way to provide array length is to calculate it using the following expression: `sizeof(optionsArray)/sizeof(SelectOptionInt)`, or `sizeof(optionsArray)/sizeof(SelectOptionByte)`, or `sizeof(optionsArray)/sizeof(SelectOptionChar)` depending on the type of the array used.

* **optionsArray**  
  *Type*: `void*` (pointer to array of type either `SelectOptionInt`, or `SelectOptionByte`, or `SelectOptionChar`)  
  Array of the available options. Type of the array is either `SelectOptionInt`, or `SelectOptionByte`, or `SelectOptionChar` depending on kind of data options are selected from. See the following section for definition of these custom types.

Example of use:

```cpp
// Integer option select
// 1) Create array of options:
SelectOptionInt optionsArray[] = {{"Opt 1", 10}, {"Opt 2", -12}, {"Opt 3", 13}};
// 2) Supply array of options to GEMSelect constructor:
GEMSelect mySelect(sizeof(optionsArray)/sizeof(SelectOptionInt), optionsArray);
```
or
```cpp
// Integer option select
// GEMSelect constructor with anonymous options array (length of array (3) can't be calculated in this case and should be explicitly supplied):
GEMSelect mySelect(3, (SelectOptionInt[]){{"Opt 1", 10}, {"Opt 2", -12}, {"Opt 3", 13}});
```


----------


### SelectOptionInt

Data structure that represents option of the select of type `int`. Object of type `SelectOptionInt` defines as follows:

```cpp
SelectOptionInt selectOption = {name, val_int};
```

* **name**  
  *Type*: `char*`  
  Text label of the option as displayed in select.

* **val_int**  
  *Type*: `int`  
  Value of the option that is assigned to linked variable upon option selection.

### SelectOptionByte

Data structure that represents option of the select of type `byte`. Object of type `SelectOptionByte` defines as follows:

```cpp
SelectOptionByte selectOption = {name, val_byte};
```

* **name**  
  *Type*: `char*`  
  Text label of the option as displayed in select.

* **val_byte**  
  *Type*: `byte`  
  Value of the option that is assigned to linked variable upon option selection.

### SelectOptionChar

Data structure that represents option of the select of type `char*`. Object of type `SelectOptionChar` defines as follows:

```cpp
SelectOptionChar selectOption = {name, val_char};
```

* **name**  
  *Type*: `char*`  
  Text label of the option as displayed in select.

* **val_char**  
  *Type*: `char*`  
  Value of the option that is assigned to linked variable upon option selection. Note that character array of associated with menu item variable (of type `char[n]`) should be big enough to hold select option with the longest value to avoid overflows.


----------


### AppContext

Data structure that represents "context" of the currently executing user action, toggled by pressing menu item button. Property `context` of the `GEM` object is of type `AppContext`. 

Consists of pointers to user-supplied functions that represent setup and loop functions (named `context.enter()` and `context.loop()` respectively) of the context. It allows you to initialize variables and e.g. prepare screen (if needed for the task that function performs), and then run through loop function, waiting for user input, or sensor reading, or command to terminate and exit back to the menu eventually. In the latter case additional `context.exit()` function will be called, that can be used to clean up your context and e.g. to free some memory and draw menu back to screen.

Object of type `AppContext` defines as follows:

```cpp
AppContext myContext = {loop, enter, exit, allowExit};
```

* **loop**  
  *Type*: `pointer to function`  
  Pointer to `loop()` function of current context. Similar to regular `loop()` function; executed each regular `loop()` iteration. Usually contains code of user-defined action that is run when menu item button is pressed.

* **enter** [*optional*]  
  *Type*: `pointer to function`  
  Pointer to `enter()` function of current context. Similar to regular `setup()` function, called manually, generally once before context's `loop()` function; is optional. Usually contains some additional set up required by the user-defined action pointed to by context's `loop()`.

* **exit** [*optional*]  
  *Type*: `pointer to function`  
  Pointer to `exit()` function of current context. Called automatically when user exits currently running context if `context.allowExit` (see below) is set to `true`. Should be invoked manually otherwise. Usually contains instructions to do some cleanup after context's `loop()` and to draw menu on screen again (by calling `drawMenu()` method of `GEM` object). If no user-defined function specified, default action will be invoked that consists of call to two methods of `GEM` object: `drawMenu()` and `clearContext()`.

* **allowExit**
  *Type*: `boolean`  
  *Default*: `true`  
  Setting to `false` will require manually exit the context's `loop()` from within the loop itself: all necessary key detection should be done in context's `loop()` accordingly, and `context.exit()` should be called explicitly; otherwise exit is handled automatically by pressing `GEM_KEY_CANCEL` key.

Basic example of use:

```cpp
...

void buttonAction(); //Forward declaration
GEMItem menuItemButton("Blink!", buttonAction);

...

// Create menu object
GEM menu(glcd);

...

void buttonAction() {
  // Declaration of context functions
  menu.context.loop = buttonContextLoop;
  menu.context.enter = buttonContextEnter; // Optional
  menu.context.exit = buttonContextExit; // Optional
  // Call of context functions
  menu.context.enter();
  //menu.context.allowExit = false; // Set to false if required to manually exit the context loop from within the loop itself (default is true)
}

void buttonContextEnter() {
  // Running some user-defined routines that usually belongs to setup()
  // Clear LCD screen
  glcd.clearScreen();
  // Initialize digital pin LED_BUILTIN as an output.
  pinMode(LED_BUILTIN, OUTPUT);
}

void buttonContextLoop() {
  // Do something in the loop() function you would normally do in the sketch, e.g. blink built in LED
  digitalWrite(LED_BUILTIN, HIGH);
  delay(500);
  digitalWrite(LED_BUILTIN, LOW);
  delay(500);
}

void buttonContextExit() {
  // Draw menu back to screen
  menu.drawMenu();
  // Clear context (assigns `nullptr` values to function pointers of the `context` property of the `GEM` object and resets `allowExit` flag to its default state)
  menu.clearContext();
}

```

To exit currently running context and return to menu, press button associated with `GEM_KEY_CANCEL` key (only if `context.allowExit` flag is set to its default value of `true`, otherwise you should handle exit from the loop manually and call `context.exit()` explicitly) - `context.exit()` callback will be called.

For more details see supplied example on context usage and read corresponding section of the [wiki](https://github.com/Spirik/GEM/wiki).

Examples
-----------
GEM library comes with several annotated examples that will help you get familiar with it. More detailed info on the examples (including schematic and breadboard view where necessary) available in [wiki](https://github.com/Spirik/GEM/wiki).

License
-----------
This library is free software; you can redistribute it and/or modify it under the terms of the GNU Lesser General Public License as published by the Free Software Foundation; either version 3 of the License, or (at your option) any later version.

This library is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public License along with this library.  If not, see <http://www.gnu.org/licenses/>.