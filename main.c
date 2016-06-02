/*
 * Copyright (c) 2016 Mieszko Mazurek
 */

#include "drivers/hd44780gpio.h"
#include "drivers/hd44780.h"
#include "drivers/hd44780lcd.h"
#include "drivers/ostream.h"

#include "hardware/gpio.h"

#include "core/thread.h"
#include "core/system.h"

#include <util/delay.h>
#include <stdlib.h>

#define SP *((volatile uint16_t*) 0x3D)

hd44780gpio_t* 	lcditf;
hd44780lcd_t*	lcd;

uint16_t	sp;

uint16_t thread_fork (void* stack);
void __thread_yield (void);

void thread (void* obj)
{
	int i = 0;

	long time = 0;

	while (1) {
		while (system_get_time () < time);
		time += 1000;

		hd44780lcd_set_position (lcd, 1, 0);
		ostream_put_string (OSTREAM (lcd), "time: ");
		ostream_put_uint32 (OSTREAM (lcd), system_get_time ());
		ostream_put_string (OSTREAM (lcd), " ");
		ostream_put_string (OSTREAM (lcd), thread_current->name);
		gpio_toggle (GPIO_PIN13);

		//system_yield ();
	}
}

int main (void)
{
	_delay_ms (500);
	gpio_mode (GPIO_PIN13, GPIO_OUTPUT);
	gpio_mode (GPIO_PIN7, GPIO_OUTPUT);

	lcditf = hd44780gpio_new (GPIO_PIN2, GPIO_PIN3, GPIO_PIN4,
				  0, 0, 0, 0,
				  GPIO_PIN9, GPIO_PIN10, GPIO_PIN11, GPIO_PIN12);
	lcd = hd44780lcd_new ((hd44780itf_t*) lcditf, LCD4X20);
	
	static uint8_t st [101];
	static thread_t thr;

	thread_exec (thread, NULL, "thread", &thr, st, 100);
		
	long time = 0;

	while (1) {
		/*
		hd44780lcd_set_position (lcd, 2, 8);
		ostream_put_uint32 (OSTREAM (lcd), i++);
		ostream_put_char (OSTREAM (lcd), ' ');
		ostream_put_uint32 (OSTREAM (lcd), system_get_time ());
		*/
		while (system_get_time () < time);
		time += 500;
		gpio_toggle (GPIO_PIN7);

		//system_yield ();
	}
	
	return 0;
}
