# pico-ssd1306-debugger
Debugging output for Raspberry PICO using OLED display (SSD1306) and PICO C++ SDK

## example implementation

```C++
#include "ssd1306.h"

void main(){
  SSD1306 display(128, 64, 0x3C);   // setup the object
  display.init_i2c(i2c0, 4, 5);     // start i2c
  display.init_display();           // initialize the display
  display.writeln("Hello World!");  // write data to buffer
  display.display();                // draw the buffer!
  while(true){}; // hault here
}
```

### TODO
* Refine command functions to increase framerate
* Add additional fonts
* Move private functions to correct access level
* Enhance this library to be a full graphics driver for the SSD1306 OLED
* Comment functions for intellisense
* Fully document library
