/*
 * Copyright (c) 2016 Mieszko Mazurek
 */

#ifndef __HD44780ITF_H
#define __HD44780ITF_H

#include <inttypes.h>

typedef struct hd44780itf_t	hd44780itf_t;
typedef struct hd44780itfvt_t	hd44780itfvt_t;

struct hd44780itf_t
{
	hd44780itfvt_t*	vtable;
};

struct hd44780itfvt_t
{
	void	(*destruct)		(hd44780itf_t*		object);
	void	(*set_interface_mode)	(hd44780itf_t*		object,
					 uint8_t		itfmode);
	void	(*write)		(hd44780itf_t*		object,
					 uint8_t		rs,
					 uint8_t		data);
	uint8_t	(*read)			(hd44780itf_t*		object,
					 uint8_t		rs);
};

void	hd44780itfvt_init		(hd44780itfvt_t*	vtable);
void	hd44780itf_construct		(hd44780itf_t*		object);
void	hd44780itf_destruct		(hd44780itf_t*		object);

static inline
void	hd44780itf_set_interface_mode	(hd44780itf_t*		object,
					 uint8_t		itfmode)
{	object->vtable->set_interface_mode (object, itfmode);	}

static inline
void	hd44780itf_write		(hd44780itf_t*		object,
					 uint8_t		rs,
					 uint8_t		data)
{	object->vtable->write (object, rs, data);		}

static inline
uint8_t	hd44780itf_read 		(hd44780itf_t*		object,
					 uint8_t		rs)
{	return object->vtable->read (object, rs);		}

#endif
