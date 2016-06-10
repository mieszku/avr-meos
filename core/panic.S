;
; Copyright (c) 2016 Mieszko Mazurek
;

#include "thread.S.h"

rnull	= 1

spl	= 0x3D
sph	= 0x3E

.section .bss
	panic_sp:
		.word	0

.global enter_panic
.global __panic_init

.section .text
	enter_panic:
		movw	r2,	r24
		call	system_enter_critical
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
		lds	r4,	panic_sp
		lds	r5,	panic_sp+1
		out	spl,	r4
		out	sph,	r5
		call	system_exit_critical
		movw	r24,	r2
		call	panic
	.Lloop:
		rjmp	.Lloop
	
	__panic_init:
		in	r24,	spl
		in	r25,	sph
		sts	panic_sp,	r24
		sts	panic_sp+1,	r25
		ret
