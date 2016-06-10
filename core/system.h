/*
 * Copyright (c) 2016 Mieszko Mazurek
 */

#ifndef __SYSTEM_H
#define __SYSTEM_H

#include <inttypes.h>

#include "thread.h"

#ifndef NULL
	#define NULL	((void*) 0)
#endif

extern const thread_t system_thread;

void		system_init		(void) __attribute__ ((naked));

uint8_t		rand_seed		(void);

uint32_t	system_get_time		(void);
void		system_sleep		(uint16_t	delay);
void		system_yield		(void);
void		system_enter_critical	(void);
void		system_exit_critical	(void);
void		system_sei		(void);
uint8_t		system_rand		(void);

#endif
