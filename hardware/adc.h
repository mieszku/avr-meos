/*
 * Copyright (c) 2016 Mieszko Mazurek
 */

#ifndef __ADC_H
#define __ADC_H

#include <inttypes.h>

#define ADC_PIN0	0
#define ADC_PIN1	1
#define ADC_PIN2	2
#define ADC_PIN3	3
#define ADC_PIN4	4
#define ADC_PIN5	5

#ifdef __cplusplus
extern "C" {
#endif

uint8_t		adc_read	(uint8_t	channel);
uint16_t	adc_read_hres	(uint8_t	channel);
void		adc_disable	(void);

#ifdef __cplusplus
}
#endif

#endif
