/*
 * Copyright (c) 2016 Mieszko Mazurek
 */

#include "drivers/hd44780gpio.h"
#include "drivers/hd44780.h"
#include "drivers/hd44780lcd.h"
#include "drivers/ostream.h"

#include "hardware/gpio.h"

#include "core/thread.h"

#include <util/delay.h>

#define SP *((volatile uint16_t*) 0x3D)

hd44780gpio_t 	lcditf;
hd44780lcd_t	lcd;

uint16_t	sp;

uint16_t thread_fork (void* stack);
void _switch (void);

void thread (void)
{
	int i = 0;

	while (1) {
		hd44780lcd_set_position (&lcd, 1, 4);

		ostream_put_string (&lcd.ostream, "time: ");
		ostream_put_uint32 (&lcd.ostream, i++);

		gpio_toggle (GPIO_PIN13);
		_delay_ms (900);
		_switch ();
	}
}

int main (void)
{
	gpio_mode (GPIO_PIN13, GPIO_OUTPUT);
	gpio_mode (GPIO_PIN7, GPIO_OUTPUT);

	hd44780gpio_construct (&lcditf,
			       GPIO_PIN2, GPIO_PIN3, GPIO_PIN4,
			       0, 0, 0, 0,
			       GPIO_PIN9, GPIO_PIN10, GPIO_PIN11, GPIO_PIN12);
	hd44780lcd_construct (&lcd, &lcditf.hd44780itf, LCD4X20);

	uint8_t stack [101];

	if (! thread_fork (stack + 100))
		thread ();

	int i = 0;
		
	while (1) {
		hd44780lcd_set_position (&lcd, 2, 8);
		ostream_put_uint32 (&lcd.ostream, i++);
		gpio_toggle (GPIO_PIN7);
		_delay_ms (900);
		_switch ();
	}
	
	return 0;
}
