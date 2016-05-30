/*
 * Copyright (c) 2016 Mieszko Mazurek
 */

#include "hd44780itf.h"

extern void (*__c_object_pure_virtual) (void*);

static hd44780itfvt_t hd44780itfvt;

__attribute__ ((constructor))
static void __init_vtable (void)
{
	hd44780itfvt_init (&hd44780itfvt);
}

void hd44780itfvt_init (hd44780itfvt_t* vtable)
{
	vtable->set_interface_mode 	= (void*) __c_object_pure_virtual;
	vtable->write 			= (void*) __c_object_pure_virtual;
	vtable->read 			= (void*) __c_object_pure_virtual;
}

void hd44780itf_construct (hd44780itf_t* this)
{
	this->vtable = &hd44780itfvt;
}

void hd44780itf_destruct (hd44780itf_t* this)
{
	/* nothing to do */
}
