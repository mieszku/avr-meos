;
; Copyright (c) 2016 Mieszko Mazurek
;

rnull	= 1

#include <config.h>

#ifdef 	__AVR_ATmega328P__

.global __hwport_init
.global __hwport_sync_clock
.global __hwport_get_tick
.global	__hwport_get_int
.global __hwport_clr_int
.global __hwport_enable
.global __hwport_disable

.global	__vector_11
.global __vector_12

OCRA	= ((F_CPU/1000/PRESC/__system_timer_mult__)-1)

.section .text

#if	__system_timer__ == 0
		
	TCCR0A	= 0x44
	TCCR0B	= 0x45
	TCNT0	= 0x46
	OCR0A	= 0x47
	OCR0B	= 0x48
	TIMSK0	= 0x6E
	TIFR0	= 0x35

	CS00	= 0x01
	CS01	= 0x02
	CS02	= 0x04
	WGM00	= 0x01
	WGM01	= 0x02
	WGM02	= 0x08
	OCIE0A	= 0x02
	OCIE0B	= 0x04
	OCF1A	= 0x02
	OCF1B	= 0x04

	; TODO

#elif	__system_timer__ == 1

	TCCR1A	= 0x80
	TCCR1B	= 0x81
	TCCR1C	= 0x82
	TCNT1L	= 0x84
	TCNT1H	= 0x85
	OCR1AL	= 0x88
	OCR1AH	= 0x89
	OCR1BL	= 0x8A
	OCR1BH	= 0x8B
	TIMSK1	= 0x6F
	TIFR1	= 0x36

	CS10	= 0x01
	CS11	= 0x02
	CS12	= 0x04
	WGM10	= 0x01
	WGM11	= 0x02
	WGM12	= 0x08
	OCIE1A	= 0x02
	OCIE1B	= 0x04
	OCF1A	= 0x02
	OCF1B	= 0x04

	PRESC	= 1

	__hwport_init:
		ldi	r18,	WGM12|CS10
		sts	TCCR1A,	rnull
		sts	TCCR1B,	r18
		sts	TCCR1C,	rnull

		ldi	r18,	lo8(OCRA)
		ldi	r19,	hi8(OCRA)
		sts	OCR1AH,	r19
		sts	OCR1AL,	r18

		sts	OCR1BH,	rnull
		sts	OCR1BL,	rnull

		ldi	r18,	OCIE1B
		sts	TIMSK1,	r18
		reti
	
	__hwport_sync_clock:
		lds	r25,	TCNT1H
		lds	r24,	TCNT1L
		subi	r24,	3
		sts	OCR1BH,	r25
		sts	OCR1BL,	r24
		ret
	
	__hwport_get_tick:
		ldi	r25,	OCF1A
		lds	r24,	TIFR1
		sts	TIFR1,	r25
		andi	r24,	OCF1A
		ret

	__hwport_get_int:
		ldi	r25,	OCF1B
		lds	r24,	TIFR1
		sts	TIFR1,	r25
		and	r24,	r25
		ret
	
	__hwport_clr_int:
		ldi	r24,	OCF1B
		sts	TIFR1,	r24
		ret

	__hwport_enable:
		ldi	r24,	OCIE1B
		sts	TIMSK1,	r24
		ret
	
	__hwport_disable:
		clr	rnull
		sts	TIMSK1,	rnull
		ret
	
	__vector_12:
		sei
		jmp	__system_clock

#else
	#error unsupported system timer
#endif
	
#endif
