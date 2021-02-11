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
    stdio_init_all();
    adc_init();
    adc_gpio_init(26);
    adc_select_input(0);
    SSD1306 display(128, 64, 0x3C);   // setup the object
    display.init_i2c(i2c0, 4, 5);     // start i2c
    display.init_display();
    char c[50];
    display.setCursor(0,0);
    while(true){
        //display.setCursor(0,0);
        float f = 3.3/65535;
        const float conversion_factor = 3.3f / (1 << 12);
        uint16_t result = adc_read();
        sprintf(c, "ADC: %f V", result * conversion_factor);
        display.writeln(c);  // write data to buffer
        display.display();
        sleep_ms(250);
        //display.clearDisplay();
    }
    return 0;
}