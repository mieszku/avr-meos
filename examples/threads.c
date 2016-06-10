/*
 * Copyright (c) 2016 Mieszko Mazurek
 */

#include "core/system.h"
#include "core/thread.h"
#include "core/panic.h"
#include "hardware/gpio.h"

#include <util/delay.h>

void blink13 (void* obj)
{
	// toggle pin 13 every 400ms
	while (1) {
		gpio_toggle (GPIO_PIN13);

		// You can even use _delay_ms but in most cases
		// it don't works properly with threads, 
		// so it's better to use system_sleep instead
		_delay_ms (400);
		//system_sleep (400);
	}
}

void blink7 (void* obj)
{
	// toggle pin 7 every 50ms
	while (1) {
		gpio_toggle (GPIO_PIN7);
		system_sleep (50);
	}
}

int main (void)
{
	// don't forget about initialize system
	system_init ();

	gpio_mode_output (GPIO_PIN13);
	gpio_mode_output (GPIO_PIN7);

	// run two threads on 20 bytes stacks
	thread_run_alloc (blink13, NULL, "blink13", 20);
	thread_run_alloc (blink7, NULL, "blink7", 20);

	// and about prevent main return by thread_kill
	// or simply fall into while (1); 
	thread_kill ();
	//while (1);
}

void panic (error_t err)
{
	while (1) {
		gpio_toggle (GPIO_PIN13);
		system_sleep (25);
	}
}
