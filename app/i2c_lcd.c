#include <stdio.h>
#include <stdlib.h>
#include "i2c_lcd.h"
#include "wm_osal.h"

// --------------------- Low level commands ---------------------

void low_write_byte(uint8_t lcd_addr, uint8_t data)
{
    // printf("================== Write Byte ==================\n");
    // printf("data = 0x%.2x \n", data | _backlightval);
    tls_i2c_write_byte(data | _backlightval, 0); // XXXX-BXXX
}

void write4bits(uint8_t lcd_addr, uint8_t data)
{
    // printf("================== Write 4Bits ==================\n");
    // printf("Part 1\n");
    low_write_byte(lcd_addr, data | EN_BIT); // XXXX-X1XX
    tls_os_time_delay(HZ * 0.00001);

    // printf("Part 2\n");
    low_write_byte(lcd_addr, data); // XXXX-XXXX
    tls_os_time_delay(HZ * 0.00050);
}

void i2c_lcd_send(uint8_t lcd_addr, uint8_t data, uint8_t mode)
{
    uint8_t highnib = data & 0xf0;
    uint8_t lownib = (data << 4) & 0xf0;
    tls_i2c_write_byte(lcd_addr << 1, 1);
    // printf("================== Send ==================\n");
    // printf("data = 0x%.2x \n", data);
    // printf("data high = 0x%.2x \n", highnib | mode);
    // printf("data low = 0x%.2x \n", lownib | mode);

    write4bits(lcd_addr, (highnib) | mode); // XXXX-XXXM
    write4bits(lcd_addr, (lownib) | mode);  // XXXX-XXXM

    tls_i2c_stop();
    tls_os_time_delay(HZ * 0.00005);
}

// --------------------- Mid level commands ---------------------

void i2c_lcd_command(uint8_t lcd_addr, uint8_t value)
{
    i2c_lcd_send(lcd_addr, value, 0);
}

void i2c_lcd_write(uint8_t lcd_addr, unsigned char value)
{
    i2c_lcd_send(lcd_addr, value, 1);
}

// --------------------- User commands ---------------------

void i2c_lcd_write_char(uint8_t lcd_addr, unsigned char value)
{
    i2c_lcd_write(lcd_addr, value);
}

void i2c_lcd_write_string(uint8_t lcd_addr, char *str)
{
    while (*str)
        i2c_lcd_write(lcd_addr, *str++);
}

void i2c_lcd_backlight(uint8_t lcd_addr, bool on)
{
    _backlightval = on ? LCD_BACKLIGHT : LCD_NOBACKLIGHT;
    i2c_lcd_command(lcd_addr, 0);
}

void i2c_lcd_set_cursor(uint8_t lcd_addr, uint8_t col, uint8_t row)
{
    int row_offset[] = {0x00, 0x40, 0x14, 0x54};
    if (row > _lines)
    {
        row = _lines - 1;
    }
    i2c_lcd_command(lcd_addr, LCD_SETDDRAMADDR | (col + row_offset[row]));
}

void i2c_lcd_home(uint8_t lcd_addr)
{
    i2c_lcd_command(lcd_addr, LCD_HOME);
}

void i2c_lcd_display(uint8_t lcd_addr, bool on)
{
    i2c_lcd_command(lcd_addr, on ? LCD_DISPLAY_ON : LCD_DISPLAY_OFF);
}

void i2c_lcd_clear(uint8_t lcd_addr)
{
    i2c_lcd_command(lcd_addr, LCD_CLRSCR);
    tls_os_time_delay(HZ * 0.0005);
}

// Location from 0 to 7 only 8 slots
void i2c_lcd_create_custom_char(uint8_t lcd_addr, uint8_t location, uint8_t *charmap)
{
    if (location >= 8)
    {
        return;
    }
    i2c_lcd_command(lcd_addr, LCD_CG_RAM | (location << 3));
    for (uint8_t i = 0; i < 8; i++)
    {
        i2c_lcd_write(lcd_addr, charmap[i]);
    }
}

void i2c_lcd_write_custom_char(uint8_t lcd_addr, uint8_t location)
{
    if (location >= 8)
    {
        return;
    }
    i2c_lcd_write(lcd_addr, (uint8_t)location);
}

void i2c_lcd_init(uint8_t lcd_addr, uint8_t cols, uint8_t lines)
{
    _cols = cols;
    _lines = lines;

    tls_os_time_delay(HZ * 0.050);

    i2c_lcd_command(lcd_addr, LCD_HOME);
    tls_os_time_delay(HZ * 0.005);
    i2c_lcd_command(lcd_addr, LCD_HOME);
    tls_os_time_delay(HZ * 0.005);
    i2c_lcd_command(lcd_addr, LCD_HOME);
    tls_os_time_delay(HZ * 0.005);
    i2c_lcd_command(lcd_addr, LCD_MODE_4BIT);
    i2c_lcd_command(lcd_addr, LCD_DISPLAY_ON);

    i2c_lcd_command(lcd_addr, 0x0C);
    i2c_lcd_command(lcd_addr, 0x06);

    i2c_lcd_command(lcd_addr, LCD_CLRSCR);
    tls_os_time_delay(HZ * 0.005);
}