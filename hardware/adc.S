;
; Copyright (c) 2016 Mieszko Mazurek
;

rtmp	= 0
rnull	= 1

.global adc_read
.global adc_read_hres
.global adc_disable

.section .data
	mtx:
		.byte	0

#ifdef __AVR_ATmega328P__
	ADMUX	= 0x7C
	ADCSRA	= 0x7A
	ADCL	= 0x78
	ADCH	= 0x79
	ADCSRB	= 0x7B
	DIDR0	= 0x7E

	ADLAR	= 0x20
	REFS0	= 0x40
	REFS1	= 0x80
	ADPS0	= 0x01
	ADPS1	= 0x02
	ADPS2	= 0x04
	ADIE	= 0x08
	ADIF	= 0x10
	ADATE	= 0x20
	ADSC	= 0x40
	ADEN	= 0x80
	ADTS0	= 0x01
	ADTS1	= 0x02
	ADTS2	= 0x04
#elif defined __AVR_ATmega32__
	ADMUX	= 0x27
	ADCSRA	= 0x26
	ADCL	= 0x24
	ADCH	= 0x25

	ADLAR	= 0x20
	REFS0	= 0x40
	REFS1	= 0x80
	ADPS0	= 0x01
	ADPS1	= 0x02
	ADPS2	= 0x04
	ADIE	= 0x08
	ADIF	= 0x10
	ADATE	= 0x20
	ADSC	= 0x40
	ADEN	= 0x80
#endif

PRESC	= ADPS0|ADPS1|ADPS2

.section .text
	adc_read:
		ori	r24,	ADLAR
		rcall	adc_read_hres
		mov	r24,	r25
		ret

	adc_read_hres:
		push	r16
		push	r17
		mov	r16,	r24
		ldi	r24,	lo8(mtx)
		ldi	r25,	hi8(mtx)
		call	mutex_lock
		ori	r16,	REFS0
		sts	ADMUX,	r16
		ldi	r16,	PRESC|ADSC|ADEN
		sts	ADCSRA,	r16
	.Lchkagain:
		lds	r18,	ADCSRA
		andi	r18,	ADIF
		breq	.Lchkagain
		ldi	r18,	ADIF
		sts	ADCSRA,	r18
		lds	r16,	ADCL
		lds	r17,	ADCH
		ldi	r24,	lo8(mtx)
		ldi	r25,	hi8(mtx)
		call	mutex_unlock
		movw	r24,	r16
		pop	r17
		pop	r16
		ret

	adc_disable:
		sts	ADMUX,	rnull
		ret
