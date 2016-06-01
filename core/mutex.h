/*
 * Copyright (c) 2016 Mieszko Mazurek
 */

#ifndef __MUTEX_H
#define __MUTEX_H

#include <inttypes.h>

typedef uint8_t mutex_t;

void	mutex_lock	(mutex_t*	mutex);
void	mutex_unlock	(mutex_t*	mutex);

#endif
