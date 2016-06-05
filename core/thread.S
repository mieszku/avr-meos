;
; Copyright (c) 2016 Mieszko Mazurek
;

#include "thread.S.h"

spl	= 0x3D
sph	= 0x3E
sreg	= 0x3F

rtmp	= 0
rnull	= 1

.macro	push_context
	push	r0
	push	r1
	push	r2
	push	r3
	push	r4
	push	r5
	push	r6
	push	r7
	push	r8
	push	r9
	push	r10
	push	r11
	push	r12
	push	r13
	push	r14
	push	r15
	push	r16
	push	r17
	push	r18
	push	r19
	push	r20
	push	r21
	push	r22
	push	r23
	push	r24
	push	r25
	push	r26
	push	r27
	push	r28
	push	r29
	push	r30
	push	r31
	in	r0,	sreg
	push	r0
.endm

.macro	pop_context
	pop	r0
	out	sreg,	r0
	pop	r31
	pop	r30
	pop	r29
	pop	r28
	pop	r27
	pop	r26
	pop	r25
	pop	r24
	pop	r23
	pop	r22
	pop	r21
	pop	r20
	pop	r19
	pop	r18
	pop	r17
	pop	r16
	pop	r15
	pop	r14
	pop	r13
	pop	r12
	pop	r11
	pop	r10
	pop	r9
	pop	r8
	pop	r7
	pop	r6
	pop	r5
	pop	r4
	pop	r3
	pop	r2
	pop	r1
	pop	r0
.endm

.global	thread_current
.global	thread_main

current	= thread_current

.section .data
	mainname:
		.byte	'm'
		.byte	'a'
		.byte	'i'
		.byte	'n'
		.byte	0

	thread_main:
		.word	thread_main
		.word	thread_main
		.word	0	; sptr
		.word	0	; spnd
		.byte	0	; flags
		.byte	0	; pending
		.word	mainname
	
	thread_current:
		.word	thread_main


.global	thread_exec
.global	thread_kill

.global	__thread_yield

.section .text
	thread_exec:
		;	r24-r25:	run
		;	r22-r23:	obj
		;	r20-r21:	name
		;	r18-r19:	thread
		;	r16-r17:	stack
		;	r14-r17:	size
		push	r28
		push	r29

		movw	r6,	r18
		movw	r8,	r20
		movw	r10,	r22
		movw	r12,	r24

		movw	r26,	r16
		add	r26,	r14
		adc	r27,	r15
		sbiw	r26,	1

		;call	system_enter_critical

		rcall	_fork
		cpse	r26,	rnull
		rjmp	.Lchild

		movw	r28,	r18
		subi	r16,	lo8(-STC_MARGIN)
		sbci	r17,	hi8(-STC_MARGIN)

		std	Y+OFF_SPTRL,	r30
		std	Y+OFF_SPTRH,	r31
		std	Y+OFF_SPNDL,	r16
		std	Y+OFF_SPNDH,	r17
		std	Y+OFF_NAMEL,	r20
		std	Y+OFF_NAMEH,	r21
		std	Y+OFF_FLAG,	rnull

		movw	r24,	r18
		rcall	_insert_node
		movw	r24,	r18

		pop	r29
		pop	r28
		ret
		;jmp	system_exit_critical
	.Lchild:
		movw	r30,	r12
		movw	r24,	r10
		icall
		rjmp	thread_kill

	_fork:	; takes new stack pointer in r26:27
		; returns stack pointer in r30:r31
		; and clr r26
		pop	r31
		pop	r30
		push	r30
		push	r31

		push	r2
		push	r3
		in	r2,	spl
		in	r3,	sph

		out	spl,	r26
		out	sph,	r27
		push	r30
		push	r31
		ser	r26
		push_context
		clr	r26
		in	r30,	spl
		in	r31,	sph

		out	spl,	r2
		out	sph,	r3
		pop	r3
		pop	r2
		ret

	thread_kill:
		call	system_yield

		lds	r24,	current
		lds	r25,	current+1
		movw	r28,	r24
		rcall	_remove_node

		; free flag check
		ldd	r18,	Y+OFF_FLAG
		andi	r18,	FLAG_FREE
		tst	r18
		breq	.Lnofree
		movw	r24,	r28
		call	memfree
	.Lnofree:
		; free flag check

		ldd	r30,	Y+OFF_NEXTL
		ldd	r31,	Y+OFF_NEXTH
		ldd	r24,	Z+OFF_SPTRL
		ldd	r25,	Z+OFF_SPTRH
		sts	current,	r30
		sts	current+1,	r31
		cli
		out	spl,	r24
		out	sph,	r25
		call	system_sei
		pop_context
		jmp	__system_awake

	__thread_yield:
		push_context
		clr	rnull
		call	__system_update_time

		in	r22,	spl
		in	r23,	sph
		lds	r28,	current
		lds	r29,	current+1

		std	Y+OFF_SPTRL,	r22
		std	Y+OFF_SPTRH,	r23
		ldd	r30,	Y+OFF_NEXTL
		ldd	r31,	Y+OFF_NEXTH
		ldd	r24,	Z+OFF_SPTRL
		ldd	r25,	Z+OFF_SPTRH
		sts	current,	r30
		sts	current+1,	r31
		out	spl,	r24
		out	sph,	r25

		;call	__system_update_time
		call	__hwport_clear_int
		pop_context
		reti
	
	_insert_node:
		; takes pointer to thread at r24:r25
		push	r28
		push	r29

		lds	r30,	current
		lds	r31,	current+1
		ldd	r28,	Z+OFF_PREVL
		ldd	r29,	Z+OFF_PREVH
		std	Y+OFF_NEXTL,	r24
		std	Y+OFF_NEXTH,	r25
		std	Z+OFF_PREVL,	r24
		std	Z+OFF_PREVH,	r25
		movw	r26,	r30
		movw	r30,	r24
		std	Z+OFF_PREVL,	r28
		std	Z+OFF_PREVH,	r29
		std	Z+OFF_NEXTL,	r26
		std	Z+OFF_NEXTH,	r27

		pop	r29
		pop	r28
		ret
	
	_remove_node:
		; takes pointer to thread at r24:r25
		push	r28
		push	r29

		movw	r28,	r24
		ldd	r26,	Y+OFF_PREVL
		ldd	r27,	Y+OFF_PREVH
		ldd	r30,	Y+OFF_NEXTL
		ldd	r31,	Y+OFF_NEXTH
		movw	r28,	r26
		std	Y+OFF_NEXTL,	r30
		std	Y+OFF_NEXTH,	r31
		std	Z+OFF_PREVL,	r28
		std	Z+OFF_PREVH,	r29

		pop	r29
		pop	r28
		ret
