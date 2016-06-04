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
#include "core/task.h"
#include "core/mutex.h"

#include <util/delay.h>
#include <stdlib.h>

hd44780gpio_t* 		lcditf;
hd44780lcd_t*		lcd;
mutex_t			mtx, lcdlock;

volatile uint8_t	lock;

uint16_t thread_fork (void* stack);
void __thread_yield (void);

void thread (void* obj)
{
	while (1) {
		//while (lock);
		//lock = 1;
		mutex_lock (&mtx);
		mutex_lock (&lcdlock);
		//while (system_get_time () < time);
		//time += 1000;

		hd44780lcd_set_position (lcd, 1, 0);
		ostream_put_string (OSTREAM (lcd), thread_current->name);
		ostream_put_string (OSTREAM (lcd), " : ");
		ostream_put_uint32 (OSTREAM (lcd), system_get_time ());

		mutex_unlock (&lcdlock);

		gpio_toggle (GPIO_PIN13);
	}
}

void showrand (void* obj)
{
	while (1) {
		mutex_lock (&lcdlock);

		uint16_t rand = system_rand ();

		hd44780lcd_set_position (lcd, 3, 0);
		ostream_put_string (OSTREAM (lcd), "            ");
		hd44780lcd_set_position (lcd, 3, 0);
		ostream_put_string (OSTREAM (lcd), "rand: ");
		ostream_put_int (OSTREAM (lcd), rand);

		mutex_unlock (&lcdlock);

		system_sleep (1000 + rand);
	}
}

uint8_t unlock0 (void* obj)
{
	mutex_unlock (&mtx);
	return 1;
}

void blink (void* obj)
{
	while (1) {
		system_sleep (397);
		gpio_toggle (GPIO_PIN7);
	}
}

int main (void)
{
	system_sleep (1000);

	gpio_mode (GPIO_PIN13, GPIO_OUTPUT);
	gpio_mode (GPIO_PIN7, GPIO_OUTPUT);

	lcditf = hd44780gpio_new (GPIO_PIN2, GPIO_PIN3, GPIO_PIN4,
				  0, 0, 0, 0,
				  GPIO_PIN9, GPIO_PIN10, GPIO_PIN11, GPIO_PIN12);
	lcd = hd44780lcd_new ((hd44780itf_t*) lcditf, LCD4X20);
	
	static uint8_t st [101];
	static thread_t thr;

	thread_exec (thread, NULL, "thread", &thr, st, 100);
	thread_run_alloc (blink, NULL, "blink", 40);
	thread_run_alloc (blink, NULL, "blink", 40);
	thread_run_alloc (blink, NULL, "blink", 40);
	thread_run_alloc (blink, NULL, "blink", 40);
	thread_run_alloc (blink, NULL, "blink", 40);

	thread_run_alloc (showrand, NULL, "rand", 90);
	
	task_register (unlock0, NULL, 5000, 500);
		
	while (1) {
		mutex_lock (&lcdlock);

		hd44780lcd_set_position (lcd, 2, 0);
		ostream_put_string (OSTREAM (lcd), thread_current->name);
		ostream_put_string (OSTREAM (lcd), " : ");
		ostream_put_uint32 (OSTREAM (lcd), system_get_time () * 5);
	
		mutex_unlock (&lcdlock);

		system_sleep (1500);
		gpio_toggle (GPIO_PIN7);

		system_yield ();

		if (system_get_time () > 110000)
			break;
	}
	
	return 0;
}
