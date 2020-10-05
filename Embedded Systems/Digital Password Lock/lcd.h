/*
 * lcd.h
 *
 *  Created on: Apr 19, 2020
 *      Author: nicks
 */

#ifndef LCD_H_
#define LCD_H_

#define RS 1     /* P4.0 mask */
#define RW 2     /* P4.1 mask */
#define EN 4     /* P4.2 mask */


void LCD_nibble_write(unsigned char data, unsigned char control);
void clear_LCD();
void home_LCD();
void LCD_command(unsigned char command);
void LCD_init(void);
void write_char_LCD(char data);
void write_string_LCD(char str[]);

#endif /* LCD_H_ */
