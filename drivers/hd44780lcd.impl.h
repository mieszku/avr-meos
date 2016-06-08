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

static inline
uint8_t hd44780lcd_get_y (hd44780lcd_t* this)
{
	return this->_posy;
}

static inline
uint8_t hd44780lcd_get_x (hd44780lcd_t* this)
{
	return this->_posx;
}

#endif
