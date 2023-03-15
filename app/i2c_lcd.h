#ifndef I2C_LCD_H
#define I2C_LCD_H

#include "wm_type_def.h"
#include "wm_gpio.h"

#define LCD_HOME 0x02
#define LCD_MODE_4BIT 0x28
#define LCD_DISPLAY_ON 0x0C
#define LCD_DISPLAY_OFF 0x08
#define LCD_CLRSCR 0x01
#define LCD_SETDDRAMADDR 0x80
#define LCD_CG_RAM 0x40
#define LCD_BACKLIGHT 0x08
#define LCD_NOBACKLIGHT 0x00
#define LCD_SCROLL_RIGHT 0x1E
#define LCD_SCROLL_LEFT 0x18

#define EN_BIT 0b00000100

uint8_t _cols;
uint8_t _lines;
uint8_t _backlightval;

void i2c_lcd_init(uint8_t lcd_addr, uint8_t cols, uint8_t lines);
void i2c_lcd_write_char(uint8_t lcd_addr, unsigned char value);
void i2c_lcd_write_string(uint8_t lcd_addr, char *str);
void i2c_lcd_backlight(uint8_t lcd_addr, bool on);
void i2c_lcd_set_cursor(uint8_t lcd_addr, uint8_t col, uint8_t row);
void i2c_lcd_home(uint8_t lcd_addr);
void i2c_lcd_display(uint8_t lcd_addr, bool on);
void i2c_lcd_clear(uint8_t lcd_addr);
void i2c_lcd_create_custom_char(uint8_t lcd_addr, uint8_t location, uint8_t *charmap);
void i2c_lcd_write_custom_char(uint8_t lcd_addr, uint8_t location);
void i2c_lcd_scroll(uint8_t lcd_addr, uint8_t direction, uint8_t scroll_size);

#endif