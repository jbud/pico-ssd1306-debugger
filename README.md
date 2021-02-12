# pico-ssd1306-debugger
Debugging output for Raspberry PICO using OLED display (SSD1306) and PICO C++ SDK

## example implementation

Examples are now included! check out the [Temp_Sensor](Temp_Sensor/main.cpp) example or others in this repo!

```C++
#include "ssd1306.h"

void main(){
    SSD1306 display(128, 64, 0x3C, false);    // setup the object
    display.init_i2c(i2c0, 4, 5);             // start i2c
    display.init_display();                   // send commands to initialize the SSD1306
    char c[50];
    int i=0;                                  // var to debug
    display.setCursor(0,0);                   // start at top left of screen 0 x 0 pixels
    while(true){
        sprintf(c, "(int) i: %d",i);          // prepare output
        display.writeln(c);                   // write data to buffer
        display.display();
        i++;
        sleep_ms(250);                        // Wait 1/4 second and proceed
}
```

### See it in action
![Temp_Sensor](https://github.com/jbud/pico-ssd1306-debugger/blob/master/images/IMG_0555.png?raw=true)

### TODO
* Refine command functions to increase framerate
* Add additional fonts
* Move private functions to correct access level
* Enhance this library to be a full graphics driver for the SSD1306 OLED
* Comment functions for intellisense
* Fully document library
