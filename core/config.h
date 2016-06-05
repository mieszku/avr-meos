/*
 * Copyright (c) 2016 Mieszko Mazurek
 */

#ifndef __CONFIG_H
#define __CONFIG_H

/*
 * keep all definitions below commented or not
 */

/* MCU definition */
//#define __atmega328p__
#define __atmega32__


/* specific platform */
//#define __arduino_uno__


/* system timer */
#define __system_timer__	0
//#define __system_timer__	1
//#define __system_timer__	2


/* timer freq multiplier */
#define __system_timer_x2__
//#define __system_timer_x4__
//#define __system_timer_x8__


/* use xmemalloc instead memalloc as default */
#define __xmemalloc_default__





/*
 * macros below must be defined
 */

/* maximum number of registered tasks */
#define MAX_TASKS		16

/* size of system stack */
#define SYSTEM_STACK_SIZE	64

/* fixed size of heap on which dynamic memory is allocated */
#define HEAP_SIZE		768 





/* DONT EDIT ASSERTION INSTRUCTIONS BELOW */

#ifdef __atmega328p__
	#define __mcu_defined__
#endif
#ifdef __atmega32__
	#ifdef __mcu_defined__
		#error please define only one mcu
	#endif
	#define __mcu_defined__
#endif

#ifndef __mcu_defined__
	#define please define one mcu
#endif

#ifndef __system_timer__
	#error please define one of possible system timer
#endif

#ifndef MAX_TASKS
	#error please define MAX_TASKS
#elif !defined SYSTEM_STACK_SIZE
	#error please define SYSTEM_STACK_SIZE
#elif !defined HEAP_SIZE
	#error please define HEAP_SIZE
#endif

#ifdef __xmemalloc_default__
	#define memalloc xmemalloc
#endif

#ifdef __system_timer_x2__
	#define __system_timer_mult__	2
	#define __system_timer_mul_defined__
#endif
#ifdef __system_timer_x4__
	#ifdef __system_timer_mul_defined__
		#error only one system clock multiplier can be defined
	#endif
	#define __system_timer_mult__	4
	#define __system_timer_mul_defined__
#endif
#ifdef __system_timer_x8__
	#ifdef __system_timer_mul_defined__
		#error only one system clock multiplier can be defined
	#endif
	#define __system_timer_mult__	8
	#define __system_timer_mul_defined__
#endif

#ifdef __system_timer_mul_defined__
	#undef __system_timer_mul_defined__
#else
	#define __system_timer_mult__	1
#endif

#endif
