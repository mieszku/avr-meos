;
; Copyright (c) 2016 Mieszko Mazurek
;

rnull	= 1

spl	= 0x3D
sph	= 0x3E

.global __panic_mode

.section .bss
	__panic_mode:
		.byte	0
	
	panic_sp:
		.word	0

.global enter_panic
.global __panic_init

.section .text
	enter_panic:
		ret
		/*
		call	__hwport_disable
		ser	r16
		sts	__panic_mode,	r16
		call	panic
	.Lloop:
		rjmp	.Lloop
		*/
	
	__panic_init:
		in	r24,	spl
		in	r25,	sph
		sts	panic_sp,	r24
		sts	panic_sp+1,	r25
		sts	__panic_mode,	rnull
		ret
