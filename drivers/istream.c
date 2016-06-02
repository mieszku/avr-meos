/*
 * Copyright (c) 2016 Mieszko Mazurek
 */

#include "istream.h"

#include <memalloc.h>

static istreamvt_t istreamvt;

__attribute__ ((constructor))
static void __init_vtable (void)
{
	istreamvt_init (&istreamvt);
}


void istreamvt_init (istreamvt_t* vtable)
{
	objectvt_init ((objectvt_t*) vtable);
	OBJECT_VTABLE (vtable)->destruct = DESTRUCTOR (istream_destruct);

	vtable->get_char = (void*) object_pure_virtual;
	vtable->has_next = (void*) object_pure_virtual;
}

void istream_construct (istream_t* this)
{
	object_construct (OBJECT (this));
	OBJECT_VTABLE (this) = (objectvt_t*) &istreamvt;
}

void istream_destruct (istream_t* this)
{
	if (this->_stack)
		memfree (this->_stack);

	object_destruct (OBJECT (this));
}
