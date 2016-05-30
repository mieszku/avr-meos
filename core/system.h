/*
 * Copyright (c) 2016 Mieszko Mazurek
 */

#ifndef __SYSTEM_H
#define __SYSTEM_H

void	system_enter_critical	(void) __attribute__ ((naked));
void	system_exit_critical	(void) __attribute__ ((naked));

#endif
