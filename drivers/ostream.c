/*
 * Copyright (c) 2016 Mieszko Mazurek
 */

#include "ostream.h"

extern void __c_object_pure_virtual (void);


static ostreamvt_t ostreamvt;

__attribute__ ((constructor))
static void __init_vtable (void)
{
	ostreamvt_init (&ostreamvt);
}

void ostreamvt_init (ostreamvt_t* vtable)
{
	vtable->destruct = ostream_destruct;
	vtable->put_char = 
		(void (*) (ostream_t*, char)) __c_object_pure_virtual;
}



void ostream_construct (ostream_t* this)
{
	this->vtable = &ostreamvt;
}

void ostream_destruct (ostream_t* this)
{
	/* nothing to do */
}

void ostream_put_string (ostream_t*  this,
			 const char* str)
{
	while (*str)
		ostream_put_char (this, *str++);
}

void ostream_put_int (ostream_t* this,
		      int16_t	 n)
{
	if (n < 0) {
		ostream_put_char (this, '-');
		n *= -1;
	}
	ostream_put_uint (this, (uint16_t) n);
}

void ostream_put_uint (ostream_t* this,
		       uint16_t	  n)
{
	static char buff [16];
	char* ptr = buff;

	while (n) {
		*ptr++ = n % 10;
		n /= 10;
	}

	if (ptr == buff)
		ostream_put_char (this, '0');
	else do
		ostream_put_char (this, *--ptr + '0');
	while (ptr != buff);
}

void ostream_put_int32 (ostream_t* this,
			int32_t	 n)
{
	if (n < 0) {
		ostream_put_char (this, '-');
		n *= -1;
	}
	ostream_put_uint (this, (uint16_t) n);
}

void ostream_put_uint32 (ostream_t* this,
			 uint32_t	  n)
{
	static char buff [16];
	char* ptr = buff;

	while (n) {
		*ptr++ = n % 10;
		n /= 10;
	}

	if (ptr == buff)
		ostream_put_char (this, '0');
	else do
		ostream_put_char (this, *--ptr + '0');
	while (ptr != buff);
}
