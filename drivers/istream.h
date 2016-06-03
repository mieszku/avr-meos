/*
 * Copyright (c) 2016 Mieszko Mazurek
 */

#ifndef __ISTREAM_H
#define __ISTREAM_H

#include <inttypes.h>

#include "object.h"

typedef struct istream_t	istream_t;
typedef struct istreamvt_t	istreamvt_t;

struct istream_t
{
	object_t	object;

	char*		_stack;
	uint8_t		_size;
	uint8_t		_quantity;
};

struct istreamvt_t
{
	objectvt_t	objectvt;
	uint8_t		(*has_next)	(istream_t*	object);
	char		(*get_char)	(istream_t*	object);
};

void		istreamvt_init		(istreamvt_t*	object);
void		istream_construct	(istream_t*	object);
void		istream_destruct	(istream_t*	object);

void		istream_unget_char	(istream_t*	object,
					 char		chr);

int16_t		istream_get_int		(istream_t*	object);
uint16_t	istream_get_uint	(istream_t*	object);

uint8_t		istream_has_next_int	(istream_t*	object);
uint8_t		istream_has_next	(istream_t*	object);
char		istream_get_char	(istream_t*	object);

#endif

