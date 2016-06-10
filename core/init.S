;
; Copyright (c) 2016 Mieszko Mazurek
;

#include "threadcfg.h"
#include "config.h"

STSIZ	= SYSTEM_STACK_SIZE+CTX_SIZE+STACK_MARGIN

.global system_thread

.section .data
	sysname:
		.asciz	"system"
	
.section .noinit
	system_thread:
		.zero	THREAD_SIZE
	
	sysstack:
		.zero	STSIZ

.global	system_init

.section .text
	_systhread:
		call	__task_handle
		call	system_yield
		rjmp	_systhread

	system_init:
		call	__panic_init
		call	__hwport_init
		call	__memalloc_reset
		ldi	r24,	lo8(gs(_systhread))
		ldi	r25,	hi8(gs(_systhread))
		ldi	r20,	lo8(sysname)
		ldi	r21,	hi8(sysname)
		ldi	r18,	lo8(system_thread)
		ldi	r19,	hi8(system_thread)
		ldi	r16,	lo8(sysstack)
		ldi	r17,	hi8(sysstack)
		ldi	r26,	lo8(STSIZ)
		ldi	r27,	hi8(STSIZ)
		movw	r14,	r26
		call	thread_exec
		ldi	r24,	50
		clr	r25
		jmp	system_sleep
