;
; Copyright 2016 Mieszko Mazurek
;

#include <config.h>

__zero	= 1

.data

#ifdef __atmega328p__
	; !0x20, 0x23, 0x26, 0x29
	.macro	ld_pin_addr
		ldi	r18,	3
		mul	r25,	r18
		ldi	r26,	0x20
		add	r26,	r0
		clr	r27
	.endm

	; !0x21, 0x24, 0x27, 0x2A
	.macro	ld_ddr_addr
		ldi	r18,	3
		mul	r25,	r18
		ldi	r26,	0x21
		add	r26,	r0
		clr	r27
	.endm

	; !0x22, 0x25, 0x28, 0x2B
	.macro	ld_port_addr
		ldi	r18,	3
		mul	r25,	r18
		ldi	r26,	0x22
		add	r26,	r0
		clr	r27
	.endm
#elif defined (__atmega32__)
	; 0x39, 0x36, 0x33, 0x30
	.macro	ld_pin_addr
		ldi	r18,	3
		mul	r25,	r18
		ldi	r26,	0x39
		sub	r26,	r0
		clr	r27
	.endm
	
	; 0x3A, 0x37, 0x34, 0x31
	.macro	ld_ddr_addr
		ldi	r18,	3
		mul	r25,	r18
		ldi	r26,	0x3A
		sub	r26,	r0
		clr	r27
	.endm

	; 0x3B, 0x38, 0x35, 0x32
	.macro	ld_port_addr
		ldi	r18,	3
		mul	r25,	r18
		ldi	r26,	0x3B
		sub	r26,	r0
		clr	r27
	.endm
#elif defined (__atmega2560__)
	; TODO avoid labels in that macros
	; 0x20, 0x23, 0x26, 0x29, 0x2C, 0x2F, 0x32, 0x100, 0x103, 0x106, 0x109
	.macro	ld_pin_addr
		ldi	r18,	3
		cpi	r25,	7
		brsh	L_space_1
	L_space_0:
		ldi	r26,	0x20
		mul	r25,	r18
		add	r26,	r0
		clr	r27
		rjmp	L_space_end
	L_space_1:
		ldi	r26,	0
		mul	r25,	r18
		add	r26,	r0
		ldi	r27,	1
	L_space_end:
	.endm
	
	; 0x21, 0x24, 0x27, 0x2A, 0x2D, 0x30, 0x33, 0x101, 0x104, 0x107, 0x10A
	.macro	ld_ddr_addr
		ldi	r18,	3
		cpi	r25,	7
		brsh	L_ddr_1
	L_ddr_0:
		ldi	r26,	0x21
		mul	r25,	r18
		add	r26,	r0
		clr	r27
		rjmp	L_ddr_end
	L_ddr_1:
		ldi	r26,	1
		mul	r25,	r18
		add	r26,	r0
		ldi	r27,	1
	L_ddr_end:
	.endm

	; 0x22, 0x25, 0x28, 0x2B, 0x2E, 0x21, 0x34, 0x102, 0x105, 0x108, 0x10B
	.macro	ld_port_addr
		ldi	r18,	3
		cpi	r25,	7
		brsh	L_port_1
	L_port_0:
		ldi	r26,	0x22
		mul	r25,	r18
		add	r26,	r0
		clr	r27
		rjmp	L_port_end
	L_port_1:
		ldi	r26,	2
		mul	r25,	r18
		add	r26,	r0
		ldi	r27,	1
	L_port_end:
	.endm
#else
	#error unsupported MCU
#endif

input	= 0
output	= 1
low	= 0
high 	= 1

.text

.global	gpio_mode
.global gpio_mode_input
.global gpio_mode_output
.global gpio_write
.global gpio_write_low
.global gpio_write_high
.global gpio_toggle
.global gpio_read

	write_bits:
		add	r26,	r25
		adc	r27,	__zero
		ld	r26,	X
		clr	r27
		ld	r18,	X
		cpse	r22,	__zero
		rjmp	L_set_bits
		com	r24
		and	r18,	r24
		rjmp	L_store_bits
	L_set_bits:
		or	r18,	r24
	L_store_bits:
		st	X,	r18
		ret

	gpio_mode:
		ld_ddr_addr
		ld	r18,	X
		cpse	r22,	__zero
		rjmp	L_mode_set
		com	r24
		and	r18,	r24
		rjmp	L_mode_store
	L_mode_set:
		or	r18,	r24
	L_mode_store:
		st	X,	r18
		ret
	
	gpio_write:
		ld_port_addr
		ld	r18,	X
		cpse	r22,	__zero
		rjmp	L_write_set
		com	r24
		and	r18,	r24
		rjmp	L_write_store
	L_write_set:
		or	r18,	r24
	L_write_store:
		st	X,	r18
		ret
	
	gpio_toggle:
		ld_port_addr
		ld	r18,	X
		eor	r18,	r24
		st	X,	r18
		ret
	
	gpio_read:
		ld_pin_addr
		ld	r18,	X
		and	r18,	r24
		cpse	r18,	__zero
		inc	r18
		ret


	gpio_mode_input:
		ldi	r22,	input
		rjmp	gpio_mode
	
	gpio_mode_output:
		ldi	r22,	output
		rjmp	gpio_mode
	
	gpio_write_low:
		ldi	r22,	low
		rjmp	gpio_write

	gpio_write_high:
		ldi	r22,	high
		rjmp	gpio_write

.global __do_copy_data
