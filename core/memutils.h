/*
 * Copyright 2016 Mieszko Mazurek
 */

#ifndef __UTILS_H
#define __UTILS_H

#include <inttypes.h>

#ifndef NULL
#define NULL		((void *) 0)
#endif

#define min(p, q)	((p) < (q) ? (p) : (q))
#define max(p, q)	((p) > (q) ? (p) : (q))

#ifdef __cplusplus
extern "C" {
#endif

void 	memcopy 	(void *dst, void *src, uint8_t siz);
void 	memswap 	(void *mem1, void *mem2, uint8_t siz);

void 	memshr		(void *mem, uint8_t siz);
void 	memshl		(void *mem, uint8_t siz);

void	memshrn		(void *mem, uint8_t siz, uint8_t sh);
void	memshln		(void *mem, uint8_t siz, uint8_t sh);

void	memrotl		(void *mem, uint8_t siz);
void	memrotr		(void *mem, uint8_t siz);

void	memrotln	(void *mem, uint8_t siz, uint8_t sh);
void	memrotrn	(void *mem, uint8_t siz, uint8_t sh);

void	memfill		(void *mem, uint8_t siz, uint8_t val);

char	*strcopy	(char *dst, const char *str);
char	*strappend	(char *dst, const char *cat);

#ifdef __cplusplus
}
#endif

#endif
