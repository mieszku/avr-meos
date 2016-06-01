/*
 * Copyright (c) 2016 Mieszko Mazurek
 */

#ifndef __OSTREAM_H
#define __OSTREAM_H

#include <inttypes.h>

#include "object.h"

#define OSTREAM(x)	((ostream_t*) (x))

typedef struct ostream_t	ostream_t;
typedef struct ostreamvt_t	ostreamvt_t;

struct ostream_t
{
	object_t	object;
};

struct ostreamvt_t
{
	objectvt_t	objectvt;
	void		(*put_char)	(ostream_t*	object,
					 char		chr);
};

void		ostreamvt_init		(ostreamvt_t*	vtable);

void		ostream_construct	(ostream_t*	object);
void		ostream_destruct	(ostream_t*	object);

void		ostream_put_string	(ostream_t*	object,
					 const char*	str);
void		ostream_put_int		(ostream_t*	object,
					 int16_t	integer);
void		ostream_put_uint	(ostream_t*	object,
					 uint16_t	uinteger);
void		ostream_put_int32	(ostream_t*	object,
					 int32_t	integer);
void		ostream_put_uint32	(ostream_t*	object,
					 uint32_t	uinteger);

static void	ostream_put_char	(ostream_t*	object,
					 char 		chr);

#include "ostream.impl.h"

#endif
