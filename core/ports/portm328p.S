;
; Copyright (c) 2016 Mieszko Mazurek
;

rnull	= 1

#include <config.h>

#ifdef 	__atmega328p__

.global __hwport_init
.global __hwport_sync_clock
.global __hwport_check_int
.global __hwport_enable
.global __hwport_disable
.global __hwport_callback

.global	__vector_11
.global __vector_12

#ifdef __system_timer_x2__
	OCRA	= ((F_CPU/1000/PRESC/2)-1)
#else
	OCRA	= ((F_CPU/1000/PRESC)-1)
#endif

#if	(__system_timer__ == 0)
	
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

#elif	(__system_timer__ == 1)

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

.section .text
	__hwport_init:
		ldi	r18,	WGM12|CS10
		sts	TCCR1A,	rnull
		sts	TCCR1B,	r18
		sts	TCCR1C,	rnull

		ldi	r18,	lo8(OCRA)
		ldi	r19,	hi8(OCRA)
		sts	OCR1AH,	r19
		sts	OCR1AL,	r18

		ldi	r18,	lo8(OCRA/2)
		ldi	r19,	hi8(OCRA/2)
		sts	OCR1BH,	r19
		sts	OCR1BL,	r18

		ldi	r18,	OCIE1B
		sts	TIMSK1,	r18
		reti
	
	__hwport_sync_clock:
		push	r26
		push	r27

		clr	r26
		sts	TIMSK1,	r26

		lds	r27,	TCNT1H
		lds	r26,	TCNT1L
		sts	OCR1BH,	r27
		sts	OCR1BL,	r26

		ldi	r26,	OCIE1B
		sts	TIMSK1,	r26

		pop	r27
		pop	r26
		jmp	__vector_12

	__hwport_check_int:
		push	r26
		push	rnull
		clr	rnull

		lds	r26,	TIFR1
		andi	r26,	OCF1B
		cpse	r26,	rnull
		call	__vector_12
		ldi	r26,	OCF1B
		sts	TIFR1,	r26

		pop	rnull
		pop	r26
		ret

	__hwport_enable:
		push	r26
		ldi	r26,	OCIE1B
		sts	TIMSK1,	r26
		pop	r26
		ret
	
	__hwport_disable:
		push	rnull
		clr	rnull
		sts	TIMSK1,	rnull
		pop	rnull
		ret

	__hwport_callback:
		push	r26
		push	rnull
		clr	rnull

		lds	r26,	TIFR1
		andi	r26,	OCF1A
		cpse	r26,	rnull
		call	__system_clock_mono
		ldi	r26,	OCF1A
		sts	TIFR1,	r26

		pop	rnull
		pop	r26
		ret
	
	__vector_11:
		jmp	__system_clock_mono
	
	__vector_12:
		sei
		push	r26
		push	rnull
		clr	rnull

		lds	r26,	TIFR1
		andi	r26,	OCF1A
		cpse	r26,	rnull
		call	__system_clock_mono
		ldi	r26,	OCF1A
		sts	TIFR1,	r26

		pop	rnull
		pop	r26
		jmp	__system_clock

#else
	#error unsupported system timer
#endif
	
#endif
