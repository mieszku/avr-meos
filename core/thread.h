/*
 * Copyright (c) 2016 Mieszko Mazurek
 */

#ifndef __THREAD_H
#define __THREAD_H

#include <inttypes.h>

typedef void (*thread_func_t) (void*);

typedef struct thread_t thread_t;

struct thread_t
{
	thread_t* 	_next;
	thread_t* 	_prev;
	void* 		_sptr;
	void*  		_spnd;
	uint8_t		_flags;

	const char* 	name;
} __attribute__ ((packed));

extern const thread_t* const 	thread_current;
extern thread_t			thread_main;

#ifdef __cplusplus
extern "C" {
#endif

thread_t*	thread_exec		(thread_func_t	run,
					 void*		obj,
					 const char*	name,
					 thread_t*	thread,
					 void*		stack,
					 uint16_t	size);
thread_t*	thread_run		(thread_func_t	run,
					 void*		obj,
					 const char*	name,
					 void*		mem,
					 uint16_t	size);
thread_t*	thread_run_alloc	(thread_func_t	run,
					 void*		obj,
					 const char*	name,
					 uint16_t	stsize);
void		thread_kill	(void) __attribute__ ((noreturn));

#ifdef __cplusplus
}
#endif

#endif
