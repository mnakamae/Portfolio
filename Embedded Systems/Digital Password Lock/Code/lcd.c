/*
 * lcd.c
 *
 *  Created on: Apr 19, 2020
 *      Author: nicks
 */

#include "msp.h"
#include "lcd.h"
#include "delay.h"

void LCD_init(void)
{
    P4->DIR = 0xFF;     /* make P4 pins output for data and controls */
    delay_us(30000,FREQ_3MHz);                /* initialization sequence */
    LCD_nibble_write(0x30, 0);
    delay_us(10000,FREQ_3MHz);
    LCD_nibble_write(0x30, 0);
    delay_us(1000,FREQ_3MHz);
    LCD_nibble_write(0x30, 0);
    delay_us(1000,FREQ_3MHz);
    LCD_nibble_write(0x20, 0);  /* use 4-bit data mode */
    delay_us(1000,FREQ_3MHz);

    LCD_command(0x28);      /* set 4-bit data, 2-line, 5x7 font */
    LCD_command(0x06);      /* move cursor right after each char */
    LCD_command(0x01);      /* clear screen, move cursor to home */
    LCD_command(0x0F);      /* turn on display, cursor blinking */
}

/* With 4-bit mode, each command or data is sent twice with upper nibble first then lower nibble.*/
void LCD_nibble_write(unsigned char data, unsigned char control)
{
    data &= 0xF0;           /* clear lower nibble for control */
    control &= 0x0F;        /* clear upper nibble for data */
    P4->OUT = data | control;       /* RS = 0, R/W = 0 */
    P4->OUT = data | control | EN;  /* pulse E */
    delay_us(1000,FREQ_3MHz);
    P4->OUT = data;                 /* clear E */
    P4->OUT = 0;
}

void LCD_command(unsigned char command)
{
    LCD_nibble_write(command & 0xF0, 0);    /* upper nibble first */
    LCD_nibble_write(command << 4, 0);      /* then lower nibble */

    if (command < 4)
        delay_us(4000,FREQ_3MHz);         /* commands 1 and 2 need up to 1.64ms */
    else
        delay_us(1000,FREQ_3MHz);         /* all others 40 us */
}

void clear_LCD()
{
    LCD_command(0x01);
}
void home_LCD()
{
    LCD_command(0x80);
}

void write_char_LCD(char data)
{
    LCD_nibble_write(data & 0xF0, RS);    /* upper nibble first */
    LCD_nibble_write(data << 4, RS);      /* then lower nibble  */

    delay_us(1000,FREQ_3MHz);
}

void write_string_LCD(char str[])
{
    int i=0;
    while(str[i])
    {
        write_char_LCD(str[i]);
        i++;
    }
}
