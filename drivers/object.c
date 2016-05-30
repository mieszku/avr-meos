/*
 * Copyright (c) 2016 Mieszko Mazurek
 */

#include <panic.h>

typedef struct object_t		object_t;
typedef struct objectvt_t	objectvt_t;

struct object_t
{
	objectvt_t*	vtable;
};

struct objectvt_t
{
	void	(*destruct)	(object_t*	object_t);
};

void __c_object_pure_virtual (void)
{
	enter_panic (ERROR_PURE_VIRTUAL);
}

void object_destruct (void* object)
{
	((object_t*) object)->vtable->destruct ((object_t*) object);
}
