/*
 * Copyright (c) 2016 Mieszko Mazurek
 */

#ifndef __MUTEX_H
#define __MUTEX_H

#include <inttypes.h>

#define MUTEX_UNLOCKED	((mutex_t) 0)
#define MUTEX_LOCKED	((mutex_t) !(0))

typedef volatile uint8_t mutex_t;

void	mutex_lock		(mutex_t*	mutex);
void	mutex_unlock		(mutex_t*	mutex);
void	mutex_unlock_later	(mutex_t*	mutex,
				 uint16_t	delay);

#endif
