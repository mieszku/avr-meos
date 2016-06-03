/*
 * Copyright (c) 2016 Mieszko Mazurek
 */

#include "memutils.h"

void memcopy (void *dst, void *src, uint8_t siz)
{
	while (siz--)
		*((uint8_t *) dst++) = *((uint8_t *) src++);
}

void memswap (void *mem1, void *mem2, uint8_t siz)
{
	while (siz--) {
		const uint8_t tmp = *((uint8_t *) mem1);
		*((uint8_t *) mem1++) = *((uint8_t *) mem2);
		*((uint8_t *) mem2++) = tmp;
	}
}

void memshr (void *mem, uint8_t siz)
{
	uint8_t * const end = mem;
	uint8_t *ptr = mem + siz - 1;

	while (ptr != end) {
		ptr [0] = ptr [-1];
		ptr--;
	}
}

void memshl (void *mem, uint8_t siz)
{
	uint8_t *ptr = mem;
	uint8_t * const end = mem + siz - 1;

	while (ptr != end) {
		ptr [0] = ptr [1];
		ptr++;
	}
}

void memshrn (void *mem, uint8_t siz, uint8_t sh)
{
	uint8_t * const end = mem + sh;
	uint8_t *ptr = mem + siz - 1;

	while (ptr >= end) {
		ptr [0] = ptr [-sh];
		ptr--;
	}
}

void memshln (void *mem, uint8_t siz, uint8_t sh)
{
	uint8_t *ptr = mem;
	uint8_t * const end = mem + siz - sh;

	while (ptr != end) {
		ptr [0] = ptr [sh];
		ptr++;
	}
}

void memrotl (void *mem, uint8_t siz)
{
	const int8_t tmp = ((int8_t *) mem) [0];
	memshl (mem, siz);
	((int8_t *) mem) [siz - 1] = tmp;
}

void memrotr (void *mem, uint8_t siz)
{
	const int8_t tmp = ((int8_t *) mem) [siz - 1];
	memshr (mem, siz);
	((int8_t *) mem) [0] = tmp;
}

void memrotln (void *mem, uint8_t siz, uint8_t sh)
{
	while (sh--)
		memrotl (mem, siz);
}

void memrotrn (void *mem, uint8_t siz, uint8_t sh)
{
	while (sh--)
		memrotr (mem, siz);
}

void memfill (void *mem, uint8_t siz, uint8_t val)
{
	uint8_t *ptr = mem;
	uint8_t * const end = ptr + siz;
	while (ptr != end)
		*ptr++ = val;
}

char *strcopy (char *dst, const char *str)
{
	while (*str)
		*dst++ = *str++;
	return dst;
}

char *strappend (char *dst, const char *cat)
{
	while (*dst)
		dst++;
	return strcopy (dst, cat);
}
