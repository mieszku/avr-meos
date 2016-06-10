/*
 * Copyright (c) 2016 Mieszko Mazurek
 */

#ifndef __HD44780LCD_H
#define __HD44780LCD_H

#include <inttypes.h>

#include "object.h"
#include "hd44780.h"
#include "hd44780itf.h"
#include "ostream.h"

typedef enum lcd_type_t 	lcd_type_t;

enum lcd_type_t
{
	LCD2X16,
	LCD4X20
};

#define HD44780LCD(x)		((hd44780lcd_t*) (x))

typedef struct hd44780lcd_t	hd44780lcd_t;
typedef struct hd44780lcdvt_t	hd44780lcdvt_t;

struct hd44780lcd_t
{
	ostream_t	ostream;

	hd44780_t	_hd44780;

	lcd_type_t	_type;
	uint8_t		_posy;
	uint8_t		_posx;
	uint8_t		_width;
	uint8_t		_height;
};

struct hd44780lcdvt_t
{
	ostreamvt_t	ostreamvt;
};

void		hd44780lcdvt_init	(hd44780lcdvt_t*	vtable);

hd44780lcd_t*	hd44780lcd_new		(hd44780itf_t*		interface,
					 uint8_t		itfmode,
					 lcd_type_t		lcd);
void		hd44780lcd_construct	(hd44780lcd_t*		object,
					 hd44780itf_t*		interface,
					 uint8_t		itfmode,
					 lcd_type_t		lcd);
void		hd44780lcd_destruct	(hd44780lcd_t*		object);

void		hd44780lcd_clear	(hd44780lcd_t*		object);
void		hd44780lcd_put_char	(hd44780lcd_t*		object,
					 char			chr);

static void	hd44780lcd_set_position	(hd44780lcd_t*		object,
					 uint8_t		posy,
					 uint8_t		posx);
static uint8_t	hd44780lcd_get_y	(hd44780lcd_t*		object);
static uint8_t	hd44780lcd_get_x	(hd44780lcd_t*		object);

#include "hd44780lcd.impl.h"

#endif
