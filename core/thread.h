/*
 * Copyright (c) 2016 Mieszko Mazurek
 */

#ifndef __THREAD_H
#define __THREAD_H

typedef void (*thread_func_t) (void*);

typedef struct thread_t thread_t;

struct thread_t
{
	thread_t*	_next;
	thread_t*	_prev;
	void*		_sptr;
	void*		_spnd;
	uint8_t		_flag;
	uint8_t		_pdng;

	const char*	name;
} __attribute__ ((packed));

thread_t*	thread_exec	(thread_func_t	run,
				 void*		obj,
				 const char*	name,
				 thread_t*	thread,
				 void*		stack,
				 uint16_t	size);
void		thread_kill	(void) __attribute__ ((noreturn));

#endif
