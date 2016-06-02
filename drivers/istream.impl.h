/*
 * Copyright (c) 2016 Mieszko Mazurek
 */

#ifndef __ISTREAM_IMPL_H
#define __ISTREAM_IMPL_H

#include "istream.h"

#define VTABLE(x)	((istreamvt_t*) OBJECT_VTABLE (x))

static inline
char istream_get_char (istream_t* this)
{
	return VTABLE (this)->get_char (this);
}

static inline
uint8_t istream_has_next (istream_t* this)
{
	return VTABLE (this)->has_next (this);
}

#undef VTABLE

#endif
