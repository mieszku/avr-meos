/*
 * Copyright (c) 2016 Mieszko Mazurek
 */

#ifndef __HD44780ITF_IMPL_H
#define __HD44780ITF_IMPL_H

#include "hd44780itf.h"

#define VTABLE(x)	((hd44780itfvt_t*) OBJECT_VTABLE (x))

static inline
void hd44780itf_set_interface_mode (hd44780itf_t* this,
				    uint8_t	  itfmode)
{
	VTABLE (this)->set_interface_mode (this, itfmode);
}

static inline
void hd44780itf_write (hd44780itf_t* this,
		       uint8_t	     rs,
		       uint8_t	     data)
{
	VTABLE (this)->write (this, rs, data);
}

static inline
uint8_t hd44780itf_read (hd44780itf_t* this,
			 uint8_t       rs)
{
	return VTABLE (this)->read (this, rs);
}

#undef VTABLE

#endif
