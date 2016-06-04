/*
 * Copyright (c) 2016 Mieszko Mazurek
 */

#ifndef __SYSTEM_H
#define __SYSTEM_H

extern uint8_t	rand_seed		(void);

uint32_t	system_get_time		(void);
void		system_sleep		(uint16_t	delay);
void		system_yield		(void);
void		system_enter_critical	(void) __attribute__ ((naked));
void		system_exit_critical	(void) __attribute__ ((naked));
void		system_sei		(void) __attribute__ ((naked));
uint8_t		system_rand		(void);

#endif
