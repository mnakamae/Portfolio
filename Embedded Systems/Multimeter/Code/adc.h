/*
 * adc.h
 *
 *  Created on: May 18, 2020
 *      Author: nicks
 */

#ifndef ADC_H_
#define ADC_H_

#define AVG_LENGTH (4)
#define MAX_READING_MV (3000)

void adc_record();
void adc_report_avg();
inline void adc_curr_val(unsigned int val);
void adc_log_reading();
unsigned int adc_get_avg();
void uart_write_volts(unsigned int val_mv);
void adc_init();
