#include <stdint.h>
#include <string.h>
#include <stdio.h>
#include <cstdint>
#include "pico/stdlib.h"
#include "hardware/i2c.h"
#include "hardware/gpio.h"
#include "hardware/adc.h"
#include "../ssd1306.h"





int main()
{
    SSD1306 display(128, 64, 0x3C);   // setup the object
    display.init_i2c(i2c0, 4, 5);     // start i2c
    display.init_display();
    char c[50];
    int i=0;
    display.setCursor(0,0);
    while(true){
        sprintf(c, "Hello World! %d",i);
        display.writeln(c);  // write data to buffer
        display.display();
        i++;
        sleep_ms(250);
        //display.clearDisplay();
        //sleep_ms(25);
    }
    return 0;
}