#include <stdio.h>
#include <stdlib.h>
#include "wm_i2c.h"
#include "wm_gpio_afsel.h"
#include "wm_type_def.h"

#include "i2c_lcd.h"

#define I2C_LCD_TASK_SIZE 2048
static OS_STK I2cLcdTaskStk[I2C_LCD_TASK_SIZE];
#define I2C_LCD_TASK_PRIO 32

#define PIN_SCL WM_IO_PA_01
#define PIN_SDA WM_IO_PA_04

#define I2C_FREQ (100000)

#define I2C_LCD_ADDR 0x27

void i2c_lcd_task(void *data)
{
	printf("I2C LCD Task Started\n");

	wm_i2c_scl_config(PIN_SCL);
	wm_i2c_sda_config(PIN_SDA);
	tls_i2c_init(I2C_FREQ);

	i2c_lcd_init(I2C_LCD_ADDR, 16, 2);
	i2c_lcd_backlight(I2C_LCD_ADDR, true);

	i2c_lcd_write(I2C_LCD_ADDR, 'H');
	i2c_lcd_write(I2C_LCD_ADDR, 'e');
	i2c_lcd_write(I2C_LCD_ADDR, 'l');
	i2c_lcd_write(I2C_LCD_ADDR, 'l');
	i2c_lcd_write(I2C_LCD_ADDR, 'o');
	i2c_lcd_set_cursor(I2C_LCD_ADDR, 1, 1);
	char testString[] = "World";
	i2c_lcd_write_string(I2C_LCD_ADDR, testString);

	for (;;)
	{
		tls_os_time_delay(HZ * 2); // One second delay
		i2c_lcd_clear(I2C_LCD_ADDR);
		uint8_t customChar[8] = {
			0b00100,
			0b01010,
			0b00100,
			0b11111,
			0b00100,
			0b00100,
			0b01010,
			0b10001};

		i2c_lcd_create_custom_char(I2C_LCD_ADDR, 0, customChar);
		i2c_lcd_set_cursor(I2C_LCD_ADDR, 0, 0);
		
		i2c_lcd_write_custom_char(I2C_LCD_ADDR, 0);
	}
}

void UserMain(void)
{
	tls_os_task_create(NULL, NULL, i2c_lcd_task, NULL, (void *)I2cLcdTaskStk, I2C_LCD_TASK_SIZE * sizeof(u32), I2C_LCD_TASK_PRIO, 0);
}
