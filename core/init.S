;
; Copyright (c) 2016 Mieszko Mazurek
;

#include "thread.S.h"
#include "config.h"

.extern	__task_handle

STSIZ	= SYSTEM_STACK_SIZE+CTX_SIZE+STC_MARGIN

.section .data
	sysname:
		.asciz	"system"
	
.section .noinit
	systhread:
		.zero	THR_SIZE
	
	sysstack:
		.zero	STSIZ


.global init_system
.global init_systhread

.section .fini8
	call	thread_kill

.section .text
	_systhread:
		call	__task_handle
		call	system_yield
		rjmp	_systhread

	init_system:
		call	__hwport_init
		ldi	r24,	lo8(gs(_systhread))
		ldi	r25,	hi8(gs(_systhread))
		ldi	r20,	lo8(sysname)
		ldi	r21,	hi8(sysname)
		ldi	r18,	lo8(systhread)
		ldi	r19,	hi8(systhread)
		ldi	r16,	lo8(sysstack)
		ldi	r17,	hi8(sysstack)
		ldi	r26,	lo8(STSIZ)
		ldi	r27,	hi8(STSIZ)
		movw	r14,	r26
		call	thread_exec
		ret
