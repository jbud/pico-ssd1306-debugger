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
    SSD1306 display(128, 64, 0x3C, true);   // setup the object
    display.init_i2c(i2c0, 4, 5);     // start i2c
    display.init_display();
    char c[50];
    uint8_t dz = 0;
    while(true){
        display.setCursor(0,0);
        sprintf(c, "Char #%d: %c", dz,dz);
        display.print(c);  // write data to buffer
        display.display();
        dz++;
        sleep_ms(250);
        if (dz > 254){
            display.clearDisplay();
            dz = 0;
        }
    }
    return 0;
}