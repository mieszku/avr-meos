/*
 * Copyright (c) 2016 Mieszko Mazurek
 */

#ifndef __OSTREAM_IMPL_H
#define __OSTREAM_IMPL_H

#include "ostream.h"

static inline 
void ostream_put_char (ostream_t* object,
		       char 	  chr)
{
	object->vtable->put_char (object, chr);
}


#endif
