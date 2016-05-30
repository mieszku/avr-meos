/*
 * Copyright (c) 2016 Mieszko Mazurek
 */

#ifndef __HD44780LCD_H
#define __HD44780LCD_H

#include <inttypes.h>

#include "hd44780.h"
#include "hd44780itf.h"
#include "ostream.h"

typedef enum lcd_type_t 	lcd_type_t;

enum lcd_type_t
{
	LCD2X16,
	LCD4X20
};

typedef struct hd44780lcd_t	hd44780lcd_t;
typedef struct hd44780lcdvt_t	hd44780lcdvt_t;

struct hd44780lcd_t
{
	union {
		ostream_t	ostream;
		hd44780lcdvt_t*	vtable;
	};

	hd44780_t	_hd44780;

	lcd_type_t	_type;
	uint8_t		_posy;
	uint8_t		_posx;
	uint8_t		_width;
	uint8_t		_height;
};

struct hd44780lcdvt_t
{
	union {
		ostreamvt_t	ostreamvt;
		void		(*destruct)	(hd44780lcd_t*	object);
	};
};

void		hd44780lcdvt_init	(hd44780lcdvt_t*	vtable);
void		hd44780lcd_construct	(hd44780lcd_t*		object,
					 hd44780itf_t*		interface,
					 lcd_type_t		lcd);
void		hd44780lcd_destruct	(hd44780lcd_t*		object);
void		hd44780lcd_clear	(hd44780lcd_t*		object);
void		hd44780lcd_put_char	(hd44780lcd_t*		object,
					 char			chr);

static void	hd44780lcd_set_position	(hd44780lcd_t*		object,
					 uint8_t		posy,
					 uint8_t		posx);

#include "hd44780lcd.impl.h"

#endif
