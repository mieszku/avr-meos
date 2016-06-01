/*
 * Copyright (c) 2016 Mieszko Mazurek
 */

#ifndef __HD44780ITF_H
#define __HD44780ITF_H

#include <inttypes.h>

#include "object.h"

#define HD44780ITF(x)		((hd44780_t*) (x))

typedef struct hd44780itf_t	hd44780itf_t;
typedef struct hd44780itfvt_t	hd44780itfvt_t;

struct hd44780itf_t
{
	object_t	object;
};

struct hd44780itfvt_t
{
	objectvt_t	objectvt;

	void	(*set_interface_mode)		(hd44780itf_t*		object,
						 uint8_t		itfmode);
	void	(*write)			(hd44780itf_t*		object,
						 uint8_t		rs,
						 uint8_t		data);
	uint8_t	(*read)				(hd44780itf_t*		object,
						 uint8_t		rs);
};

void		hd44780itfvt_init		(hd44780itfvt_t*	vtable);
void		hd44780itf_construct		(hd44780itf_t*		object);
void		hd44780itf_destruct		(hd44780itf_t*		object);

static void	hd44780itf_set_interface_mode	(hd44780itf_t*		object,
						 uint8_t		itfmode);
static void	hd44780itf_write		(hd44780itf_t*		object,
						 uint8_t		rs,
						 uint8_t		data);
static uint8_t	hd44780itf_read			(hd44780itf_t*		object,
						 uint8_t		rs);

#include "hd44780itf.impl.h"

#endif
