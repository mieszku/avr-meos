/*
 * Copyright (c) 2016 Mieszko Mazurek
 */

#ifndef __GPIO_H
#define __GPIO_H

#include <inttypes.h>

#include "gpio_defs.h"

#ifdef __cplusplus
extern "C" {
#endif

void	gpio_mode		(uint16_t pin, uint8_t mode);
void	gpio_mode_input		(uint16_t pin);
void	gpio_mode_output	(uint16_t pin);
void	gpio_write		(uint16_t pin, uint8_t state);
void	gpio_write_low		(uint16_t pin);
void	gpio_write_high		(uint16_t pin);
void	gpio_toggle		(uint16_t pin);
uint8_t gpio_read		(uint16_t pin);

#ifdef __cplusplus
}
#endif

#endif
