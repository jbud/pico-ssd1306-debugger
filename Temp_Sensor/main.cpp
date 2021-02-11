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
    adc_set_temp_sensor_enabled(true);
    SSD1306 display(128, 64, 0x3C, true);   // setup the object
    display.init_i2c(i2c0, 4, 5);     // start i2c
    display.init_display();
    char c[50];
    while(true){
        display.setCursor(0,0);
        //float f = 3.3/65535;
        const float conversion_factor = 3.3F / (1<<12);
        
        uint16_t result = adc_read();
        float r = result * conversion_factor;
        float t = 27 - (r - 0.706) / 0.001721; //T = 27 - (ADC_voltage - 0.706)/0.001721
        float f = 9 / 5 * t + 32;
        //float k = t + 273;
        sprintf(c, "TempC: %.2f*F \nVoltage: %.2f V \nRaw: 0x%03x", f,r,result);
        display.print(c);  // write data to buffer
        display.display();
        //sleep_ms(250);
        //display.clearDisplay();
    }
    return 0;
}