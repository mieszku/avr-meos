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

#ifdef memalloc
	#undef memalloc
#endif

#define ALLOC_ATTEMPTS	16
#define OVERLAP		(sizeof (struct header) - sizeof (uint16_t))

#ifndef NULL
	#define NULL	((void*) 0)
#endif

struct header
{
	struct header	*next;
	struct header	*prev;
	uint16_t	size;
} 
__attribute__ ((packed));


/* .noinit section generates warning */
static uint8_t heap [HEAP_SIZE] __attribute__ ((section (".bss")));

struct header* 			_root [2];
static struct header* const 	root = (void*) _root;
static mutex_t			mutex;

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
	ins_header (root, h);
}

static inline void rm_header (struct header *h)
{
	h->prev->next = h->next;
	h->next->prev = h->prev;
}

__attribute__ ((section (".init7"), naked))
static void _init (void)
{
	root->next = (void*) heap;
	root->prev = (void*) heap;

	void* const hptr = heap;
	
	((struct header*) hptr)->next = root;
	((struct header*) hptr)->prev = root;
	((struct header*) hptr)->size = HEAP_SIZE - sizeof (struct header);
}

void __memalloc_reset (void)
{
	_init ();
	mutex_unlock (&mutex);
}

static void* try_alloc (uint16_t size)
{
	struct header* ch;
	struct header* h;

	ch = NULL;
	h = root->next;

	while (h != root) {
		if (h->size >= size && (!ch || h->size < ch->size))
			ch = h;

		h = h->next;
	}

	if (ch == NULL)
		return NULL;

	rm_header (ch);
	
	if (ch->size > size + sizeof (struct header)) {
		h = ((void*) ch) + size + sizeof (struct header);
		h->size = ch->size - size - sizeof (struct header);
		ch->size = size;
		add_header (h);
	}

	return ((void*) ch) + sizeof (uint16_t);
}

void* memalloc (uint16_t size)
{
	void* 	mem;
	uint8_t	attempts = ALLOC_ATTEMPTS;

	if (size > OVERLAP)
		size -= OVERLAP;

	while (attempts) {
		mutex_lock (&mutex);

		mem = try_alloc (size);

		mutex_unlock (&mutex);
		
		if (mem)
			break;

		system_yield ();
		attempts--;
	}

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

__attribute__ ((pure))
uint16_t memalloc_real_size (void* mem)
{
	return ((struct header*) (mem - sizeof (uint16_t)))->size;
}

static uint8_t _defrag (void)
{
	mutex_lock (&mutex);

	struct header* 	h = root->next;
	uint8_t 	opt = 0;

	while (h != root) {
		struct header* h2 = h->next;

		while (h2 != root) {
			if (h2 == ((void*) h) + h->size + sizeof (struct header)) {
				rm_header (h2);
				h->size += h2->size + sizeof (struct header);
				opt = 0xFF;
			} else 
			if (h == ((void*) h2) + h2->size + sizeof (struct header)) {
				rm_header (h);
				h2->size += h->size + sizeof (struct header);
				opt = 0xFF;
			}

			h2 = h2->next;
		}

		h = h->next;
	}

	mutex_unlock (&mutex);

	return opt;
}

__attribute__ ((unused))
static uint8_t _sort (void)
{
	uint16_t opt = 0;

	mutex_lock (&mutex);

	struct header* h = root->next;
	struct header* const end = root->prev;

	while (h != end) {
		__auto_type next = h->next;

		if (next < h) {
			rm_header (h);
			ins_header (next, h);
			opt = 0xFF;
		}
	}

	mutex_unlock (&mutex);

	return opt;
}

void memfree (void* mem)
{
	mutex_lock (&mutex);

	struct header* h1 = mem - sizeof (uint16_t);
	add_header (h1);

	mutex_unlock (&mutex);

	while (_defrag ());
	/* useless on fixed size heap */
	/* while (_sort ()); */
}
