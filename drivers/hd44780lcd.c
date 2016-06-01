/*
 * Copyright (c) 2016 Mieszko Mazurek
 */

#include "hd44780lcd.h"

#include "object.h"

#include <panic.h>
#include <memalloc.h>

static uint8_t real_addr (hd44780lcd_t*);


static hd44780lcdvt_t hd44780lcdvt;

__attribute__ ((constructor))
static void __init_vtable (void)
{
	hd44780lcdvt_init (&hd44780lcdvt);
}


void hd44780lcdvt_init (hd44780lcdvt_t* vtable)
{
	ostreamvt_init (&vtable->ostreamvt);
	((objectvt_t*) vtable)->destruct =
		(void (*) (object_t*))
		hd44780lcd_destruct;
	((ostreamvt_t*) vtable)->put_char = 
		(void (*) (ostream_t*, char)) hd44780lcd_put_char;
}


hd44780lcd_t* hd44780lcd_new (hd44780itf_t* interface,
			      lcd_type_t    lcd_type)
{
	hd44780lcd_t* instance = xmemalloc (sizeof (hd44780lcd_t));
	hd44780lcd_construct (instance, interface, lcd_type);
	return instance;
}

void hd44780lcd_construct (hd44780lcd_t* this,
			   hd44780itf_t* interface,
			   lcd_type_t	 lcd_type)
{
	ostream_construct (&this->ostream);
	OBJECT_VTABLE (this) = (objectvt_t*) &hd44780lcdvt;

	hd44780_construct (&this->_hd44780,
			   interface);
	
	this->_type = lcd_type;

	switch (lcd_type) {
	case LCD2X16: 
		this->_width = 16;
		this->_height = 2;
		break;
	case LCD4X20:
		this->_width = 20;
		this->_height = 4;
		break;
	default:
		enter_panic (ERROR_INVALID_ARGUMENT);
	}
	
	hd44780_function_set (&this->_hd44780,
			      INTERFACE_MODE_4BIT,
			      LINE_MODE_TWOLINE,
			      MATRIX_5X7);
	hd44780_display_set (&this->_hd44780,
			     DISPLAY_ON,
			     CURSOR_MODE_NONE);
	hd44780_display_clear (&this->_hd44780);
}

void hd44780lcd_destruct (hd44780lcd_t* this)
{
	hd44780_destruct (&this->_hd44780);
	ostream_destruct (OSTREAM (this));
}

void hd44780lcd_clear (hd44780lcd_t* this)
{
	hd44780_display_clear (&this->_hd44780);
}

void hd44780lcd_put_char (hd44780lcd_t* this,
			  char 		chr)
{
	uint8_t addr = real_addr (this);

	hd44780_dd_ram_set (&this->_hd44780, addr);
	hd44780_data_write (&this->_hd44780, chr);

	if (this->_posx + 1 == this->_width) {
		this->_posx = 0;
		if (this->_posy + 1 == this->_height)
			this->_posy = 0;
		else
			this->_posy++;
	} else {
		this->_posx++;
	}
}



static uint8_t get_addr_4x20 (uint8_t y, uint8_t x)
{
	if (y > 1)
		x += 20;
	
	switch (y) {
	case 1: y = 2; break;
	case 2: y = 1; break;
	}

	return y > 1 ? 0x40 | x : x;
}

static uint8_t get_addr_2x16 (uint8_t y, uint8_t x)
{
	return y ? x | 0x40 : x;
}

static uint8_t (*get_addr []) (uint8_t, uint8_t) = {
	get_addr_2x16,
	get_addr_4x20
};

static uint8_t real_addr (hd44780lcd_t* this)
{
	return get_addr [this->_type] (this->_posy, this->_posx);
}
