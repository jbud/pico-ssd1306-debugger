#include <stdint.h>
#include <string.h>
#include <stdio.h>
#include <cstdint>
#include <stdlib.h>
#include "pico/stdlib.h"
#include "hardware/i2c.h"
#include "hardware/gpio.h"
#include "hardware/adc.h"
#include "../ssd1306.h"

void testanimate(const uint8_t *bitmap, uint8_t w, uint8_t h);

#define NUMFLAKES     10
#define LOGO_HEIGHT   16
#define LOGO_WIDTH    16
#define XPOS   0 // Indexes into the 'icons' array in function below
#define YPOS   1
#define DELTAY 2
static const unsigned char logo_bmp[] =
{ 0x01, 0x80, 
  0x03, 0xc0, 
  0x01, 0x80, 
  0x25, 0xa4, 
  0x67, 0xe6, 
  0x7d, 0xbe, 
  0x0d, 0xb0, 
  0x1b, 0xd8, 
  0x1b, 0xd8, 
  0x0d, 0xb0, 
  0x7d, 0xbe, 
  0x67, 0xe6, 
  0x25, 0xa4, 
  0x01, 0x80, 
  0x03, 0xc0, 
  0x01, 0x80,  };

u8 random(u8 s, u8 e){
    return rand() % s + e;
}

int main()
{
    stdio_init_all();
    adc_init();
    adc_gpio_init(26);
    adc_select_input(0);
    SSD1306 display(128, 64, 0x3C, false);   // setup the object
    display.init_i2c(i2c0, 4, 5);     // start i2c
    display.init_display();
    char c[50];
    display.setCursor(0,0);
    while(true){
        int8_t f, icons[NUMFLAKES][3];

        // Initialize 'snowflake' positions
        for(f=0; f< NUMFLAKES; f++) {
            icons[f][XPOS]   = random(1 - LOGO_WIDTH, display.get_width());
            icons[f][YPOS]   = random(1-LOGO_HEIGHT, display.get_height());
            icons[f][DELTAY] = random(1, 10);
        }

        for(;;) { // Loop forever...
            display.clearDisplay(); // Clear the display buffer

            // Draw each snowflake:
            for(f=0; f< NUMFLAKES; f++) {
                display.draw_bitmap(icons[f][XPOS], icons[f][YPOS], logo_bmp, LOGO_WIDTH, LOGO_HEIGHT, 1);
            }

            display.display(); // Show the display buffer on the screen
            sleep_ms(50);        
            // Then update coordinates of each flake...
            for(f=0; f< NUMFLAKES; f++) {
            icons[f][YPOS] += icons[f][DELTAY];
            // If snowflake is off the bottom of the screen...
            if (icons[f][YPOS] >= display.get_height()) {
                // Reinitialize to a random position, just off the top
                icons[f][XPOS]   = random(1 - LOGO_WIDTH, display.get_width());
                icons[f][YPOS]   = random(1-LOGO_HEIGHT, display.get_height());
                icons[f][DELTAY] = random(1, 10);
            }
            }
        }
    }
    return 0;
}







void testanimate(const uint8_t *bitmap, uint8_t w, uint8_t h) {
  
}