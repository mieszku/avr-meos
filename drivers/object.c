/*
 * Copyright (c) 2016 Mieszko Mazurek
 */

#include <panic.h>

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
}

void object_destruct (object_t* object)
{
	/* nothing to do */
}

void object_pure_virtual (object_t* this)
{
	enter_panic (ERROR_PURE_VIRTUAL);
}
