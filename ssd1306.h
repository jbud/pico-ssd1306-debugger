/*
  SSD1306.h - Library for SSD1306 simple debug output on Raspberry Pico C++.
  Created by Joe Jackson, February 10, 2021.
  Released into the public domain.
*/
#ifndef LIB_SSD1306_h
#define LIB_SSD1306_h

#include <stdint.h>
#include <string.h>
#include <cstdint>
#include "pico/stdlib.h"
#include "hardware/i2c.h"
#include "font.h"

#define SET_CONTRAST 0x81
#define SET_ENTIRE_ON 0xA4
#define SET_NORM_INV 0xA6
#define SET_DISP 0xAE
#define SET_MEM_ADDR 0x20
#define SET_COL_ADDR 0x21
#define SET_PAGE_ADDR 0x22
#define SET_DISP_START_LINE 0x40
#define SET_SEG_REMAP 0xA0
#define SET_MUX_RATIO 0xA8
#define SET_COM_OUT_DIR 0xC0
#define SET_DISP_OFFSET 0xD3
#define SET_COM_PIN_CFG 0xDA
#define SET_DISP_CLK_DIV 0xD5
#define SET_PRECHARGE 0xD9
#define SET_VCOM_DESEL 0xDB
#define SET_CHARGE_PUMP 0x8D

typedef uint8_t u8;

class SSD1306
{
public:
    SSD1306();
    SSD1306(uint8_t w, uint8_t h, uint8_t addr);
    void writeln(char* v);
    void print(char* v);
    void begin();
    void init_i2c(uint8_t port, uint8_t sda_pin, uint8_t scl_pin);
    void send(u8 v1, u8 v2);
    void init_display();
    void write_cmd(u8 cmd);
    void send(u8 v1, u8 v2);
    void fill_scr(u8 v);
    void show_scr();
    void poweroff();
    void poweron();
    void contrast(u8 contrast);
    void invert(u8 invert);
    void draw_pixel(int16_t x, int16_t y, int color);
    void draw_bitmap(int16_t x, int16_t y, const uint8_t bitmap[], int16_t w,
                              int16_t h, uint16_t color);
    void draw_letter_at(u8 x, u8 y, char c);
    void draw_letter(char c);
    void pixel(int x, int y);
    void draw_pixel(int16_t x, int16_t y, int color);
    void ssd1306_println(const char* str);
    void display();
    void clearDisplay();
private:
    int _width;
    int _height;
    int _pages;
    int _cursorx;
    int _cursory;
    uint8_t _port;
    uint8_t _sda_pin;
    uint8_t _scl_pin;
    int _i2cAddr;
    int _i;
    uint8_t _scr[];
    bool _external_vcc;
    SSD1306 _display;
};

#endif