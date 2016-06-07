/*
 * Copyright (c) 2016 Mieszko Mazurek
 */

#ifndef __PANIC_H
#define __PANIC_H

#include "error.h"

#include <inttypes.h>

typedef uint8_t error_t;

void	enter_panic	(error_t	error) __attribute__ ((noreturn));

#endif
