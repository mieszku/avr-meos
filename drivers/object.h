/*
 * Copyright (c) 2016 Mieszko Mazurek
 */

#ifndef __OBJECT_H
#define __OBJECT_H

#include <inttypes.h>

#define OBJECT(x)		((object_t*) (x))
#define OBJECT_VTABLE(x)	OBJECT (x)->vtable

#define OBJECT_DESTRUCT(x)	OBJECT_VTABLE(x)->destruct (x)
#define DESTRUCTOR(x)		((void (*) (object_t*)) (x))

typedef struct object_t 	object_t;
typedef struct objectvt_t	objectvt_t;

struct object_t
{
	objectvt_t*	vtable;
	uint8_t		refs;
};

struct objectvt_t
{
	void	(*destruct)	(object_t*	object);
};

void	objectvt_init		(objectvt_t*	vtable);

void	object_construct	(object_t*	object);
void	object_destruct		(object_t*	object);
void	object_pure_virtual	(object_t*	object);

void	object_ref		(void*		object);
void	object_unref		(void*		object);

#endif
