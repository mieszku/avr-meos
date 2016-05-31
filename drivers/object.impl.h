/*
 * Copyright (c) 2016 Mieszko Mazurek
 */

#ifndef __OBJECT_IMPL_H
#define __OBJECT_IMPL_H

#include "object.h"

static inline
void object_destruct (object_t* this)
{
	this->vtable->destruct (this);
}

#endif
