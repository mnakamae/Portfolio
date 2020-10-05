/*
 * keypad.h
 *
 *  Created on: Apr 26, 2020
 *      Author: nakamae
 */

#ifndef KEYPAD_H_
#define KEYPAD_H_

#include <stdint.h>

#define COL_0 BIT0
#define COL_1 BIT1
#define COL_2 BIT2
#define ROW_0 BIT3
#define ROW_1 BIT4
#define ROW_2 BIT5
#define ROW_3 BIT6


void keypad_init(void);
uint8_t keypad_Getkey(void);

#endif /* KEYPAD_H_ */
