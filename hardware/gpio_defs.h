/*
 * Copyright (c) 2016 Mieszko Mazurek
 */

#ifndef __GPIO_DEFS_H
#define __GPIO_DEFS_H

#include <config.h>

#define GPIO_INPUT      0x000   
#define GPIO_OUTPUT     0x001   
#define GPIO_LOW        0x000   
#define GPIO_HIGH       0x001

#ifdef __atmega328p__
	#define __gpio_portb__
	#define __gpio_portc__
	#define __gpio_portd__
#endif

#ifdef __atmega32__
	#define __gpio_porta__
	#define __gpio_portb__
	#define __gpio_portc__
	#define __gpio_portd__
#endif

#ifdef __gpio_porta__
	#define GPIO_PINA0      0x001
	#define GPIO_PINA1      0x002
	#define GPIO_PINA2      0x004
	#define GPIO_PINA3      0x008
	#define GPIO_PINA4      0x010
	#define GPIO_PINA5      0x020
	#define GPIO_PINA6      0x040
	#define GPIO_PINA7      0x080
#endif
#ifdef __gpio_portb__
	#define GPIO_PINB0      0x101
	#define GPIO_PINB1      0x102
	#define GPIO_PINB2      0x104
	#define GPIO_PINB3      0x108
	#define GPIO_PINB4      0x110
	#define GPIO_PINB5      0x120
	#define GPIO_PINB6      0x140
	#define GPIO_PINB7      0x180
#endif
#ifdef __gpio_portc__
	#define GPIO_PINC0      0x201
	#define GPIO_PINC1      0x202
	#define GPIO_PINC2      0x204
	#define GPIO_PINC3      0x208
	#define GPIO_PINC4      0x210
	#define GPIO_PINC5      0x220
	#define GPIO_PINC6      0x240
	#define GPIO_PINC7      0x280
#endif
#ifdef __gpio_portd__
	#define GPIO_PIND0      0x301
	#define GPIO_PIND1      0x302
	#define GPIO_PIND2      0x304
	#define GPIO_PIND3      0x308
	#define GPIO_PIND4      0x310
	#define GPIO_PIND5      0x320   
	#define GPIO_PIND6      0x340
	#define GPIO_PIND7      0x380
#endif
#ifdef __gpio_porte__
	#define GPIO_PINE0      0x401
	#define GPIO_PINE1      0x402
	#define GPIO_PINE2      0x404
	#define GPIO_PINE3      0x408
	#define GPIO_PINE4      0x410
	#define GPIO_PINE5      0x420   
	#define GPIO_PINE6      0x440
	#define GPIO_PINE7      0x480
#endif
#ifdef __gpio_portf__
	#define GPIO_PINF0      0x501
	#define GPIO_PINF1      0x502
	#define GPIO_PINF2      0x504
	#define GPIO_PINF3      0x508
	#define GPIO_PINF4      0x510
	#define GPIO_PINF5      0x520   
	#define GPIO_PINF6      0x540
	#define GPIO_PINF7      0x580
#endif
#ifdef __gpio_portg__
	#define GPIO_PING0      0x601
	#define GPIO_PING1      0x602
	#define GPIO_PING2      0x604
	#define GPIO_PING3      0x608
	#define GPIO_PING4      0x610
	#define GPIO_PING5      0x620   
	#define GPIO_PING6      0x640
	#define GPIO_PING7      0x680
#endif
#ifdef __gpio_porth__
	#define GPIO_PINH0      0x701
	#define GPIO_PINH1      0x702
	#define GPIO_PINH2      0x704
	#define GPIO_PINH3      0x708
	#define GPIO_PINH4      0x710
	#define GPIO_PINH5      0x720   
	#define GPIO_PINH6      0x740
	#define GPIO_PINH7      0x780
#endif
#ifdef __gpio_portj__
	#define GPIO_PINJ0      0x801
	#define GPIO_PINJ1      0x802
	#define GPIO_PINJ2      0x804
	#define GPIO_PINJ3      0x808
	#define GPIO_PINJ4      0x810
	#define GPIO_PINJ5      0x820   
	#define GPIO_PINJ6      0x840
	#define GPIO_PINJ7      0x880
#endif
#ifdef __gpio_portk__
	#define GPIO_PINK0      0x901
	#define GPIO_PINK1      0x902
	#define GPIO_PINK2      0x904
	#define GPIO_PINK3      0x908
	#define GPIO_PINK4      0x910
	#define GPIO_PINK5      0x920   
	#define GPIO_PINK6      0x940
	#define GPIO_PINK7      0x980
#endif
#ifdef __gpio_portl__
	#define GPIO_PINL0      0xA01
	#define GPIO_PINL1      0xA02
	#define GPIO_PINL2      0xA04
	#define GPIO_PINL3      0xA08
	#define GPIO_PINL4      0xA10
	#define GPIO_PINL5      0xA20   
	#define GPIO_PINL6      0xA40
	#define GPIO_PINL7      0xA80
#endif

#ifdef __arduino_uno__
	#define GPIO_PIN0	GPIO_PIND0
	#define GPIO_PIN1	GPIO_PIND1
	#define GPIO_PIN2	GPIO_PIND2
	#define GPIO_PIN3	GPIO_PIND3
	#define GPIO_PIN4	GPIO_PIND4
	#define GPIO_PIN5	GPIO_PIND5
	#define GPIO_PIN6	GPIO_PIND6
	#define GPIO_PIN7	GPIO_PIND7
	#define GPIO_PIN8	GPIO_PINB0
	#define GPIO_PIN9	GPIO_PINB1
	#define GPIO_PIN10	GPIO_PINB2
	#define GPIO_PIN11	GPIO_PINB3
	#define GPIO_PIN12	GPIO_PINB4
	#define GPIO_PIN13	GPIO_PINB5
#endif

#endif
