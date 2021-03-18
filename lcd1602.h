/*
 * lcd1602.h
 *
 *  Created on: Mar 18, 2021
 *      Author: Janmay
 */

#ifndef INC_LCD1602_H_
#define INC_LCD1602_H_

#define lcd1602_hi2c		hi2c2
#define lcd1602_address		0x4E

void lcd_init (void);
void lcd_display_string (char *str);
void lcd_send_cmd (uint8_t cmd);
void lcd_send_data (uint8_t data);
void lcd_set_cursor(uint8_t row, uint8_t col);
void lcd_display_on_off(uint8_t display_on, uint8_t cursor_on, uint8_t cursor_blink_on);
void lcd_backlight_on_off(uint8_t light_on);
void lcd_display_clear(void);
void lcd_display(uint8_t row, uint8_t col, char *str);

#endif /* INC_LCD1602_H_ */
