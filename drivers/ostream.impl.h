/*
 * Copyright (c) 2016 Mieszko Mazurek
 */

#ifndef __OSTREAM_IMPL_H
#define __OSTREAM_IMPL_H

#include "ostream.h"

#define VTABLE(x)	((ostreamvt_t*) OBJECT_VTABLE (x))

static inline 
void ostream_put_char (ostream_t* this,
		       char 	  chr)
{
	VTABLE (this)->put_char (this, chr);
}

#undef VTABLE

#endif
