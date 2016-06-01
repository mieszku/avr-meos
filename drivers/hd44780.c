/*
 * Copyright (c) 2016 Mieszko Mazurek
 */

#include "hd44780.h"

#include <memalloc.h>

static hd44780vt_t hd44780vt;

__attribute__ ((constructor))
static void __init_vt (void)
{
	hd44780vt_init (&hd44780vt);
}

void hd44780vt_init (hd44780vt_t* vtable)
{
	objectvt_init ((objectvt_t*) vtable);

	((objectvt_t*) vtable)->destruct = 
		(void (*) (object_t*)) hd44780_destruct;
}


hd44780_t* hd44780_new (hd44780itf_t* interface)
{
	hd44780_t* instance = xmemalloc (sizeof (hd44780_t));
	hd44780_construct (instance, interface);
	return instance;
}

void hd44780_construct (hd44780_t*    this,
			hd44780itf_t* interface)
{
	object_construct (OBJECT (this));
	OBJECT_VTABLE (this) = (objectvt_t*) &hd44780vt;
	this->_interface = interface;
}

void hd44780_destruct (hd44780_t* this)
{
	object_unref (this->_interface);
	object_destruct (OBJECT (this));
}

void hd44780_display_clear (hd44780_t* this)
{
	hd44780itf_write (this->_interface, 0, 0x1);
}

void hd44780_cursor_home (hd44780_t* this)
{
	hd44780itf_write (this->_interface, 0, 0x2);
}

void hd44780_entry_mode_set (hd44780_t* this,
			     uint8_t	entry_mode,
			     uint8_t	entry_direction)
{
	uint8_t data = 0x4 | 
		(entry_mode == ENTRY_MODE_CONTENT) |
		(entry_direction == ENTRY_DIRECTION_RIGHT) << 1;
	hd44780itf_write (this->_interface, 0, data);
}

void hd44780_display_set (hd44780_t* this,
			  uint8_t    display,
			  uint8_t    cursor)
{
	uint8_t data = 0x8 |
		(cursor == CURSOR_MODE_BLOCK) |
		((cursor != CURSOR_MODE_NONE) << 1) |
		((display == DISPLAY_ON) << 2);
	hd44780itf_write (this->_interface, 0, data);
}

void hd44780_shift (hd44780_t* this,
		    uint8_t    shift_mode,
		    uint8_t    shift_direction)
{
	uint8_t data = 0x10 |
		((shift_direction == SHIFT_DIRECTION_RIGHT) << 2) |
		((shift_mode == SHIFT_MODE_CONTENT) << 3);
	hd44780itf_write (this->_interface, 0, data);
}

void hd44780_function_set (hd44780_t* this,
			   uint8_t    interface_mode,
			   uint8_t    line_mode,
			   uint8_t    matrix)
{
	hd44780itf_set_interface_mode (this->_interface, interface_mode);

	if (interface_mode == INTERFACE_MODE_4BIT)
		hd44780itf_write (this->_interface, 0, 0x20);
	
	uint8_t data = 0x20 | 
		((matrix == MATRIX_5X10) << 2) |
		((line_mode == LINE_MODE_TWOLINE) << 3) |
		((interface_mode == INTERFACE_MODE_8BIT) << 4);
	hd44780itf_write (this->_interface, 0, data);
}

void hd44780_cg_ram_set (hd44780_t* this,
			 uint8_t    addr)
{
	hd44780itf_write (this->_interface, 0, 0x40 | addr);
}

void hd44780_dd_ram_set (hd44780_t* this,
			 uint8_t    addr)
{
	hd44780itf_write (this->_interface, 0, 0x80 + addr);
}

uint8_t hd44780_busy_flag_read (hd44780_t* this)
{
	return hd44780itf_read (this->_interface, 0);
}

uint8_t hd44780_data_read (hd44780_t* this)
{
	return hd44780itf_read (this->_interface, 1);
}

void hd44780_data_write (hd44780_t* this,
			 uint8_t    data)
{
	hd44780itf_write (this->_interface, 1, data);
}
