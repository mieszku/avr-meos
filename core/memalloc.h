/*
 * Copyright (c) 2016 Mieszko Mazurek
 */

#ifndef __MEMALLOC_H
#define __MEMALLOC_H

#include <inttypes.h>

#include "config.h"

#ifdef __cplusplus
extern "C" {
#endif

void*		memalloc		(uint16_t 	size)
		__attribute__ ((malloc));
void*		xmemalloc		(uint16_t	size)
		__attribute__ ((malloc));
void*		memcalloc		(uint16_t	num,
					 uint16_t	size)
		__attribute__ ((malloc));
void*		memrealloc		(void*    	mem,
					 uint16_t 	newsize)
		__attribute__ ((malloc));
uint16_t	memalloc_real_size	(void* 	  	mem);
void		memfree			(void* 	  	mem);

#ifdef __cplusplus
}
#endif

#endif
