/*
 * Copyright (c) 2016 Mieszko Mazurek
 */

#include "istream.h"

#include <memalloc.h>


static inline uint8_t is_digit (char c)
{
	return c >= '0' && c <= '9';
}

#define VTABLE(x)	((istreamvt_t*) OBJECT_VTABLE (x))

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

	this->_stack = 0;
}

void istream_destruct (istream_t* this)
{
	if (this->_stack)
		memfree (this->_stack);

	object_destruct (OBJECT (this));
}

void istream_unget_char (istream_t* this,
			 char	    chr)
{
	if (this->_size == this->_quantity) {
		this->_size += this->_size >> 1;
		memrealloc (this->_stack, this->_size);
	}
}

int16_t istream_get_int (istream_t* this)
{
	char neg;
	char fst = istream_get_char (this);

	if (fst == '-') {
		neg = 0xFF;
	} else {
		neg = 0;
		istream_unget_char (this, fst);
	}

	int16_t num = (int16_t) istream_get_uint (this);

	if (neg)
		num *= -1;

	return num;
}

uint16_t istream_get_uint (istream_t* this)
{
	uint16_t num = 0;

	do {
		char c = istream_get_char (this);

		if (! is_digit (c))
			break;
			
		num *= 10;
		num += c - '0';
	} while (1);

	return num;
}

uint8_t istream_has_next_int (istream_t* this)
{
	while (istream_has_next (this)) {
		char c = istream_get_char (this);
		if (is_digit (c) || c == '-') {
			istream_unget_char (this, c);
			return 1;
		}
	}
	return 0;
}

uint8_t istream_has_next (istream_t* this)
{
	return this->_size != 0 || VTABLE (this)->has_next (this);
}

char istream_get_char (istream_t* this)
{
	if (this->_size)
		return this->_stack [--this->_size];
	return VTABLE (this)->get_char (this);
}
