/*
 * Copyright (c) 2016 Mieszko Mazurek
 */

#ifndef __HD44780LCD_IMPL_H
#define __HD44780LCD_IMPL_H

static inline
void hd44780lcd_set_position (hd44780lcd_t* this,
			      uint8_t	    posy,
			      uint8_t	    posx)
{
	this->_posy = posy;
	this->_posx = posx;
}

#endif
