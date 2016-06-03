/*
 * Copyright (c) 2016 Mieszko Mazurek
 */

#include <panic.h>
#include <memalloc.h>

#include "object.h"

static objectvt_t objectvt;

__attribute__ ((constructor))
static void __init_vtable (void)
{
	objectvt_init (&objectvt);
}


void objectvt_init (objectvt_t* vtable)
{
	vtable->destruct = object_destruct;
}

void object_construct (object_t* this)
{
	this->vtable = &objectvt;
	this->refs = 1;
}

void object_destruct (object_t* object)
{
	/* nothing to do */
}

void object_pure_virtual (object_t* this)
{
	enter_panic (ERROR_PURE_VIRTUAL);
}

void object_ref (void* ptr)
{
	((object_t*) ptr)->refs++;
}

void object_unref (void* ptr)
{
	object_t* this = (object_t*) ptr;

	if (this->refs) {
		this->refs--;

		if (! this->refs) {
			OBJECT_DESTRUCT (this);
			memfree (ptr);
		}
	}
}
