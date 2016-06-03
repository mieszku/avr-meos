/*
 * Copyright (c) 2016 Mieszko Mazurek
 */

#include "hd44780gpio.h"
#include "hd44780itf.h"

#include "hd44780.h"

#include <gpio.h>
#include <system.h>
#include <memalloc.h>

#include <util/delay.h>

#define GPIO_UNDEFINED		10
#define FIRST_BIT(itf)		(((itf) == INTERFACE_MODE_4BIT) << 2)

static hd44780gpiovt_t hd44780gpiovt;

__attribute__ ((constructor))
static void __init_vtable (void)
{
	hd44780gpiovt_init (&hd44780gpiovt);
}

void hd44780gpiovt_init (hd44780gpiovt_t* vtable)
{
	hd44780itfvt_init (&vtable->hd44780itfvt);

	((objectvt_t*) vtable)->destruct =
		(void (*) (object_t*)) hd44780gpio_destruct;

	vtable->hd44780itfvt.set_interface_mode = 
		(void (*) (hd44780itf_t*, uint8_t)) 
		hd44780gpio_set_interface_mode;
	
	vtable->hd44780itfvt.write =
		(void (*) (hd44780itf_t*, uint8_t, uint8_t)) 
		hd44780gpio_write;
	
	vtable->hd44780itfvt.read =
		(uint8_t (*) (hd44780itf_t*, uint8_t)) 
		hd44780gpio_read;
}

hd44780gpio_t* hd44780gpio_new (uint16_t rs,
				uint16_t rw,
				uint16_t en,
				uint16_t db0,
				uint16_t db1,
				uint16_t db2,
				uint16_t db3,
				uint16_t db4,
				uint16_t db5,
				uint16_t db6,
				uint16_t db7)
{
	hd44780gpio_t* instance = xmemalloc (sizeof (hd44780gpio_t));
	hd44780gpio_construct (instance, rs, rw, en,
			       db0, db1, db2, db3,
			       db4, db5, db6, db7);
	return instance;
}

void hd44780gpio_construct (hd44780gpio_t* this,
			    uint16_t	   rs,
			    uint16_t	   rw,
			    uint16_t	   en,
			    uint16_t       db0,
			    uint16_t       db1,
			    uint16_t       db2,
			    uint16_t       db3,
			    uint16_t       db4,
			    uint16_t       db5,
			    uint16_t       db6,
			    uint16_t       db7)
{
	hd44780itf_construct (&this->hd44780itf);
	OBJECT_VTABLE (this) = (objectvt_t*) &hd44780gpiovt;

	this->_gpio_mode = GPIO_UNDEFINED;
	this->_rs = rs;
	this->_rw = rw;
	this->_en = en;
	this->_db [0] = db0;
	this->_db [1] = db1;
	this->_db [2] = db2;
	this->_db [3] = db3;
	this->_db [4] = db4;
	this->_db [5] = db5;
	this->_db [6] = db6;
	this->_db [7] = db7;
}

void hd44780gpio_destruct (hd44780gpio_t* this)
{
	hd44780itf_destruct (&this->hd44780itf);
}

static void set_gpio_mode (hd44780gpio_t* this,
			   uint8_t	  mode)
{
	if (this->_gpio_mode != mode) {
		this->_gpio_mode = mode;

		gpio_mode (this->_rs, GPIO_OUTPUT);
		gpio_mode (this->_rw, GPIO_OUTPUT);
		gpio_mode (this->_en, GPIO_OUTPUT);

		for (uint8_t i = FIRST_BIT (this->_itfmode); i < 8; i++)
			gpio_mode (this->_db [i], mode);
	}
}

void hd44780gpio_set_interface_mode (hd44780gpio_t* this,
				     uint8_t	    interface_mode)
{
	this->_itfmode = interface_mode;
}

static void write_byte (hd44780gpio_t* this,
			uint8_t	       rs,
			uint8_t	       data)
{
	set_gpio_mode (this, GPIO_OUTPUT);
	
	gpio_write (this->_en, GPIO_HIGH);
	gpio_write (this->_rw, GPIO_LOW);
	gpio_write (this->_rs, rs);

	for (uint8_t i = FIRST_BIT (this->_itfmode); i < 8; i++)
		gpio_write (this->_db [i], (data & (1 << i)) != 0);
	
	gpio_write (this->_en, GPIO_LOW);

	_delay_ms (10);
}

void hd44780gpio_write (hd44780gpio_t* this,
			uint8_t	       rs,
			uint8_t	       data)
{
	system_enter_critical ();

	if (this->_itfmode == INTERFACE_MODE_4BIT) {
		write_byte (this, rs, data & 0xF0);
		write_byte (this, rs, (data & 0x0F) << 4);
	} else {
		write_byte (this, rs, data);
	}

	system_exit_critical ();
}

uint8_t hd44780gpio_read (hd44780gpio_t* object,
			  uint8_t	 rs)
{
	return 0;
}
