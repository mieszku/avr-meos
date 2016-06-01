/*
 * Copyright (c) 2016 Mieszko Mazurek
 */

#ifndef __HD44780_H
#define __HD44780_H

#include <inttypes.h>

#include "object.h"
#include "hd44780itf.h"

#define INTERFACE_MODE_4BIT	0
#define INTERFACE_MODE_8BIT	1
#define ENTRY_MODE_CONTENT	0
#define ENTRY_MODE_CURSOR	1
#define ENTRY_DIRECTION_LEFT	0
#define ENTRY_DIRECTION_RIGHT	1
#define DISPLAY_OFF		0
#define DISPLAY_ON		1
#define SHIFT_MODE_CURSOR	0
#define SHIFT_MODE_CONTENT	1
#define SHIFT_DIRECTION_LEFT	0
#define SHIFT_DIRECTION_RIGHT	1
#define CURSOR_MODE_NONE	0
#define CURSOR_MODE_BLOCK	1
#define CURSOR_MODE_UNDERLINE	2
#define LINE_MODE_ONELINE	0
#define LINE_MODE_TWOLINE	1
#define MATRIX_5X7		0
#define MATRIX_5X10		1

#define HD44780(x)		((hd44780_t*) (x))

typedef struct hd44780_t	hd44780_t;
typedef struct hd44780vt_t	hd44780vt_t;

struct hd44780_t
{
	object_t	object;
	hd44780itf_t*	_interface;
};

struct hd44780vt_t
{
	objectvt_t	objectvt;
};



void		hd44780vt_init		(hd44780vt_t*	vtable);

hd44780_t*	hd44780_new		(hd44780itf_t*	interface);
void		hd44780_construct	(hd44780_t*	object,
					 hd44780itf_t*	interface);
void		hd44780_destruct	(hd44780_t*	object);

void		hd44780_display_clear	(hd44780_t*	object);
void		hd44780_cursor_home	(hd44780_t*	object);
void		hd44780_entry_mode_set	(hd44780_t*	object,
					 uint8_t	entry_mode,
					 uint8_t	entry_direction);
void		hd44780_display_set	(hd44780_t*	object,
					 uint8_t	display,
					 uint8_t	cursor);
void		hd44780_shift		(hd44780_t*	object,
				 	 uint8_t	shift_mode,
					 uint8_t	shift_direction);
void		hd44780_function_set	(hd44780_t*	object,
					 uint8_t	interface_mode,
					 uint8_t	line_mode,
					 uint8_t	matrix);
void		hd44780_cg_ram_set	(hd44780_t*	object,
					 uint8_t	addr);
void		hd44780_dd_ram_set	(hd44780_t*	object,
					 uint8_t	addr);
uint8_t		hd44780_busy_flag_read	(hd44780_t*	object);
uint8_t		hd44780_data_read	(hd44780_t*	object);
void		hd44780_data_write	(hd44780_t*	object,
					 uint8_t	data);

#endif
