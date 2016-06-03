/*
 * Copyright (c) 2016 Mieszko Mazurek
 */

#ifndef __TASK_H
#define __TASK_H

#include <inttypes.h>

typedef uint8_t (*task_func_t) (void*);

#ifdef __cplusplus
extern "C" {
#endif

void	task_register	(task_func_t 	task,
			 void*		object,
			 uint16_t	delay,
			 uint16_t	period);

#ifdef __cplusplus
}
#endif

#endif
