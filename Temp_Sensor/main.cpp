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
    //adc_gpio_init(26);
    adc_select_input(4);
    SSD1306 display(128, 64, 0x3C);   // setup the object
    display.init_i2c(i2c0, 4, 5);     // start i2c
    display.init_display();
    char c[50];
    display.setCursor(0,0);
    while(true){
        display.setCursor(0,0);
        //float f = 3.3/65535;
        const float conversion_factor = 3.3F / (1 << 12);
        
        uint16_t result = adc_read();
        float r = result * conversion_factor;
        float t = 27 - (r - 0.706) / 0.001721; //T = 27 - (ADC_voltage - 0.706)/0.001721
        sprintf(c, "Temp: %fC \nVoltage: %fV\nRaw: 0x%03x", t,r,result);
        display.print(c);  // write data to buffer
        display.display();
        //sleep_ms(250);
        //display.clearDisplay();
    }
    return 0;
}