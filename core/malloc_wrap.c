/*
 * Copyright (c) 2016 Mieszko Mazurek
 */

#include "memalloc.h"

__attribute__ ((naked))
void* __wrap_malloc (uint16_t size)
{
	asm ("jmp memalloc");
}

void* __real_malloc (uint16_t);

__attribute__ ((naked))
void* __wrap_calloc (uint16_t siz, uint16_t cnt)
{
	asm ("jmp memcalloc");
}

void* __real_calloc (uint16_t, uint16_t);

__attribute__ ((naked))
void* __wrap_realloc (void* mem, uint16_t size)
{
	asm ("jmp memrealloc");
}

void* __real_realloc (void*, uint16_t);

__attribute__ ((naked))
void* __wrap_free (void* mem)
{
	asm ("jmp memfree");
}

void* __real_free (void*);
