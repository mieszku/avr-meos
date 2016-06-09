;
; Copyright (c) 2016 Mieszko Mazurek
;

#include "thread.S.h"

rnull	= 1

spl	= 0x3D
sph	= 0x3E

;.global __panic_mode

.section .bss
	__panic_mode:
		.byte	0
	
	panic_sp:
		.word	0

.global enter_panic
.global __panic_init

.section .text
	enter_panic:
		movw	r2,	r24
		call	__hwport_disable
		ldi	r30,	lo8(system_thread)
		ldi	r31,	hi8(system_thread)
		lds	r28,	thread_current
		lds	r29,	thread_current+1
		std	Z+OFF_NEXTL,	r28
		std	Z+OFF_NEXTH,	r29
		std	Z+OFF_PREVL,	r28
		std	Z+OFF_PREVH,	r29
		std	Y+OFF_NEXTL,	r30
		std	Y+OFF_NEXTH,	r31
		std	Y+OFF_PREVL,	r30
		std	Y+OFF_PREVH,	r31
		call	__hwport_enable
		movw	r24,	r2
		call	panic
	.Lloop:
		rjmp	.Lloop
	
	__panic_init:
		in	r24,	spl
		in	r25,	sph
		sts	panic_sp,	r24
		sts	panic_sp+1,	r25
		sts	__panic_mode,	rnull
		ret
