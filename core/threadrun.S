;
; Copyright (c) 2016 Mieszko Mazurek
;

#include "config.h"
#include "threadcfg.h"

rnull	= 1

.global	thread_run
.global	thread_run_alloc

EXTRA	= THREAD_SIZE+CTX_SIZE+STACK_MARGIN

.section .text
	thread_run:
		push	r14
		push	r15

		subi	r16,	lo8(THREAD_SIZE)
		sbci	r17,	hi8(THREAD_SIZE)
		movw	r14,	r16
		movw	r16,	r18
		subi	r16,	lo8(-THREAD_SIZE)
		sbci	r17,	hi8(-THREAD_SIZE)
		call	thread_exec

		pop	r15
		pop	r14
		ret
	
	thread_run_alloc:
		push	r16
		push	r17

		push	r20
		push	r21
		push	r22
		push	r23
		push	r24
		push	r25

		movw	r24,	r18
		subi	r24,	lo8(-EXTRA)
		sbci	r25,	hi8(-EXTRA)
		movw	r16,	r24

		call	memalloc

		cp	r24,	rnull
		cpc	r25,	rnull
		breq	.Lallocfailed

		movw	r18,	r24
		
		pop	r25
		pop	r24
		pop	r23
		pop	r22
		pop	r21
		pop	r20

		call	system_enter_critical
		ldi	r26,	FLAG_FREE
		sts	__thread_flags,	r26
		call	thread_run

		pop	r17
		pop	r16
		ret
		
	.Lallocfailed:
		pop	r25
		pop	r24
		pop	r23
		pop	r22
		pop	r21
		pop	r20

		pop	r17
		pop	r16
		ret
