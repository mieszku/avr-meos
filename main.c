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
#include <stdlib.h>

#define SP *((volatile uint16_t*) 0x3D)

hd44780gpio_t* 	lcditf;
hd44780lcd_t*	lcd;

uint16_t	sp;

uint16_t thread_fork (void* stack);
void _switch (void);

void foo (void)
{
	while (1) _switch  ();
}

void thread (void* obj)
{
	while (1) _switch  ();
	/*
	int i = 0;

	while (1) {
		hd44780lcd_set_position (lcd, 1, 4);
		ostream_put_string (OSTREAM (lcd), "time: ");
		ostream_put_uint32 (OSTREAM (lcd), i++);
		gpio_toggle (GPIO_PIN13);

		_delay_ms (900);
		_switch ();
	}
	*/
}

int main (void)
{
	_delay_ms (2000);
	gpio_mode (GPIO_PIN13, GPIO_OUTPUT);
	gpio_mode (GPIO_PIN7, GPIO_OUTPUT);

	lcditf = hd44780gpio_new (GPIO_PIN2, GPIO_PIN3, GPIO_PIN4,
				  0, 0, 0, 0,
				  GPIO_PIN9, GPIO_PIN10, GPIO_PIN11, GPIO_PIN12);
	lcd = hd44780lcd_new ((hd44780itf_t*) lcditf, LCD4X20);
	
	static uint8_t st [101];
	static thread_t thr;

	thread_exec (thread, NULL, "thread", &thr, st, 100);
		
	int i = 0;

	while (1) {
		//hd44780lcd_set_position (lcd, 2, 8);
		//ostream_put_uint32 (OSTREAM (lcd), i++);
		gpio_toggle (GPIO_PIN7);

		_delay_ms (500);
		_switch ();
	}
	
	return 0;
}
