/*
 * Copyright (c) 2016 Mieszko Mazurek
 */

#include <inttypes.h>
#include <string.h>

#include "memalloc.h"
#include "system.h"
#include "mutex.h"
#include "config.h"
#include "panic.h"
#include "threadcfg.h"

#ifdef memalloc
	#undef memalloc
#endif

#define ALLOC_ATTEMPTS	64
#define OVERLAP		(sizeof (struct header) - sizeof (uint16_t))

#ifndef NULL
	#define NULL	((void*) 0)
#endif

extern uint8_t __heap_start;

struct header
{
	uint16_t	size;
	struct header	*next;
	struct header	*prev;
} 
__attribute__ ((packed));

void* memalloc_brk;

static mutex_t			memmtx;
static struct header* 		_root [2];

#define root			((struct header* const) (((void*) _root) - sizeof (uint16_t)))

static inline void ins_header (struct header* restrict p,
			       struct header* restrict n)
{
	n->next = p;
	n->prev = p->prev;
	p->prev->next = n;
	p->prev = n;
}

static inline void add_header (struct header *h)
{
	struct header* ptr = root->next;

	while (ptr != root && ptr < h)
		ptr = ptr->next;

	ins_header (ptr, h);
}

static inline void rm_header (struct header *h)
{
	h->prev->next = h->next;
	h->next->prev = h->prev;
}

static uint8_t defrag (void)
{
	struct header* 	h = root->next;
	uint8_t 	opt = 0;

	while (h != root) {
		struct header* h2 = h->next;

		while (h2 != root) {
			if (h2 == ((void*) h) + h->size + sizeof (uint16_t)) {
				rm_header (h2);
				h->size += h2->size + sizeof (uint16_t);
				opt = 0xFF;
			} else 
			if (h == ((void*) h2) + h2->size + sizeof (uint16_t)) {
				rm_header (h);
				h2->size += h->size + sizeof (uint16_t);
				opt = 0xFF;
			}

			h2 = h2->next;
		}

		h = h->next;
	}

	return opt;
}

void __memalloc_reset (void)
{
	root->next = root;
	root->prev = root;

	memalloc_brk = &__heap_start;
	mutex_unlock (&memmtx);
}

static uint8_t enlarge_heap (uint16_t size)
{
	uint16_t blksize = size + 2;
	struct header* ptr = memalloc_brk;
	void* newbrk = memalloc_brk + blksize;

	if (thread_main._sptr - STACK_MARGIN > newbrk) {
		thread_main._spnd = newbrk;
		ptr->size = size;
		add_header (ptr);

		memalloc_brk = newbrk;
		defrag ();
		return 1;
	}
	return 0;
}

static inline void decrease_heap (void)
{
	struct header* last = root->prev;

	if (((void*) last) + last->size + 2 == memalloc_brk) {
		memalloc_brk -= last->size + 2;
		rm_header (last);
	}
}

static void* try_alloc (uint16_t size)
{
	mutex_lock (&memmtx);

	struct header* ptr = root->next;
	struct header* best = NULL;

	while (ptr != root) {
		if (ptr->size >= size &&
			(best == NULL || ptr->size < best->size))
		{
			best = ptr;
		}

		ptr = ptr->next;
	}

	if (best == NULL) {
		mutex_unlock (&memmtx);
		return NULL;
	}

	rm_header (best);

	if (best->size > size + sizeof (struct header)) {
		ptr = ((void*) best) + sizeof (uint16_t) + size;
		ptr->size = best->size - sizeof (uint16_t) - size;
		best->size = size;
		add_header (ptr);
	}

	mutex_unlock (&memmtx);

	return ((void*) best) + sizeof (uint16_t);
}

void* memalloc (uint16_t size)
{
	void*	mem;
	uint8_t	attempts;

	attempts = ALLOC_ATTEMPTS;

	if (size < OVERLAP)
		size = OVERLAP;

	do {
		mem = try_alloc (size);

		if (mem)
			break;

		if (enlarge_heap (size))
			continue;

		system_yield ();
	} while (--attempts);

	return mem;
}

void* xmemalloc (uint16_t size)
{
	void* mem = memalloc (size);
	if (mem)
		return mem;
	enter_panic (ERROR_XMEMALLOC);
}

void* memcalloc (uint16_t num, uint16_t size)
{
	size *= num;
	void* mem = memalloc (size);

	if (mem) 
		memset (mem, 0, size);
	
	return mem;
}

void* memrealloc (void* oldmem, uint16_t newsize)
{
	uint8_t oldsize;
	void*	newmem;

	if (! oldmem)
		return memalloc (newsize);
	
	oldsize = memalloc_real_size (oldmem);

	memfree (oldmem);
	newmem = memalloc (newsize);

	if (newmem != oldmem) {
		uint8_t less = newsize < oldsize ? newsize : oldsize;
		memcpy (newmem, oldmem, less);
	}

	return newmem;
}

uint16_t memalloc_real_size (void* mem)
{
	return ((struct header*) (mem - sizeof (uint16_t)))->size;
}

void memfree (void* mem)
{
	mutex_lock (&memmtx);

	struct header* h1 = mem - sizeof (uint16_t);
	add_header (h1);

	while (defrag ());
	decrease_heap ();

	mutex_unlock (&memmtx);
}
