/*
 * Copyright (c) 2016 Mieszko Mazurek
 */

#ifndef __CONFIG_H
#define __CONFIG_H

/* specific platform */
#define __arduino_uno__


/* system timer */
//#define __system_timer__	0
#define __system_timer__	1
//#define __system_timer__	2


/* timer freq multiplier */
//#define __system_timer_mult__	1
//#define __system_timer_mult__	2
//#define __system_timer_mult__	4
//#define __system_timer_mult__	8

/* use xmemalloc instead memalloc as default */
#define __xmemalloc_default__




/*
 * macros below must be defined
 */

/* maximum number of registered tasks */
#define MAX_TASKS		16

/* size of system stack */
#define SYSTEM_STACK_SIZE	32

/* fixed size of heap on which dynamic memory is allocated */
#define HEAP_SIZE		768






#ifndef __system_timer__
	#error __system_timer__ was not defined
#endif

#ifndef MAX_TASKS
	#error MAX_TASKS was not defined
#elif !defined SYSTEM_STACK_SIZE
	#error SYSTEM_STACK_SIZE was not defined
#elif !defined HEAP_SIZE
	#error HEAP_SIZE was not defined
#endif

#ifdef __xmemalloc_default__
	#define memalloc xmemalloc
#endif


#ifdef __system_timer_mult__
	#if 	__system_timer_mult__ != 1 &&\
		__system_timer_mult__ != 2 &&\
		__system_timer_mult__ != 4 &&\
		__system_timer_mult__ != 8
		#error	__system_timer_mult__ must be equal 1, 2, 4 or 8
	#endif
#else
	#define __system_timer_mult__	1
#endif

#endif
