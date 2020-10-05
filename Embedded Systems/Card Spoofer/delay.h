/*
 * delay.h
 *
 *  Created on: Apr 19, 2020
 *      Author: nakamae
 */

#ifndef DELAY_H_
#define DELAY_H_

#define FREQ_1_5MHz CS_CTL0_DCORSEL_0
#define FREQ_3MHz CS_CTL0_DCORSEL_1
#define FREQ_6MHz CS_CTL0_DCORSEL_2
#define FREQ_12MHz CS_CTL0_DCORSEL_3
#define FREQ_24MHz CS_CTL0_DCORSEL_4
#define FREQ_48MHz CS_CTL0_DCORSEL_5

void delay_us(int us);
void set_DCO(int val);

#endif /* DELAY_H_ */
