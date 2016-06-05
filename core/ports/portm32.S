;
; Copyright (c) 2016 Mieszko Mazurek
;

#include "config.h"

#ifdef __atmega32__

rnull	= 1

sreg 	= 0x3F

TIMSK	= 0x59
TIFR	= 0x58

OCRA	= ((F_CPU/1000/PRESC/__system_timer_mult__)-1)

#define BREAK	(512/PRESC)

.global	__hwport_init
.global	__hwport_sync_clock
.global __hwport_get_tick
.global __hwport_get_int
.global __hwport_enable
.global __hwport_disable

.global	__vector_7
.global	__vector_8

.section .text

#if	(__system_timer__ == 1)
	TCCR0	= 0x53
	TCNT0	= 0x52
	OCR0	= 0x5c

	TCCR1A	= 0x4f
	TCCR1B	= 0x4e
	TCNT1L	= 0x4c
	TCNT1H	= 0x3d
	OCR1AL	= 0x4a
	OCR1AH	= 0x4b
	OCR1BL	= 0x48
	OCR1BH	= 0x49

	TCCR2	= 0x45
	TCNT2	= 0x44
	OCR2	= 0x43

	CS00	= 0x01
	CS01	= 0x02
	CS02	= 0x04
	WGM00	= 0x40
	WGM01	= 0x08
	OCIE0	= 0x02

	CS10	= 0x01
	CS11	= 0x02
	CS12	= 0x04
	WGM10	= 0x01
	WGM11	= 0x02
	WGM12	= 0x08
	WGM13	= 0x10
	OCIE1A	= 0x10
	OCIE1B	= 0x08
	OCF1A	= 0x10
	OCF1B	= 0x08

	CS20	= 0x01
	CS21	= 0x02
	CS22	= 0x04
	WGM20	= 0x40
	WGM21	= 0x08
	OCIE2	= 0x80

	PRESC	= 1

	__hwport_init:
		cli
		ldi	r18,	WGM12|CS10
		sts	TCCR1A,	rnull
		sts	TCCR1B,	r18

		ldi	r18,	lo8(OCRA)
		ldi	r19,	hi8(OCRA)
		sts	OCR1AH,	r19
		sts	OCR1AL,	r18

		sts	OCR1BH,	rnull
		sts	OCR1BL,	rnull

		ldi	r18,	OCIE1B
		sts	TIMSK,	r18
		reti
	
	__hwport_sync_clock:
		lds	r27,	TCNT1H
		lds	r26,	TCNT1L
		subi	r26,	3
		sts	OCR1BH,	r27
		sts	OCR1BL,	r26
		ret
	
	__hwport_get_tick:
		ldi	r25,	OCF1A
		lds	r24,	TIFR
		sts	TIFR,	r25
		and	r24,	r25
		ret
	
	__hwport_get_int:
		ldi	r25,	OCF1B
		lds	r24,	TIFR
		sts	TIFR,	r25
		and	r24,	r25
		ret
	
	__hwport_enable:
		ldi	r26,	OCIE1B
		sts	TIMSK,	r26
		ret
	
	__hwport_disable:
		sts	TIMSK,	rnull
		ret
	
	__vector_8:
		sei
		jmp	__system_clock
#else
	#error unsupported system timer
#endif

#endif
