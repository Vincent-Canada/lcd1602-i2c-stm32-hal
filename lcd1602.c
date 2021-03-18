/*
 * lcd1602.c
 *
 *  Created on: Mar 18, 2021
 *      Author: Janmay
 */
#include "stm32f4xx_hal.h"
#include "lcd1602.h"
#include "i2c.h"

/*This function is used to send commands to lcd1602 through PCF8574
 * lcd1602 is using 4 bit connection mode, and one byte needs to be sent two times. Upper 4 bits first.
 * EN=1 to send data to lcd1602.
 */
void lcd_send_cmd (uint8_t cmd)
{
	uint8_t data_u, data_l;
	uint8_t data_t[4];
	data_u = (cmd&0xf0);
	data_l = ((cmd<<4)&0xf0);
	data_t[0] = data_u|0x0C;  		//EN=1, RS=0
	data_t[1] = data_u|0x08;  		//EN=0, RS=0
	data_t[2] = data_l|0x0C;  		//EN=1, RS=0
	data_t[3] = data_l|0x08;  		//EN=0, RS=0
	HAL_I2C_Master_Transmit (&lcd1602_hi2c, lcd1602_address,(uint8_t *) data_t, 4, 2);
}

/*This function is used to send data to lcd1602 through PCF8574
 * lcd1602 is using 4 bit connection mode, and one byte needs to be sent two times. Upper 4 bits first.
 * EN=1 to send data to lcd1602.
 */
void lcd_send_data (uint8_t data)
{
	uint8_t data_u;
	uint8_t data_l;
	uint8_t data_t[4];
	data_u = (data&0xf0);
	data_l = ((data<<4)&0xf0);
	data_t[0] = data_u|0x0D;  //en=1, rs=1
	data_t[1] = data_u|0x09;  //en=0, rs=1
	data_t[2] = data_l|0x0D;  //en=1, rs=1
	data_t[3] = data_l|0x09;  //en=0, rs=1
	HAL_I2C_Master_Transmit (&lcd1602_hi2c, lcd1602_address,(uint8_t *) data_t, 4, 2);
}


/*This function is used to display strings in lcd1602.
 * needs to set cursor befroe using this function.
 */
void lcd_display_string (char *str)
{
	while (*str) lcd_send_data (*str++);
}

/*This function is used to display strings in lcd1602.
 * needs to provide cursor position using this function.
 */
void lcd_display(uint8_t row, uint8_t col, char *str)
{
	lcd_set_cursor(row, col);
	while (*str) lcd_send_data (*str++);
}


/*This function is used to do the initialization.
 * The sequence is described in lcd1602 data sheet
 */
void lcd_init (void)
{
	// 4 bit initialization: needs to set to 8 bit first and change it to 4 bit after.
	HAL_Delay(50);  // wait for >40ms
	lcd_send_cmd (0x30);
	HAL_Delay(5);  // wait for >4.1ms
	lcd_send_cmd (0x30);
	HAL_Delay(1);  // wait for >100us
	lcd_send_cmd (0x30);
	HAL_Delay(10);
	lcd_send_cmd (0x20);  // 4bit mode
	HAL_Delay(10);

  // screen display initialization
	lcd_send_cmd (0x28); 	// Function set --> DL=0 (4 bit mode), N = 1 (2 line display) F = 0 (5x8 characters)
	HAL_Delay(1);
	lcd_send_cmd (0x08); 	//Display on/off control --> D=0,C=0, B=0  ---> display off
	HAL_Delay(1);
	lcd_send_cmd (0x01);  	// clear display
	HAL_Delay(1);
	HAL_Delay(1);
	lcd_send_cmd (0x06); 	//Entry mode set --> I/D = 1 (increment cursor) & S = 0 (no shift)
	HAL_Delay(1);
	lcd_send_cmd (0x0C); 	//Display on/off control --> D = 1, C and B = 0. (Cursor and blink, last two bits)
}

/*This function is used to set cursor position.
 * row: 0, 1; col: 0 - 15
 */
void lcd_set_cursor(uint8_t row, uint8_t col)
{
	uint8_t address;
	switch (row)
	{
	case 0:
		address=0x00+col;
		break;
	case 1:
		address=0x40+col;
		break;
	default:
		break;
	}
	address=address|0x80;
	lcd_send_cmd(address);
}

/*This function is used to turn display on and off.
 * can yurn on display, cursor and cursor blink on and off.
 */
void lcd_display_on_off(uint8_t display_on, uint8_t cursor_on, uint8_t cursor_blink_on)
{
	uint8_t data_t=0;
	data_t=data_t|(display_on<<2)|(cursor_on<<1)|(cursor_blink_on<<0);
	data_t=data_t|0x08;
	lcd_send_cmd(data_t);
}

/*This function is used to turn back-light on/off.
 */
void lcd_backlight_on_off(uint8_t light_on)
{
	uint8_t data_t=0;
	data_t=data_t|(light_on<<3);
	HAL_I2C_Master_Transmit (&lcd1602_hi2c, lcd1602_address,(uint8_t *) &data_t, 1, 2);
}

/*This function is used to clear display.
 */
void lcd_display_clear(void)
{
	uint8_t data_t;
	data_t=0x01;
	lcd_send_cmd(data_t);
}
