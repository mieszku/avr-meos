/*
 * Copyright (c) 2016 Mieszko Mazurek
 */

#include "drivers/hd44780gpio.h"
#include "drivers/hd44780.h"
#include "drivers/hd44780lcd.h"
#include "drivers/ostream.h"

#include "hardware/gpio.h"
#include "hardware/adc.h"

#include "core/thread.h"
#include "core/system.h"
#include "core/task.h"
#include "core/mutex.h"
#include "core/memalloc.h"
#include "core/panic.h"

#include <util/delay.h>
#include <stdlib.h>

hd44780gpio_t* 		lcditf;
hd44780lcd_t*		lcd;
mutex_t			mtx, lcdlock;

volatile uint8_t	lock;

uint16_t thread_fork (void* stack);
void __thread_yield (void);

void toggle13 (void* obj)
{
	gpio_toggle (GPIO_PIN13);
}

void thread (void* obj)
{
	while (1) {
		mutex_lock (&mtx);
		mutex_lock (&lcdlock);

		hd44780lcd_set_position (lcd, 1, 0);
		ostream_put_string (OSTREAM (lcd), thread_current->name);
		ostream_put_string (OSTREAM (lcd), " : ");
		ostream_put_uint32 (OSTREAM (lcd), system_get_time ());

		mutex_unlock (&lcdlock);

		thread_run_alloc (toggle13, NULL, "toggle 13", 30);
	}
}

void showrand (void* obj)
{
	while (1) {
		mutex_lock (&lcdlock);

		uint16_t rand = system_rand ();

		hd44780lcd_set_position (lcd, 3, 0);
		ostream_put_string (OSTREAM (lcd), "rand: ");
		hd44780lcd_set_position (lcd, 3, 7);
		ostream_put_string (OSTREAM (lcd), "  ");
		hd44780lcd_set_position (lcd, 3, 6);
		ostream_put_int (OSTREAM (lcd), rand);

		mutex_unlock (&lcdlock);

		system_sleep (2000);
	}
}

uint8_t flg;

void toggle7 (void* obj)
{
	flg = thread_current->_flags;

	gpio_toggle (GPIO_PIN7);
	if (obj) 
		memfree (obj);
}

uint8_t unlock0 (void* obj)
{
	mutex_unlock (&mtx);
	return 1;
}

void blink (void* obj)
{
	while (1) {
		system_sleep (system_rand ());
	}
}

int main (void)
{
	system_init ();
	system_sleep (500);

	gpio_mode (GPIO_PIN13, GPIO_OUTPUT);
	gpio_mode (GPIO_PIN7, GPIO_OUTPUT);

	lcditf = hd44780gpio_new (GPIO_PIN2, GPIO_PIN3, GPIO_PIN4,
				  0, 0, 0, 0,
				  GPIO_PIN9, GPIO_PIN10, GPIO_PIN11, GPIO_PIN12);
	lcd = hd44780lcd_new ((hd44780itf_t*) lcditf, INTERFACE_MODE_4BIT, LCD4X20);
	
	static uint8_t st [160];
	static thread_t thr;

	thread_exec (thread, NULL, "thread", &thr, st, sizeof (st));
	thread_run_alloc (blink, NULL, "blink", 50);

	thread_run_alloc (showrand, NULL, "rand", 90);
	
	task_register (unlock0, NULL, 2000, 2000);
	
	while (1) {
		uint16_t v = adc_read (ADC_PIN5);
		adc_disable ();

		mutex_lock (&lcdlock);

		hd44780lcd_set_position (lcd, 2, 0);
		ostream_put_string (OSTREAM (lcd), thread_current->name);
		ostream_put_string (OSTREAM (lcd), " : ");
		ostream_put_uint32 (OSTREAM (lcd), system_get_time () * 5);
		ostream_put_string (OSTREAM (lcd), ", ");
		uint8_t x = hd44780lcd_get_x (lcd);
		ostream_put_string (OSTREAM (lcd), "     ");
		hd44780lcd_set_position (lcd, 2, x);
		ostream_put_int (OSTREAM (lcd), (int) memalloc_brk);

		mutex_unlock (&lcdlock);

		void* mem = memalloc (100);
		thread_run_alloc (toggle7, mem, "toggle7", 30);
		system_sleep (500);
	}
	
	return 0;
}

void panic (error_t err)
{
	hd44780lcd_clear (lcd);
	hd44780lcd_set_position (lcd, 0, 0);
	ostream_put_string (OSTREAM (lcd), "panic: ");
	hd44780lcd_set_position (lcd, 1, 0);

	const char* strerr;

	switch (err) {
	case ERROR_STACK_SMASH:
		strerr = "STACK SMASH";
		break;
	case ERROR_PURE_VIRTUAL:
		strerr = "PURE VIRTUAL";
		break;
	case ERROR_XMEMALLOC:
		strerr = "XMEMALLOC";
		break;
	case ERROR_INVALID_ARGUMENT:
		strerr = "INVALID ARGUMENT";
		break;
	default:
		strerr = "UNKNOWN ERROR";
		break;
	}

	ostream_put_string (OSTREAM (lcd), strerr);

	hd44780lcd_set_position (lcd, 2, 0);
	ostream_put_string (OSTREAM (lcd), "thread: ");
	ostream_put_string (OSTREAM (lcd), thread_current->name);
	system_sleep (2000);

	while (1) {
		gpio_toggle (GPIO_PIN13);
		system_sleep (25);
	}
}
