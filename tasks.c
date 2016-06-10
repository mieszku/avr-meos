/*
 * Copyright (c) 2016 Mieszko Mazurek
 */

#include "core/system.h"
#include "core/task.h"
#include "core/panic.h"
#include "hardware/gpio.h"

uint8_t toggle7 (void* obj)
{
	gpio_toggle (GPIO_PIN7);

	// don't repeat that task
	return 0;
}

uint8_t toggle13 (void* obj)
{
	gpio_toggle (GPIO_PIN13);

	// register new task with 50ms delay
	task_register (toggle7, NULL, 50, 0);

	// repeat that task every 100ms
	return 1;
}

int main (void)
{
	// don't forget about initialize system
	system_init ();

	gpio_mode_output (GPIO_PIN13);
	gpio_mode_output (GPIO_PIN7);

	// register task with 2000ms delay and 100ms period
	task_register (toggle13, NULL, 2000, 100);

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
