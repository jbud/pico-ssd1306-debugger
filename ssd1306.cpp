/*
  SSD1306.cpp - Library for SSD1306 simple debug output on Raspberry Pico C++.
  Created by Joe Jackson, February 10, 2021.
  Released into the public domain.
*/
#include "SSD1306.h"

SSD1306::SSD1306()
{
    _width = 128;
    _height = 32;
    _pages = _height / 8;
    _i2cAddr = 0x3C;
    _external_vcc = false;
    scr[pages*width+1];
}

SSD1306::SSD1306(uint8_t w, uint8_t h, uint8_t addr)
{
    _width = w;
    _height = h;
    _pages = _height / 8;
    _i2cAddr = addr;
    _external_vcc = false;
    scr[pages*width+1];
}

SSD1306::begin()
{
    
}



SSD1306::writeln(char* v)
{
    ssd1306_println(v);
    display();
    _i++;
    if (_i >= 8) 
    {
        _i = 0;
        //delay(500);
        clearDisplay();
        setCursor(0, 0);
    }
}

SSD1306::print(char* v)
{
    clearDisplay();
    setCursor(0, 0);
    ssd1306_print(v);
    display();
}

SSD1306::init_i2c(uint8_t port, uint8_t sda_pin = 4, uint8_t scl_pin = 5)
{
	_port = port;
    _sda_pin = sda_pin;
    _scl_pin = scl_pin;
	i2c_init(_port, 100 * 1000);
	gpio_set_function(sda_pin, GPIO_FUNC_I2C);
	gpio_set_function(scl_pin, GPIO_FUNC_I2C);
	gpio_pull_up(sda_pin);
	gpio_pull_up(scl_pin);
}

SSD1306::init_display()
{
	u8 cmds[] = {
		SET_DISP | 0x00,  // display off 0x0E | 0x00

		SET_MEM_ADDR, // 0x20
		0x00,  // horizontal

		//# resolution and layout
		SET_DISP_START_LINE | 0x00, // 0x40
		SET_SEG_REMAP | 0x01,  //# column addr 127 mapped to SEG0

		SET_MUX_RATIO, // 0xA8
		height - 1,

		SET_COM_OUT_DIR | 0x08,  //# scan from COM[N] to COM0  (0xC0 | val)
		SET_DISP_OFFSET, // 0xD3
		0x00,

		//SET_COM_PIN_CFG, // 0xDA
		//0x02 if self.width > 2 * self.height else 0x12,
		//width > 2*height ? 0x02 : 0x12,
		//SET_COM_PIN_CFG, height == 32 ? 0x02 : 0x12,

		//# timing and driving scheme
		SET_DISP_CLK_DIV, // 0xD5
		0x80,

		SET_PRECHARGE, // 0xD9
		//0x22 if self.external_vcc else 0xF1,
		external_vcc ? 0x22 : 0xF1,

		SET_VCOM_DESEL, // 0xDB
		//0x30,  //# 0.83*Vcc
		0x40, // changed by mcarter

		//# display
		SET_CONTRAST, // 0x81
		0xFF,  //# maximum

		SET_ENTIRE_ON,  //# output follows RAM contents // 0xA4
		SET_NORM_INV,  //# not inverted 0xA6

		SET_CHARGE_PUMP, // 0x8D
		//0x10 if self.external_vcc else 0x14,
		external_vcc ? 0x10 : 0x14,

		SET_DISP | 0x01
	};

	// write all the commands
	for(int i=0; i<sizeof(cmds); i++)
		write_cmd(cmds[i]);
	fill_scr(0);
	show_scr();
}

SSD1306::write_cmd(u8 cmd) 
{ 
	send(0x80, cmd);
}

SSD1306::send(u8 v1, u8 v2)
{
	u8 buf[2];
	buf[0] = v1;
	buf[1] = v2;
	i2c_write_blocking(_port, _i2cAddr, buf, 2, false);
}

SSD1306::clearDisplay()
{
    fill_scr(0);
    show_scr();
}

SSD1306::display()
{
    show_scr();
}

SSD1306::fill_scr(u8 v)
{
	memset(_scr, v, sizeof(_scr));
}

SSD1306::show_scr()
{

	write_cmd(SET_MEM_ADDR); // 0x20
	write_cmd(0b01); // vertical addressing mode

	write_cmd(SET_COL_ADDR); // 0x21
	write_cmd(0);
	write_cmd(127);

	write_cmd(SET_PAGE_ADDR); // 0x22
	write_cmd(0);
	write_cmd(pages-1);


	_scr[0] = 0x40; // the data instruction	
	i2c_write_blocking(_port, _i2cAddr, _scr, sizeof(_scr), false);
}


SSD1306::poweroff() 
{
    write_cmd(SET_DISP | 0x00);
}

SSD1306::poweron() 
{
    write_cmd(SET_DISP | 0x01); 
}

SSD1306::contrast(u8 contrast) 
{
    write_cmd(SET_CONTRAST); write_cmd(contrast);
}

SSD1306::invert(u8 invert) 
{
    write_cmd(SET_NORM_INV | (invert & 1));
}

SSD1306::draw_pixel(int16_t x, int16_t y, int color) 
{
	if(x<0 || x >= width || y<0 || y>= height) return;

	int page = y/8;
	int bit = 1<<(y % 8);
	u8* ptr = _scr + x*8 + page  + 1; 

	switch (color) {
		case 1: // white
			*ptr |= bit;
			break;
		case 0: // black
			*ptr &= ~bit;
			break;
		case -1: //inverse
			*ptr ^= bit;
			break;
	}

}

SSD1306::draw_bitmap(int16_t x, int16_t y, const uint8_t bitmap[], int16_t w,
                              int16_t h, uint16_t color) {

  int16_t byteWidth = (w + 7) / 8; // Bitmap scanline pad = whole byte
  uint8_t byte = 0;

  for (int16_t j = 0; j < h; j++, y++) {
    for (int16_t i = 0; i < w; i++) {
      if (i & 7)
        byte <<= 1;
      else
        byte = bitmap[j * byteWidth + i / 8];
      if (byte & 0x80)
        draw_pixel(x + i, y, color);
    }
  }
}

SSD1306::draw_letter_at(u8 x, u8 y, char c)
{
	if(c< ' ' || c>  0x7F) c = '?'; // 0x7F is the DEL key

	int offset = 4 + (c - ' ' )*6;
	for(int col = 0 ; col < 6; col++) {
		u8 line =  ssd1306_font6x8[offset+col];
		for(int row =0; row <8; row++) {
			draw_pixel(x+col, y+row, line & 1);
			line >>= 1;
		}
	}

	for(int row = 0; row<8; row++) {
		draw_pixel(x+6, y+row, 0);
		draw_pixel(x+7, y+row, 0);
	}

}

SSD1306::setCursor(u8 x, u8 y){
    _cursorx = x;
    _cursory = y;
}

SSD1306::draw_letter(char c) 
{
    draw_letter_at(_cursorx, _cursory, c);
}

SSD1306::pixel(int x, int y)
{
	int page = y/8;
	u8 patt = 1<<(y%8);
	_scr[1+ x*8 + page] |= patt;

}

SSD1306::ssd1306_println(const char* str)
{
	char c;
	while(c = *str) {
		str++;
		if(c == '\n') {
			_cursorx = 0;
			_cursory += 8;
			continue;
		}
		draw_letter_at(cursorx, cursory, c);
		cursorx += 8;
	}
    _cursory += 8;
}

SSD1306::ssd1306_print(const char* str)
{
	char c;
	while(c = *str) {
		str++;
		if(c == '\n') {
			cursorx = 0;
			cursory += 8;
			continue;
		}
		draw_letter_at(cursorx, cursory, c);
		cursorx += 8;
	}
}