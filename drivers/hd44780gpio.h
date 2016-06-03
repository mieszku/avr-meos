/*
 * Copyright (c) 2016 Mieszko Mazurek
 */

#ifndef __HD44780GPIO_H
#define __HD44780GPIO_H

#include "object.h"
#include "hd44780itf.h"

#include <mutex.h>

#define HD44780GPIO(x)		((hd44780gpio_t*) (x))

typedef struct hd44780gpio_t	hd44780gpio_t;
typedef struct hd44780gpiovt_t	hd44780gpiovt_t;

struct hd44780gpio_t
{
	hd44780itf_t	hd44780itf;

	mutex_t		_lock;
	uint8_t		_gpio_mode;
	uint8_t		_itfmode;
	uint16_t	_rs;
	uint16_t	_rw;
	uint16_t	_en;
	uint16_t	_db [8];
};

struct hd44780gpiovt_t
{
	hd44780itfvt_t	hd44780itfvt;
};

void		hd44780gpiovt_init		(hd44780gpiovt_t*	vtable);

hd44780gpio_t*	hd44780gpio_new			(uint16_t		rs,
						 uint16_t		rw,
						 uint16_t		en,
						 uint16_t		db0,
						 uint16_t		db1,
						 uint16_t		db2,
						 uint16_t		db3,
						 uint16_t		db4,
						 uint16_t		db5,
						 uint16_t		db6,
						 uint16_t		db7);
void		hd44780gpio_construct		(hd44780gpio_t*		object,
						 uint16_t		rs,
						 uint16_t		rw,
						 uint16_t		en,
						 uint16_t		db0,
						 uint16_t		db1,
						 uint16_t		db2,
						 uint16_t		db3,
						 uint16_t		db4,
						 uint16_t		db5,
						 uint16_t		db6,
						 uint16_t		db7);
void		hd44780gpio_destruct		(hd44780gpio_t*		object);

void		hd44780gpio_set_interface_mode	(hd44780gpio_t*		object,
						 uint8_t		interface_mode);
void		hd44780gpio_write		(hd44780gpio_t*		object,
						 uint8_t		rs,
						 uint8_t		data);
uint8_t		hd44780gpio_read		(hd44780gpio_t*		object,
						 uint8_t		rs);

#endif
